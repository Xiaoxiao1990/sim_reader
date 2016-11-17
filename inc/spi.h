#ifndef SPI_H
#define SPI_H

/******************* spi.c ************************/
#define SPI_TRANSFER_MTU                                (uint16_t)300
//SPI Buffer state
#define SPI_BUF_STATE_EMPTY                             (uint8_t)0
#define SPI_BUF_STATE_PACKAGING                         (uint8_t)1
#define SPI_BUF_STATE_READY                             (uint8_t)2
#define SPI_BUF_STATE_FULL                              (uint8_t)3
#define SPI_BUF_STATE_TRANSMITING                       (uint8_t)4
//SIM_BIT & No.
#define SIM_NO_1_BIT                                    (uint8_t)0x01
#define SIM_NO_2_BIT                                    (uint8_t)0x02
#define SIM_NO_3_BIT                                    (uint8_t)0x04
#define SIM_NO_4_BIT                                    (uint8_t)0x08
#define SIM_NO_5_BIT                                    (uint8_t)0x10
#define SIM_NO_ALL_BIT                                  (uint8_t)0x20
//
#define SIM_NO_ALL                                      (uint8_t)0x00
#define SIM_NO_1                                        (uint8_t)0x01
#define SIM_NO_2                                        (uint8_t)0x02
#define SIM_NO_3                                        (uint8_t)0x03
#define SIM_NO_4                                        (uint8_t)0x04
#define SIM_NO_5                                        (uint8_t)0x05

typedef struct SPI_Buf{
    uint8_t Buf[SPI_TRANSFER_MTU];
    uint16_t Length;
    uint16_t Buf_Len_Left;
    uint8_t state;
    uint8_t block;
    uint8_t pre_sum;
    uint8_t checksum;
}SPI_Buf_TypeDef;

void _SPI_Dev_Init(int argc, char *argv[]);
void _SPI_Buf_init(SPI_Buf_TypeDef *buf);

#endif // SPI_H
