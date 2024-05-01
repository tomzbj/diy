#ifndef _BDS_H
#define _BDS_H

class BDS {
public:
    bool sync = false;  // 是否已成功对时
    BDS(void)
    {
    }
    void init(void);
    void off(void)
    {
        ZPin::write(PA0, 1);
    }
    void on(void)
    {
        ZPin::write(PA0, 0);
    }
    void parse(const void* msg, int size);
private:
};

#endif
