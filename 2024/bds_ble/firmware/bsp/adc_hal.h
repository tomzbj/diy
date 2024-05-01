#ifndef _ADC_HAL_H
#define _ADC_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

class CADC {
public:
    CADC(void)
    {
    }
    void init(void);
    uint16_t read(void)
    {
        return ADC1->DR;
    }
private:
    ADC_HandleTypeDef _hadc;
};

#ifdef __cplusplus
}
#endif

#endif
