#include <stdio.h>
#include <string.h>

//#include "regs.h"
#include "nrf24le1.h"

void Init(void)
{
    UART_InitTypeDef UART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
//    SPI_InitTypeDef SPI_InitStructure;
//    ADC_InitTypeDef ADC_InitStructure;
//    PWM_InitTypeDef PWM_InitStructure;
    RF_InitTypeDef RF_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutBuffer_Normal;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;
    GPIO_Init(GPIO0, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_InBuffer_PullUp;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIO0, &GPIO_InitStructure);

    UART_InitStructure.UART_Mode = UART_Mode_8bit;
    UART_InitStructure.UART_Rx = ENABLE;
    UART_InitStructure.UART_BaudRate = UART_BaudRate_500000;
    UART_Init(&UART_InitStructure);

    /*
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutBuffer_Normal;
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;
     GPIO_Init(GPIO0, &GPIO_InitStructure);
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_InBuffer_PullUp;
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
     GPIO_Init(GPIO0, &GPIO_InitStructure);

     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutBuffer_Normal;
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4
     | GPIO_Pin_5;
     GPIO_Init(GPIO1, &GPIO_InitStructure);
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_InBuffer_PullUp;
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_6;
     GPIO_Init(GPIO1, &GPIO_InitStructure);

     SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
     SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
     SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
     SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
     SPI_InitStructure.SPI_Master = ENABLE;
     SPI_Init(&SPI_InitStructure);
     */

    /*
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_InBuffer_PullDown;
     GPIO_Init(GPIO0, &GPIO_InitStructure);
     P0 &= ~_BV(0);
     ADC_InitStructure.ADC_Reference = ADC_Reference_VDD;
     ADC_InitStructure.ADC_Channel = ADC_Channel_1_3_VDD;
     ADC_InitStructure.ADC_InputAcquisitionTime = ADC_InputAcquisitionTime_36us;
     ADC_InitStructure.ADC_DataRate = ADC_DataRate_4k;
     ADC_InitStructure.ADC_ContinueousConvMode = DISABLE;
     ADC_InitStructure.ADC_DifferentialMode = ADC_DifferentialMode_Off;
     ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
     ADC_InitStructure.ADC_Resolution = ADC_Resolution_12bits;
     ADC_Init(&ADC_InitStructure);

     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OutBuffer_Normal;
     GPIO_Init(GPIO0, &GPIO_InitStructure);
     PWM_InitStructure.PWM_PeriodLength = PWM_PeriodLength_8bits;
     // PWM Freq = 16M / 255 / (0 + 1) = 62.7kHz
     PWM_InitStructure.PWM_FrequencyPrescaler = PWM_FrequencyPrescaler_0;
     PWM_InitStructure.PWM_Channel_0_State = ENABLE;
     PWM_InitStructure.PWM_Channel_1_State = DISABLE;
     PWM_Init(&PWM_InitStructure);
     PWM_SetDutyCycle(PWM_Channel_0, 80);
     */

    RF_InitStructure.RF_Mask_RXDR = DISABLE;
    RF_InitStructure.RF_Mask_TXDS = DISABLE;
    RF_InitStructure.RF_Mask_MAXRT = DISABLE;
    RF_InitStructure.RF_EN_CRC = ENABLE;
    RF_InitStructure.RF_CRC_Encoding = RF_CRC_Encoding_2byte;
    RF_InitStructure.RF_AutoAck_DataPipe = RF_AutoAck_DataPipe_0;
    RF_InitStructure.RF_RxAddr_DataPipe = RF_RxAddr_DataPipe_0;
    RF_InitStructure.RF_AddrWidth = RF_AddrWidth_5bytes;
    RF_InitStructure.RF_RetryDelay = RF_RetryDelay_250us;
    RF_InitStructure.RF_RetryCount = 15;
    RF_InitStructure.RF_Channel = 110;
    RF_InitStructure.RF_DataRate = RF_DataRate_2Mbps;
    RF_InitStructure.RF_Power = RF_Power_0dBm;
    RF_InitStructure.RF_RxAddr_P0 = "\xe7\xe7\xe7\xe7\xe7";
    RF_InitStructure.RF_RxAddr_P1 = "00001";
    RF_InitStructure.RF_RxAddr_P2LSB = '2';
    RF_InitStructure.RF_RxAddr_P3LSB = '3';
    RF_InitStructure.RF_RxAddr_P4LSB = '4';
    RF_InitStructure.RF_RxAddr_P5LSB = '5';
    RF_InitStructure.RF_TxAddr = "\xe7\xe7\xe7\xe7\xe7";
    RF_InitStructure.RF_RxPayloadWidth_P0 = 32;
    RF_InitStructure.RF_RxPayloadWidth_P1 = 32;
    RF_InitStructure.RF_RxPayloadWidth_P2 = 32;
    RF_InitStructure.RF_RxPayloadWidth_P3 = 32;
    RF_InitStructure.RF_RxPayloadWidth_P4 = 32;
    RF_InitStructure.RF_RxPayloadWidth_P5 = 32;
    RF_InitStructure.RF_DynamicPayloadWidth_DataPipe =
            RF_DynamicPayloadWidth_DataPipe_None;
    RF_InitStructure.RF_DynamicPayloadWidth = DISABLE;
    RF_InitStructure.RF_PayloadWithAck = DISABLE;
    RF_InitStructure.RF_TxPayloadNoAckCmd = DISABLE;

    RF_Init(&RF_InitStructure);
    RF_Cmd(ENABLE);

    INTERRUPT_EnableIRQ(GLOBAL_IRQn | UART_IRQn);
}

typedef enum {
    TX, RX
} rf_mode_t;

int main(void)
{
    static unsigned short n = 3, m = 2, k;
    rf_mode_t mode = TX;

    Init();

    _delay_ms(10);

    /*
    if(mode == TX) {
        while(1) {
            int ret;
            char tx_payload[32], buf[32];
            memset(tx_payload, '\0', 32);
            k = n * m;
            sprintf(tx_payload, "%d\n", k);
//            sprintf(tx_payload, "%ld * %ld = %ld", n, m, k);
//            sprintf(buf, "%04x * %04x = %04x", n, m, n * m);
//            strcat(tx_payload, buf);
            printf("%s\n", tx_payload);
            ret = RF_TX(tx_payload, 32);
            if(ret > 0) {
                printf("TX Succeed!\n");
            }
            else {
                printf("TX Failed!\n");
            }
            n++;
            m++;
            _delay_ms(250);
        }
    }
    else {
        while(1) {
//        ADC_StartConversion();
//        printf("%d\n", ADC_ReadData());
//        RF_ReadAllRegs();
            char buf[32];
            RF_RX(buf, sizeof(buf));
            printf("%s\n", buf);
        }
    }
//    return 0;
 *
 */
}

