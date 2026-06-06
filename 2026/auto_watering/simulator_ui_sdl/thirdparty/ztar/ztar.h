/* Author: tomzbj  Updated: 2024-04-18 Rev01 */

#ifndef _ZTAR_H
#define _ZTAR_H

#include <cstdint>
#include <cstring>
#include <string>

class ZTAR {
  public:
    typedef struct {
        int (*read_f)(const void* addr, uint32_t size, void* buf);
    } cfg_t;

    ZTAR(const cfg_t& cfg) : cfg_(cfg)
    {
    }
    int find(const char* fname, const void* start_addr, uint32_t max_size,
      void** pofs, uint32_t* psize)
    {
      uint8_t* addr = (uint8_t*)start_addr;
      uint32_t size;
      while((uint32_t)addr < (uint32_t)start_addr + max_size) {
        header_t header;
        cfg_.read_f(addr, sizeof(header), &header);
        addr += 512;    //sizeof(header);
        if(strlen(header.name) == 0)
          return -1;      // file 'fname' not found
//        sscanf(header.size, "%lo", (unsigned long*)&size);
        size = std::stoi(std::string(header.size), 0, 8);
        if(strncmp(header.name, fname, sizeof(header.name)) == 0) {
          *pofs = addr;
          *psize = size;
          return 0;
        }
        if(size % 512 != 0)
          size += (512 - (size % 512));
        addr += size;
      }
      return -1;
    }
  private:
    typedef struct {
        char name[100];
        char reserved_1[24];
        char size[12];
    } header_t;

    const cfg_t& cfg_;
};

#endif
