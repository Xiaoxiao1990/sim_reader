#ifndef SIMCARD_H
#define SIMCARD_H

/******************* simcard.c ************************/
#define MCU_NUMS                                (uint8_t)108
#define SIM_NUMS                                (uint8_t)5              //

#define SIM_APDU_LENGTH                         (uint8_t)40
#define ICCID_LENGTH                            (uint8_t)0x0A
#define IMSI_LENGTH                             (uint8_t)0x08

typedef struct{
    uint8_t ICCID[ICCID_LENGTH];
    uint8_t IMSI[IMSI_LENGTH];
    uint8_t AD;
    uint8_t Tx_APDU[SIM_APDU_LENGTH];
    uint8_t Tx_Length;
    uint8_t Rx_APDU[SIM_APDU_LENGTH];
    uint8_t Rx_Length;
    uint8_t state;
}SIM_TypeDef;

typedef struct{
    SIM_TypeDef SIM[SIM_NUMS];
    SPI_Buf_TypeDef TxBuf;
    SPI_Buf_TypeDef RxBuf;
    uint8_t HardWare_Version[3];
    uint8_t SoftWare_Version[3];
    uint8_t SIM_StateTblR, SIM_StateTblW;
    uint8_t SIM_APDUTblR, SIM_APDUTblW;
    uint8_t SIM_InfoTblR, SIM_InfoTblW;
    uint8_t SIM_CheckErrR,SIM_CheckErrW;
    uint8_t SIM_ResetTbl,SIM_StopTbl;
    uint8_t VersionR,VersionW;
}MCU_TypeDef;
/******************* end ************************/
void _SIM_info_init(SIM_TypeDef *sim);
void _SIMs_Table_init(void);
int _apdu_onload(uint8_t *data,SIM_TypeDef *sim,uint8_t len);

#endif // SIMCARD_H
