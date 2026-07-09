#include "tusb.h"
#include "ff.h"
#include "fs_lock.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>

//------------- device info -------------//
#define DEV_INFO_MANUFACTURER   "Stavros"
#define DEV_INFO_MODEL          "Shuffle Clone"
#define DEV_INFO_VERSION        "0.1"
#define DEV_PROP_FRIENDLY_NAME  "Shuffle"

//------------- storage info -------------//
#define STORAGE_DESCRIPTION { 'S', 'D', ' ', 'C', 'a', 'r', 'd', 0 }
#define VOLUME_IDENTIFIER { 'v', 'o', 'l', 0 }

enum {
  STORAGE_DESC_LEN = TU_ARRAY_SIZE((uint16_t[]) STORAGE_DESCRIPTION),
  VOLUME_ID_LEN = TU_ARRAY_SIZE((uint16_t[])VOLUME_IDENTIFIER)
};

typedef MTP_STORAGE_INFO_STRUCT(STORAGE_DESC_LEN, VOLUME_ID_LEN)
storage_info_t;

storage_info_t storage_info = {
  .storage_type = MTP_STORAGE_TYPE_REMOVABLE_RAM,
  .filesystem_type = MTP_FILESYSTEM_TYPE_GENERIC_HIERARCHICAL,
  .access_capability = MTP_ACCESS_CAPABILITY_READ_WRITE,
  .max_capacity_in_bytes = 0,
  .free_space_in_bytes = 0,
  .free_space_in_objects = 0,
  .storage_description = {
    .count = (TU_FIELD_SIZE(storage_info_t, storage_description) - 1) / sizeof(uint16_t),
    .utf16 = STORAGE_DESCRIPTION,
  },
  .volume_identifier = {
    .count = (TU_FIELD_SIZE(storage_info_t, volume_identifier) - 1) / sizeof(uint16_t),
    .utf16 = VOLUME_IDENTIFIER,
  },
};

//--------------------------------------------------------------------+
// MTP object table backed by FatFs on SD card
//--------------------------------------------------------------------+
#define FS_MAX_OBJECT_COUNT  64u
#define FS_MAX_PATH_LEN      128u
#define FS_FIXED_DATETIME    "20250808T173500.0"

typedef struct {
  bool used;
  bool is_dir;
  uint32_t parent_handle;
  uint32_t size;
  uint16_t object_format;
  uint16_t protection_status;
  char path[FS_MAX_PATH_LEN];
} mtp_obj_t;

static mtp_obj_t mtp_objects[FS_MAX_OBJECT_COUNT];

enum {
  SUPPORTED_STORAGE_ID = 0x00010001u
};

static bool is_session_opened = false;
static uint32_t send_obj_handle = 0;
static bool ul_objinfo_parsed = false;

typedef struct {
  bool active;
  bool file_open;
  bool chunk_ready;
  bool need_receive;
  bool write_error;
  uint32_t file_size;
  uint32_t bytes_written;
  uint16_t chunk_len;
  uint8_t chunk[CFG_TUD_MTP_EP_BUFSIZE];
  char path[FS_MAX_PATH_LEN];
  FIL fil;
} mtp_upload_t;

static mtp_upload_t mtp_up;
static mtp_container_header_t mtp_up_rx_hdr = {.len = sizeof(mtp_container_header_t)};
static mtp_container_info_t mtp_up_rx_io = {.header = &mtp_up_rx_hdr};

static FIL dl_fil;
static bool dl_open = false;

static int32_t fs_get_device_info(tud_mtp_cb_data_t* cb_data);
static int32_t fs_open_close_session(tud_mtp_cb_data_t* cb_data);
static int32_t fs_get_storage_ids(tud_mtp_cb_data_t* cb_data);
static int32_t fs_get_storage_info(tud_mtp_cb_data_t* cb_data);
static int32_t fs_get_device_properties(tud_mtp_cb_data_t* cb_data);
static int32_t fs_get_object_handles(tud_mtp_cb_data_t* cb_data);
static int32_t fs_get_object_info(tud_mtp_cb_data_t* cb_data);
static int32_t fs_get_object(tud_mtp_cb_data_t* cb_data);
static int32_t fs_get_partial_object(tud_mtp_cb_data_t* cb_data);
static int32_t fs_delete_object(tud_mtp_cb_data_t* cb_data);
static int32_t fs_send_object_info(tud_mtp_cb_data_t* cb_data);
static int32_t fs_send_object(tud_mtp_cb_data_t* cb_data);

typedef int32_t (*fs_op_handler_t)(tud_mtp_cb_data_t* cb_data);
typedef struct {
  uint32_t op_code;
  fs_op_handler_t handler;
} fs_op_handler_dict_t;

fs_op_handler_dict_t fs_op_handler_dict[] = {
  {MTP_OP_GET_DEVICE_INFO, fs_get_device_info},
  {MTP_OP_OPEN_SESSION, fs_open_close_session},
  {MTP_OP_CLOSE_SESSION, fs_open_close_session},
  {MTP_OP_GET_STORAGE_IDS, fs_get_storage_ids},
  {MTP_OP_GET_STORAGE_INFO, fs_get_storage_info},
  {MTP_OP_GET_DEVICE_PROP_DESC, fs_get_device_properties},
  {MTP_OP_GET_DEVICE_PROP_VALUE, fs_get_device_properties},
  {MTP_OP_GET_OBJECT_HANDLES, fs_get_object_handles},
  {MTP_OP_GET_OBJECT_INFO, fs_get_object_info},
  {MTP_OP_GET_OBJECT, fs_get_object},
  {MTP_OP_GET_PARTIAL_OBJECT, fs_get_partial_object},
  {MTP_OP_DELETE_OBJECT, fs_delete_object},
  {MTP_OP_SEND_OBJECT_INFO, fs_send_object_info},
  {MTP_OP_SEND_OBJECT, fs_send_object},
};

//--------------------------------------------------------------------+
// FatFs helpers
//--------------------------------------------------------------------+
static mtp_obj_t* mtp_obj_from_handle(uint32_t handle);

static void mtp_upload_reset(void)
{
  fs_lock();
  if(mtp_up.file_open) {
    f_sync(&mtp_up.fil);
    f_close(&mtp_up.fil);
  }
  memset(&mtp_up, 0, sizeof(mtp_up));
  mtp_up_rx_hdr.len = sizeof(mtp_container_header_t);
  mtp_up_rx_io.header = &mtp_up_rx_hdr;
  fs_unlock();
}

static bool mtp_upload_flush(void)
{
  if(!mtp_up.chunk_ready) {
    return true;
  }
  mtp_up.chunk_ready = false;

  if(!mtp_up.file_open) {
    if(f_open(&mtp_up.fil, mtp_up.path, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) {
      mtp_up.write_error = true;
      return false;
    }
    mtp_up.file_open = true;
  }

  if(mtp_up.chunk_len == 0u || mtp_up.write_error) {
    return !mtp_up.write_error;
  }

  UINT bw = 0;
  const FRESULT fr = f_write(&mtp_up.fil, mtp_up.chunk, mtp_up.chunk_len, &bw);
  if(fr != FR_OK || bw != mtp_up.chunk_len) {
    mtp_up.write_error = true;
    return false;
  }
  mtp_up.bytes_written += bw;
  return true;
}

static void mtp_upload_finish(bool ok)
{
  fs_lock();
  (void)mtp_upload_flush();
  mtp_obj_t* obj = mtp_obj_from_handle(send_obj_handle);
  if(mtp_up.file_open) {
    if(f_sync(&mtp_up.fil) != FR_OK) {
      ok = false;
    }
    f_close(&mtp_up.fil);
    mtp_up.file_open = false;
  }

  if(ok && obj != NULL) {
    FILINFO info;
    if(f_stat(obj->path, &info) == FR_OK) {
      obj->size = info.fsize;
    }
    if(mtp_up.file_size > 0u && mtp_up.bytes_written != mtp_up.file_size) {
      ok = false;
    }
  }

  if(!ok && mtp_up.path[0] != '\0') {
    (void)f_unlink(mtp_up.path);
    if(obj != NULL) {
      obj->used = false;
      send_obj_handle = 0;
    }
  }

  mtp_up.active = false;
  fs_unlock();
}

void mtp_upload_poll(void)
{
  if(!mtp_up.active) {
    return;
  }

  if(mtp_up.chunk_ready) {
    fs_lock();
    (void)mtp_upload_flush();
    fs_unlock();
  }

  if(mtp_up.need_receive && !mtp_up.write_error) {
    mtp_up.need_receive = false;
    (void)tud_mtp_data_receive(&mtp_up_rx_io);
  }
}

static void mtp_xfer_close_all(void)
{
  fs_lock();
  if(dl_open) {
    f_close(&dl_fil);
    dl_open = false;
  }
  mtp_upload_reset();
  fs_unlock();
}

static void mtp_obj_clear_all(void)
{
  memset(mtp_objects, 0, sizeof(mtp_objects));
  send_obj_handle = 0;
  ul_objinfo_parsed = false;
  mtp_upload_reset();
}

bool fs_mtp_transfer_busy(void)
{
  return mtp_up.active || dl_open;
}

void mtp_host_disconnected(void)
{
  is_session_opened = false;
  ul_objinfo_parsed = false;
  fs_mtp_session_set(false);
  mtp_xfer_close_all();
  mtp_obj_clear_all();
}

static mtp_obj_t* mtp_obj_from_handle(uint32_t handle)
{
  if(handle == 0 || handle > FS_MAX_OBJECT_COUNT) {
    return NULL;
  }
  mtp_obj_t* obj = &mtp_objects[handle - 1u];
  return obj->used ? obj : NULL;
}

static uint32_t mtp_obj_handle_of(const mtp_obj_t* obj)
{
  return (uint32_t)(obj - mtp_objects) + 1u;
}

static mtp_obj_t* mtp_obj_find_by_path(const char* path)
{
  for(size_t i = 0; i < FS_MAX_OBJECT_COUNT; i++) {
    mtp_obj_t* obj = &mtp_objects[i];
    if(obj->used && strcmp(obj->path, path) == 0) {
      return obj;
    }
  }
  return NULL;
}

static mtp_obj_t* mtp_obj_alloc(void)
{
  for(size_t i = 0; i < FS_MAX_OBJECT_COUNT; i++) {
    mtp_obj_t* obj = &mtp_objects[i];
    if(!obj->used) {
      memset(obj, 0, sizeof(*obj));
      obj->used = true;
      return obj;
    }
  }
  return NULL;
}

static void mtp_path_leaf(const char* path, char* leaf, size_t leaf_len)
{
  const char* slash = strrchr(path, '/');
  if(slash != NULL && slash[1] != '\0') {
    strncpy(leaf, slash + 1, leaf_len);
  }
  else if(strcmp(path, "/") == 0) {
    strncpy(leaf, "", leaf_len);
  }
  else {
    strncpy(leaf, path, leaf_len);
  }
  leaf[leaf_len - 1u] = '\0';
}

static void mtp_ascii_to_utf16(const char* ascii, uint16_t* utf16, size_t max_chars)
{
  size_t i = 0;
  while(ascii[i] != '\0' && i + 1u < max_chars) {
    utf16[i] = (uint16_t)(uint8_t)ascii[i];
    i++;
  }
  utf16[i] = 0;
}

static void mtp_path_to_utf16(const char* path, uint16_t* utf16, size_t max_chars)
{
  char leaf[FF_MAX_LFN + 1];
  mtp_path_leaf(path, leaf, sizeof(leaf));
  mtp_ascii_to_utf16(leaf, utf16, max_chars);
}

static size_t mtp_utf16_to_ascii(const uint16_t* utf16, char* ascii, size_t max_len)
{
  size_t i = 0;
  while(utf16[i] != 0 && i + 1u < max_len) {
    ascii[i] = (char)(utf16[i] & 0xffu);
    i++;
  }
  ascii[i] = '\0';
  return i;
}

static uint32_t mtp_normalize_parent(uint32_t parent_handle)
{
  if(parent_handle == 0xFFFFFFFFu || parent_handle == 0u) {
    return 0u;
  }
  return parent_handle;
}

static uint32_t mtp_parent_to_external(uint32_t parent_handle)
{
  return (parent_handle == 0u) ? 0xFFFFFFFFu : parent_handle;
}

static bool mtp_parent_dir_path(uint32_t parent_handle, char* path, size_t path_len)
{
  parent_handle = mtp_normalize_parent(parent_handle);
  if(parent_handle == 0u) {
    strncpy(path, "/", path_len);
    path[path_len - 1u] = '\0';
    return true;
  }

  mtp_obj_t* parent = mtp_obj_from_handle(parent_handle);
  if(parent == NULL || !parent->is_dir) {
    return false;
  }
  strncpy(path, parent->path, path_len);
  path[path_len - 1u] = '\0';
  return true;
}

static void mtp_join_path(const char* parent_path, const char* name, char* out, size_t out_len)
{
  if(strcmp(parent_path, "/") == 0) {
    snprintf(out, out_len, "/%s", name);
  }
  else {
    snprintf(out, out_len, "%s/%s", parent_path, name);
  }
}

static uint16_t mtp_format_from_name(const char* name, bool is_dir)
{
  if(is_dir) {
    return MTP_OBJ_FORMAT_ASSOCIATION;
  }

  const char* dot = strrchr(name, '.');
  if(dot == NULL) {
    return MTP_OBJ_FORMAT_UNDEFINED;
  }

  dot++;
  if(strcasecmp(dot, "txt") == 0) {
    return MTP_OBJ_FORMAT_TEXT;
  }
  if(strcasecmp(dot, "png") == 0) {
    return MTP_OBJ_FORMAT_PNG;
  }
  if(strcasecmp(dot, "jpg") == 0 || strcasecmp(dot, "jpeg") == 0) {
    return MTP_OBJ_FORMAT_EXIF_JPEG;
  }
  if(strcasecmp(dot, "wav") == 0) {
    return MTP_OBJ_FORMAT_WAV;
  }
  if(strcasecmp(dot, "mp3") == 0) {
    return MTP_OBJ_FORMAT_MP3;
  }
  if(strcasecmp(dot, "flac") == 0) {
    return MTP_OBJ_FORMAT_FLAC;
  }
  if(strcasecmp(dot, "aac") == 0 || strcasecmp(dot, "m4a") == 0) {
    return MTP_OBJ_FORMAT_AAC;
  }
  return MTP_OBJ_FORMAT_UNDEFINED;
}

static void mtp_obj_fill_from_finfo(mtp_obj_t* obj, uint32_t parent_handle,
  const char* parent_path, const FILINFO* info)
{
  mtp_join_path(parent_path, info->fname, obj->path, sizeof(obj->path));
  obj->parent_handle = mtp_normalize_parent(parent_handle);
  obj->is_dir = (info->fattrib & AM_DIR) != 0;
  obj->size = obj->is_dir ? 0u : info->fsize;
  obj->object_format = mtp_format_from_name(info->fname, obj->is_dir);
  obj->protection_status = (info->fattrib & AM_RDO) ?
    MTP_PROTECTION_STATUS_READ_ONLY : MTP_PROTECTION_STATUS_NO_PROTECTION;
}

static FRESULT mtp_scan_directory(uint32_t parent_handle)
{
  fs_lock();
  char dir_path[FS_MAX_PATH_LEN];
  if(!mtp_parent_dir_path(parent_handle, dir_path, sizeof(dir_path))) {
    fs_unlock();
    return FR_INVALID_OBJECT;
  }

  DIR dir;
  FILINFO info;
  FRESULT res = f_opendir(&dir, dir_path);
  if(res != FR_OK) {
    fs_unlock();
    return res;
  }

  parent_handle = mtp_normalize_parent(parent_handle);
  while((res = f_readdir(&dir, &info)) == FR_OK && info.fname[0] != '\0') {
    if(info.fname[0] == '.' &&
      (info.fname[1] == '\0' || (info.fname[1] == '.' && info.fname[2] == '\0'))) {
      continue;
    }

    char child_path[FS_MAX_PATH_LEN];
    mtp_join_path(dir_path, info.fname, child_path, sizeof(child_path));

    mtp_obj_t* obj = mtp_obj_find_by_path(child_path);
    if(obj == NULL) {
      obj = mtp_obj_alloc();
      if(obj == NULL) {
        f_closedir(&dir);
        fs_unlock();
        return FR_TOO_MANY_OPEN_FILES;
      }
    }

    mtp_obj_fill_from_finfo(obj, parent_handle, dir_path, &info);
  }

  f_closedir(&dir);
  fs_unlock();
  return res;
}

static uint32_t mtp_count_objects(void)
{
  uint32_t count = 0;
  for(size_t i = 0; i < FS_MAX_OBJECT_COUNT; i++) {
    if(mtp_objects[i].used) {
      count++;
    }
  }
  return count;
}

static bool mtp_update_storage_info(void)
{
  DWORD fre_clst = 0;
  FATFS* fs = NULL;
  if(fs_getfree_safe("0:", &fre_clst, &fs) != FR_OK || fs == NULL) {
    return false;
  }

  const uint32_t bytes_per_clst = (uint32_t)fs->csize * FF_MAX_SS;
  storage_info.max_capacity_in_bytes =
    (uint32_t)(fs->n_fatent - 2) * bytes_per_clst;
  storage_info.free_space_in_bytes = (uint32_t)fre_clst * bytes_per_clst;
  storage_info.free_space_in_objects = FS_MAX_OBJECT_COUNT - mtp_count_objects();
  return true;
}

//--------------------------------------------------------------------+
// Control Request callback
//--------------------------------------------------------------------+
bool tud_mtp_request_cancel_cb(tud_mtp_request_cb_data_t* cb_data)
{
  mtp_request_reset_cancel_data_t cancel_data;
  memcpy(&cancel_data, cb_data->buf, sizeof(cancel_data));
  (void)cancel_data.code;
  (void)cancel_data.transaction_id;
  mtp_xfer_close_all();
  return true;
}

bool tud_mtp_request_device_reset_cb(tud_mtp_request_cb_data_t* cb_data)
{
  (void)cb_data;
  mtp_xfer_close_all();
  return true;
}

int32_t tud_mtp_request_get_extended_event_cb(tud_mtp_request_cb_data_t* cb_data)
{
  (void)cb_data;
  return false;
}

int32_t tud_mtp_request_get_device_status_cb(tud_mtp_request_cb_data_t* cb_data)
{
  uint16_t* buf16 = (uint16_t*)(uintptr_t)cb_data->buf;
  buf16[0] = 4;
  buf16[1] = MTP_RESP_OK;
  return 4;
}

//--------------------------------------------------------------------+
// Bulk Only Protocol
//--------------------------------------------------------------------+
static int32_t mtp_dispatch_handler(tud_mtp_cb_data_t* cb_data)
{
  const mtp_container_command_t* command = cb_data->command_container;
  mtp_container_info_t* io_container = &cb_data->io_container;
  fs_op_handler_t handler = NULL;

  for(size_t i = 0; i < TU_ARRAY_SIZE(fs_op_handler_dict); i++) {
    if(fs_op_handler_dict[i].op_code == command->header.code) {
      handler = fs_op_handler_dict[i].handler;
      break;
    }
  }

  if(handler == NULL) {
    return MTP_RESP_OPERATION_NOT_SUPPORTED;
  }

  const int32_t resp_code = handler(cb_data);
  if(resp_code > MTP_RESP_UNDEFINED && cb_data->phase != MTP_PHASE_DATA) {
    io_container->header->code = (uint16_t)resp_code;
    tud_mtp_response_send(io_container);
  }
  return resp_code;
}

int32_t tud_mtp_command_received_cb(tud_mtp_cb_data_t* cb_data)
{
  return mtp_dispatch_handler(cb_data);
}

int32_t tud_mtp_data_xfer_cb(tud_mtp_cb_data_t* cb_data)
{
  mtp_dispatch_handler(cb_data);
  if(cb_data->phase == MTP_PHASE_DATA
    && cb_data->command_container->header.code == MTP_OP_SEND_OBJECT) {
    mtp_upload_poll();
  }
  return 0;
}

int32_t tud_mtp_data_complete_cb(tud_mtp_cb_data_t* cb_data)
{
  const mtp_container_command_t* command = cb_data->command_container;
  mtp_container_info_t* resp = &cb_data->io_container;

  switch(command->header.code) {
    case MTP_OP_SEND_OBJECT_INFO: {
      mtp_obj_t* obj = mtp_obj_from_handle(send_obj_handle);
      if(obj == NULL) {
        resp->header->code = MTP_RESP_GENERAL_ERROR;
        break;
      }
      (void)mtp_container_add_uint32(resp, SUPPORTED_STORAGE_ID);
      (void)mtp_container_add_uint32(resp, mtp_parent_to_external(obj->parent_handle));
      (void)mtp_container_add_uint32(resp, send_obj_handle);
      resp->header->code = MTP_RESP_OK;
      break;
    }

    case MTP_OP_SEND_OBJECT: {
      const bool xfer_ok = (cb_data->xfer_result == XFER_RESULT_SUCCESS);
      const bool write_ok = xfer_ok && mtp_up.active && !mtp_up.write_error;
      mtp_upload_finish(write_ok);
      resp->header->code = write_ok ? MTP_RESP_OK : MTP_RESP_GENERAL_ERROR;
      break;
    }

    case MTP_OP_GET_OBJECT:
    case MTP_OP_GET_PARTIAL_OBJECT:
      fs_lock();
      if(dl_open) {
        f_close(&dl_fil);
        dl_open = false;
      }
      fs_unlock();
      if(command->header.code == MTP_OP_GET_PARTIAL_OBJECT) {
        const uint32_t len = cb_data->total_xferred_bytes
          - sizeof(mtp_container_header_t);
        (void)mtp_container_add_uint32(resp, len);
      }
      resp->header->code =
        (cb_data->xfer_result == XFER_RESULT_SUCCESS) ?
          MTP_RESP_OK : MTP_RESP_GENERAL_ERROR;
      break;

    default:
      resp->header->code =
        (cb_data->xfer_result == XFER_RESULT_SUCCESS) ?
          MTP_RESP_OK : MTP_RESP_GENERAL_ERROR;
      break;
  }

  tud_mtp_response_send(resp);
  return 0;
}

int32_t tud_mtp_response_complete_cb(tud_mtp_cb_data_t* cb_data)
{
  (void)cb_data;
  return 0;
}

size_t board_usb_get_serial(uint16_t desc_str1[], size_t max_chars);

//--------------------------------------------------------------------+
// File System Handlers
//--------------------------------------------------------------------+
static int32_t fs_get_device_info(tud_mtp_cb_data_t* cb_data)
{
  int32_t resp_code = 0;
  mtp_container_info_t* io_container = &cb_data->io_container;
  (void)mtp_container_add_cstring(io_container, DEV_INFO_MANUFACTURER);
  (void)mtp_container_add_cstring(io_container, DEV_INFO_MODEL);
  (void)mtp_container_add_cstring(io_container, DEV_INFO_VERSION);

  enum { MAX_SERIAL_NCHARS = 32 };
  uint16_t serial_utf16[MAX_SERIAL_NCHARS + 1];
  size_t nchars = board_usb_get_serial(serial_utf16, MAX_SERIAL_NCHARS);
  serial_utf16[tu_min32(nchars, MAX_SERIAL_NCHARS)] = 0;
  (void)mtp_container_add_string(io_container, serial_utf16);

  if(!tud_mtp_data_send(io_container)) {
    resp_code = MTP_RESP_DEVICE_BUSY;
  }
  return resp_code;
}

static int32_t fs_open_close_session(tud_mtp_cb_data_t* cb_data)
{
  const mtp_container_command_t* command = cb_data->command_container;
  if(command->header.code == MTP_OP_OPEN_SESSION) {
    if(is_session_opened) {
      return MTP_RESP_SESSION_ALREADY_OPEN;
    }
    fs_playback_suspend();
    mtp_xfer_close_all();
    mtp_obj_clear_all();
    is_session_opened = true;
    fs_mtp_session_set(true);
  }
  else {
    if(!is_session_opened) {
      return MTP_RESP_SESSION_NOT_OPEN;
    }
    mtp_xfer_close_all();
    mtp_obj_clear_all();
    is_session_opened = false;
    fs_mtp_session_set(false);
  }
  return MTP_RESP_OK;
}

static int32_t fs_get_storage_ids(tud_mtp_cb_data_t* cb_data)
{
  mtp_container_info_t* io_container = &cb_data->io_container;
  uint32_t storage_ids[] = {SUPPORTED_STORAGE_ID};
  (void)mtp_container_add_auint32(io_container, 1, storage_ids);
  tud_mtp_data_send(io_container);
  return 0;
}

static int32_t fs_get_storage_info(tud_mtp_cb_data_t* cb_data)
{
  const mtp_container_command_t* command = cb_data->command_container;
  mtp_container_info_t* io_container = &cb_data->io_container;
  const uint32_t storage_id = command->params[0];
  TU_VERIFY(SUPPORTED_STORAGE_ID == storage_id, -1);

  if(!mtp_update_storage_info()) {
    return MTP_RESP_GENERAL_ERROR;
  }

  (void)mtp_container_add_raw(io_container, &storage_info, sizeof(storage_info));
  tud_mtp_data_send(io_container);
  return 0;
}

static int32_t fs_get_device_properties(tud_mtp_cb_data_t* cb_data)
{
  const mtp_container_command_t* command = cb_data->command_container;
  mtp_container_info_t* io_container = &cb_data->io_container;
  const uint16_t dev_prop_code = (uint16_t)command->params[0];

  if(command->header.code == MTP_OP_GET_DEVICE_PROP_DESC) {
    mtp_device_prop_desc_header_t device_prop_header;
    device_prop_header.device_property_code = dev_prop_code;
    switch(dev_prop_code) {
      case MTP_DEV_PROP_DEVICE_FRIENDLY_NAME:
        device_prop_header.datatype = MTP_DATA_TYPE_STR;
        device_prop_header.get_set = MTP_MODE_GET;
        (void)mtp_container_add_raw(io_container, &device_prop_header,
          sizeof(device_prop_header));
        (void)mtp_container_add_cstring(io_container, DEV_PROP_FRIENDLY_NAME);
        (void)mtp_container_add_cstring(io_container, DEV_PROP_FRIENDLY_NAME);
        (void)mtp_container_add_uint8(io_container, 0);
        tud_mtp_data_send(io_container);
        break;

      default:
        return MTP_RESP_PARAMETER_NOT_SUPPORTED;
    }
  }
  else {
    switch(dev_prop_code) {
      case MTP_DEV_PROP_DEVICE_FRIENDLY_NAME:
        (void)mtp_container_add_cstring(io_container, DEV_PROP_FRIENDLY_NAME);
        tud_mtp_data_send(io_container);
        break;

      default:
        return MTP_RESP_PARAMETER_NOT_SUPPORTED;
    }
  }
  return 0;
}

static int32_t fs_get_object_handles(tud_mtp_cb_data_t* cb_data)
{
  const mtp_container_command_t* command = cb_data->command_container;
  mtp_container_info_t* io_container = &cb_data->io_container;

  const uint32_t storage_id = command->params[0];
  const uint32_t obj_format = command->params[1];
  const uint32_t parent_handle = command->params[2];
  (void)obj_format;

  if(storage_id != 0xFFFFFFFFu && storage_id != SUPPORTED_STORAGE_ID) {
    return MTP_RESP_INVALID_STORAGE_ID;
  }

  const uint32_t parent = mtp_normalize_parent(parent_handle);
  if(parent != 0u && mtp_obj_from_handle(parent) == NULL) {
    return MTP_RESP_INVALID_OBJECT_HANDLE;
  }

  if(mtp_scan_directory(parent_handle) != FR_OK) {
    return MTP_RESP_GENERAL_ERROR;
  }

  uint32_t handles[FS_MAX_OBJECT_COUNT] = {0};
  uint32_t count = 0u;
  for(size_t i = 0; i < FS_MAX_OBJECT_COUNT; i++) {
    mtp_obj_t* obj = &mtp_objects[i];
    if(!obj->used) {
      continue;
    }
    if(obj->parent_handle == parent) {
      if(obj_format == 0u || obj_format == 0xFFFFFFFFu
        || obj->object_format == (uint16_t)obj_format) {
        handles[count++] = (uint32_t)i + 1u;
      }
    }
  }

  (void)mtp_container_add_auint32(io_container, count, handles);
  tud_mtp_data_send(io_container);
  return 0;
}

static int32_t fs_get_object_info(tud_mtp_cb_data_t* cb_data)
{
  const mtp_container_command_t* command = cb_data->command_container;
  mtp_container_info_t* io_container = &cb_data->io_container;
  const uint32_t obj_handle = command->params[0];
  mtp_obj_t* obj = mtp_obj_from_handle(obj_handle);
  if(obj == NULL) {
    return MTP_RESP_INVALID_OBJECT_HANDLE;
  }

  mtp_object_info_header_t obj_info_header = {
    .storage_id = SUPPORTED_STORAGE_ID,
    .object_format = obj->object_format,
    .protection_status = obj->protection_status,
    .object_compressed_size = obj->size,
    .thumb_format = MTP_OBJ_FORMAT_UNDEFINED,
    .thumb_compressed_size = 0,
    .thumb_pix_width = 0,
    .thumb_pix_height = 0,
    .image_pix_width = 0,
    .image_pix_height = 0,
    .image_bit_depth = 0,
    .parent_object = mtp_parent_to_external(obj->parent_handle),
    .association_desc = obj->is_dir ? (uint16_t)MTP_ASSOCIATION_GENERIC_FOLDER : 0u,
    .sequence_number = 0,
  };

  uint16_t name_utf16[FF_MAX_LFN + 1];
  mtp_path_to_utf16(obj->path, name_utf16, FF_MAX_LFN + 1);
  (void)mtp_container_add_raw(io_container, &obj_info_header, sizeof(obj_info_header));
  (void)mtp_container_add_string(io_container, name_utf16);
  (void)mtp_container_add_cstring(io_container, FS_FIXED_DATETIME);
  (void)mtp_container_add_cstring(io_container, FS_FIXED_DATETIME);
  (void)mtp_container_add_cstring(io_container, "");
  tud_mtp_data_send(io_container);
  return 0;
}

static int32_t fs_read_object_chunk(mtp_obj_t* obj, mtp_container_info_t* io_container,
  uint32_t file_offset, uint32_t bytes_to_send, tud_mtp_cb_data_t* cb_data)
{
  if(obj->is_dir) {
    return MTP_RESP_INVALID_OBJECT_HANDLE;
  }

  if(cb_data->phase == MTP_PHASE_COMMAND) {
    fs_lock();
    if(dl_open) {
      f_close(&dl_fil);
      dl_open = false;
    }
    if(f_open(&dl_fil, obj->path, FA_READ) != FR_OK) {
      fs_unlock();
      return MTP_RESP_GENERAL_ERROR;
    }
    dl_open = true;

    if(file_offset > 0u) {
      if(f_lseek(&dl_fil, file_offset) != FR_OK) {
        f_close(&dl_fil);
        dl_open = false;
        fs_unlock();
        return MTP_RESP_GENERAL_ERROR;
      }
    }

    uint8_t* buf = mtp_container_payload_ptr(io_container);
    const uint32_t chunk = tu_min32(bytes_to_send, io_container->payload_bytes);
    UINT br = 0;
    if(chunk > 0u) {
      if(f_read(&dl_fil, buf, chunk, &br) != FR_OK) {
        f_close(&dl_fil);
        dl_open = false;
        fs_unlock();
        return MTP_RESP_GENERAL_ERROR;
      }
    }
    // len 必须设为完整传输长度，TinyUSB 据此决定后续 DATA 阶段
    (void)mtp_container_add_raw(io_container, buf, bytes_to_send);
    fs_unlock();
    tud_mtp_data_send(io_container);
  }
  else if(cb_data->phase == MTP_PHASE_DATA) {
    const uint32_t offset = cb_data->total_xferred_bytes
      - sizeof(mtp_container_header_t);
    const uint32_t xact_len = tu_min32(bytes_to_send - offset, io_container->payload_bytes);
    if(xact_len > 0u) {
      UINT br = 0;
      fs_lock();
      if(f_read(&dl_fil, io_container->payload, xact_len, &br) != FR_OK || br == 0u) {
        fs_unlock();
        return MTP_RESP_GENERAL_ERROR;
      }
      fs_unlock();
      tud_mtp_data_send(io_container);
    }
  }

  return 0;
}

static int32_t fs_get_object(tud_mtp_cb_data_t* cb_data)
{
  const mtp_container_command_t* command = cb_data->command_container;
  mtp_container_info_t* io_container = &cb_data->io_container;
  const uint32_t obj_handle = command->params[0];
  mtp_obj_t* obj = mtp_obj_from_handle(obj_handle);
  if(obj == NULL) {
    return MTP_RESP_INVALID_OBJECT_HANDLE;
  }

  return fs_read_object_chunk(obj, io_container, 0u, obj->size, cb_data);
}

static int32_t fs_get_partial_object(tud_mtp_cb_data_t* cb_data)
{
  const mtp_container_command_t* command = cb_data->command_container;
  mtp_container_info_t* io_container = &cb_data->io_container;
  const uint32_t obj_handle = command->params[0];
  const uint32_t req_offset = command->params[1];
  const uint32_t req_max = command->params[2];
  mtp_obj_t* obj = mtp_obj_from_handle(obj_handle);
  if(obj == NULL) {
    return MTP_RESP_INVALID_OBJECT_HANDLE;
  }

  const uint32_t avail = (req_offset >= obj->size) ? 0u : (obj->size - req_offset);
  const uint32_t to_send = tu_min32(avail, req_max);
  return fs_read_object_chunk(obj, io_container, req_offset, to_send, cb_data);
}

static int32_t fs_delete_object(tud_mtp_cb_data_t* cb_data)
{
  const mtp_container_command_t* command = cb_data->command_container;
  const uint32_t obj_handle = command->params[0];
  const uint32_t obj_format = command->params[1];
  (void)obj_format;

  if(!is_session_opened) {
    return MTP_RESP_SESSION_NOT_OPEN;
  }

  mtp_obj_t* obj = mtp_obj_from_handle(obj_handle);
  if(obj == NULL) {
    return MTP_RESP_INVALID_OBJECT_HANDLE;
  }

  fs_lock();
  if(f_unlink(obj->path) != FR_OK) {
    fs_unlock();
    return MTP_RESP_GENERAL_ERROR;
  }
  fs_unlock();

  obj->used = false;
  return MTP_RESP_OK;
}

static int32_t fs_send_object_info(tud_mtp_cb_data_t* cb_data)
{
  const mtp_container_command_t* command = cb_data->command_container;
  mtp_container_info_t* io_container = &cb_data->io_container;
  const uint32_t storage_id = command->params[0];
  const uint32_t cmd_parent_handle = command->params[1];

  if(!is_session_opened) {
    return MTP_RESP_SESSION_NOT_OPEN;
  }
  if(storage_id != 0xFFFFFFFFu && storage_id != SUPPORTED_STORAGE_ID) {
    return MTP_RESP_INVALID_STORAGE_ID;
  }

  if(cb_data->phase == MTP_PHASE_COMMAND) {
    ul_objinfo_parsed = false;
    (void)tud_mtp_data_receive(io_container);
  }
  else if(cb_data->phase == MTP_PHASE_DATA && !ul_objinfo_parsed) {
    ul_objinfo_parsed = true;

    mtp_object_info_header_t* obj_info =
      (mtp_object_info_header_t*)io_container->payload;
    if(obj_info->storage_id != 0 && obj_info->storage_id != SUPPORTED_STORAGE_ID) {
      return MTP_RESP_INVALID_STORAGE_ID;
    }

    uint32_t parent = mtp_normalize_parent(cmd_parent_handle);
    if(parent == 0u && obj_info->parent_object != 0u
      && obj_info->parent_object != 0xFFFFFFFFu) {
      parent = mtp_normalize_parent(obj_info->parent_object);
    }

    if(parent != 0u) {
      mtp_obj_t* parent_obj = mtp_obj_from_handle(parent);
      if(parent_obj == NULL || !parent_obj->is_dir) {
        return MTP_RESP_INVALID_PARENT_OBJECT;
      }
    }

    char parent_path[FS_MAX_PATH_LEN];
    if(!mtp_parent_dir_path(parent, parent_path, sizeof(parent_path))) {
      return MTP_RESP_INVALID_PARENT_OBJECT;
    }

    if(parent != 0u) {
      FILINFO parent_info;
      fs_lock();
      const FRESULT parent_res = f_stat(parent_path, &parent_info);
      fs_unlock();
      if(parent_res != FR_OK || (parent_info.fattrib & AM_DIR) == 0) {
        return MTP_RESP_INVALID_PARENT_OBJECT;
      }
    }

    char leaf_name[FF_MAX_LFN + 1];
    uint16_t leaf_utf16[FF_MAX_LFN + 1];
    uint8_t* buf = io_container->payload + sizeof(mtp_object_info_header_t);
    (void)mtp_container_get_string(buf, leaf_utf16);
    mtp_utf16_to_ascii(leaf_utf16, leaf_name, sizeof(leaf_name));

    mtp_obj_t* obj = mtp_obj_alloc();
    if(obj == NULL) {
      return MTP_RESP_STORE_FULL;
    }

    send_obj_handle = mtp_obj_handle_of(obj);
    mtp_join_path(parent_path, leaf_name, obj->path, sizeof(obj->path));
    obj->parent_handle = parent;
    obj->size = obj_info->object_compressed_size;
    obj->object_format = obj_info->object_format;
    obj->protection_status = obj_info->protection_status;
    obj->is_dir = (obj_info->object_format == MTP_OBJ_FORMAT_ASSOCIATION);

    if(obj->is_dir) {
      FILINFO stat;
      fs_lock();
      if(f_mkdir(obj->path) != FR_OK && f_stat(obj->path, &stat) != FR_OK) {
        fs_unlock();
        obj->used = false;
        send_obj_handle = 0;
        return MTP_RESP_GENERAL_ERROR;
      }
      fs_unlock();
    }
  }

  return 0;
}

static int32_t fs_send_object(tud_mtp_cb_data_t* cb_data)
{
  mtp_container_info_t* io_container = &cb_data->io_container;
  mtp_obj_t* obj = mtp_obj_from_handle(send_obj_handle);
  if(obj == NULL) {
    return MTP_RESP_INVALID_OBJECT_HANDLE;
  }
  if(obj->is_dir) {
    return MTP_RESP_OK;
  }

  if(cb_data->phase == MTP_PHASE_COMMAND) {
    mtp_upload_reset();
    mtp_up.active = true;
    strncpy(mtp_up.path, obj->path, sizeof(mtp_up.path));
    mtp_up.path[sizeof(mtp_up.path) - 1u] = '\0';
    mtp_up.file_size = obj->size;

    mtp_up_rx_hdr.len = (uint32_t)sizeof(mtp_container_header_t) + obj->size;
    mtp_up_rx_io.header = &mtp_up_rx_hdr;
    (void)tud_mtp_data_receive(&mtp_up_rx_io);
  }
  else {
    if(!mtp_up.active) {
      return MTP_RESP_GENERAL_ERROR;
    }

    if(mtp_up.file_size == 0u
      && io_container->header->len > (uint32_t)sizeof(mtp_container_header_t)) {
      mtp_up.file_size = io_container->header->len
        - (uint32_t)sizeof(mtp_container_header_t);
      obj->size = mtp_up.file_size;
      mtp_up_rx_hdr.len = io_container->header->len;
    }

    if(io_container->payload_bytes > 0u) {
      memcpy(mtp_up.chunk, io_container->payload, io_container->payload_bytes);
      mtp_up.chunk_len = (uint16_t)io_container->payload_bytes;
      mtp_up.chunk_ready = true;
    }

    const uint32_t payload_xferred = cb_data->total_xferred_bytes
      - (uint32_t)sizeof(mtp_container_header_t);
    mtp_up.need_receive = !mtp_up.write_error && payload_xferred < mtp_up.file_size;
  }

  return 0;
}
