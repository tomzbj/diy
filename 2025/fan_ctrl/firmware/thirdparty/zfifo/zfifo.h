/* Author: tomzbj  Updated: 2024-04-13  Rev: 0.01 */

#ifndef _FIFO_H
#define _FIFO_H

#include <cstdint>
#include <cstring>

typedef size_t zfifo_size_t;

class ZFIFO {
public:
    ZFIFO(void* workbuf, size_t size)
    {
        _buffer = (uint8_t*)workbuf;
        if((size & (size - 1)) != 0)
            size = 0x80000000 >> (__builtin_clz(size) - 1);
        _size = size;
        _in = _out = 0;
    }
    uint8_t peek(void)
    {
        return *(uint8_t*)(_buffer + (_out & (_size - 1)));
    }
    void traverse(int (*printf_f)(const char* args, ...))
    {
        zfifo_size_t size = _in - _out;
//        for(size_t pos = (_out & (_size - 1)); pos != (_in & (_size - 1)); pos++) {
        for(size_t i = 0; i < size; i++) {
            printf_f("%c", _buffer[((_out + i) & (_size - 1))]);
        }
        printf_f("\n");
        printf_f("in: %d out: %d free: %d\n", _in, _out, _size - _in + _out);
    }

    int put(const void* pdata, size_t len)    //向kfifo中添加数t
    {
        zfifo_size_t l;
        len = min(len, _size - _in + _out);    //buffer中空的长度
//        smp_mb(); /* * Ensure that we sample the _out index -before- we * start putting bytes into the kfifo. */
        l = min(len, _size - (_in & (_size - 1))); /* first put the data starting from _in to buffer end */
        memcpy(_buffer + (_in & (_size - 1)), pdata, l);
        memcpy(_buffer, (uint8_t*)pdata + l, len - l); /* then put the rest (if any) at the beginning of the buffer */
//        smp_wmb(); /* * Ensure that we add the bytes to the kfifo -before- * we update the _in index. */
        _in += len;    //每次累加，到达最大值后溢出，自动转为0

        return len;
    }
    int get(void* pdata, size_t len)    //从kfifo中取数据
    {
        zfifo_size_t l;
        len = min(len, _in - _out);    //有数据的缓冲区的长度
//        smp_rmb(); /* * Ensure that we sample the _in index -before- we * start removing bytes from the kfifo. */
        l = min(len, _size - (_out & (_size - 1))); /* first get the data from _out until the end of the buffer */
        memcpy(pdata, (uint8_t*)_buffer + (_out & (_size - 1)), l);
        memcpy((uint8_t*)pdata + l, _buffer, len - l); /* then get the rest (if any) from the beginning of the buffer */
//        smp_mb(); /* * Ensure that we remove the bytes from the kfifo -before- * we update the _out index. */
        _out += len;    //每次累加，到达最大值后溢出，自动转为0
        return len;
    }
    int len(void)    //获取kfifo中有数据的buffer大小
    {
        return _in - _out;
    }

private:
    uint8_t* _buffer;
    zfifo_size_t _size, _in, _out;

    template<typename T>
    inline T min(T x, T y)
    {
        return (x <= y) ? x : y;
    }
};

#endif
