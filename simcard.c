#include "inc/includes.h"
/********************************************************
 * Function:Initial the SIM cards.
 * Params:Buffer structure arrays.
 * *****************************************************/
#define SIM_STATE_NOSIM                         (uint8_t)0x00
#define SIM_STATE_WORKING                       (uint8_t)0xAA
#define SIM_STATE_STOP                          (uint8_t)0x55

void _SIM_info_init(SIM_TypeDef *sim)
{
    sim->AD = 0x00;
    sim->state = SIM_STATE_NOSIM;
    flush_array(sim->ICCID);
    flush_array(sim->IMSI);
    flush_array(sim->Rx_APDU);
    sim->Rx_Length = 0x00;
    flush_array(sim->Tx_APDU);
    sim->Tx_Length = 0x00;
}

void _SIMs_Table_init(void)
{
    uint16_t i = 0,j = 0;
    for(i = 0;i < MCU_NUMS;i++)
    {
        //SIM information initail
        for(j = 0;j < SIM_NUMS;j++)
        {
            _SIM_info_init(&MCUs[i].SIM[j]);
        }
        //Hardware version initial
        MCUs[i].HardWare_Version[0] = 0x00;
        MCUs[i].HardWare_Version[1] = 0x00;
        MCUs[i].HardWare_Version[2] = 0x00;
        //Software version initial
        MCUs[i].SoftWare_Version[0] = 0x00;
        MCUs[i].SoftWare_Version[1] = 0x00;
        MCUs[i].SoftWare_Version[2] = 0x00;
        //RxBuf initial
        _SPI_Buf_init(&MCUs[i].RxBuf);
        /*
        MCUs[i].RxBuf.block = 0x00;
        MCUs[i].RxBuf.Length = 0x00;
        MCUs[i].RxBuf.Buf_Len_Left = SPI_TRANSFER_MTU;
        MCUs[i].RxBuf.checksum = 0x00;
        MCUs[i].RxBuf.pre_sum = 0x00;
        MCUs[i].RxBuf.state = SPI_BUF_STATE_EMPTY;
        flush_array(MCUs[i].RxBuf.Buf);
        //TxBuf initial
        MCUs[i].TxBuf.block = 0x00;
        MCUs[i].TxBuf.Length = 0x00;
        MCUs[i].TxBuf.Buf_Len_Left = SPI_TRANSFER_MTU;
        MCUs[i].TxBuf.checksum = 0x00;
        MCUs[i].TxBuf.pre_sum = 0x00;
        MCUs[i].TxBuf.state = SPI_BUF_STATE_EMPTY;
        flush_array(MCUs[i].TxBuf.Buf);
        */
        _SPI_Buf_init(&MCUs[i].TxBuf);
        //MCU events flags initial
        MCUs[i].SIM_APDUTblR = 0x00;
        MCUs[i].SIM_APDUTblW = 0x00;
        MCUs[i].SIM_CheckErrR = 0x00;
        MCUs[i].SIM_CheckErrW = 0x00;
        MCUs[i].SIM_InfoTblR = 0x00;
        MCUs[i].SIM_InfoTblW = 0x00;
        MCUs[i].SIM_StateTblR = 0x00;
        MCUs[i].SIM_StateTblW = 0x00;
        MCUs[i].VersionR = 0x00;
        MCUs[i].VersionW = 0x00;

#ifdef CODING_DEBUG_NO_PRINT
        printf("===================== MCU[%d] =====================\n",i+1);
        print_MCU(&MCUs[i]);
#endif
    }
}

int _apdu_onload(uint8_t *data,SIM_TypeDef *sim,uint8_t len)
{
    if(len > SIM_APDU_LENGTH){
        printf("APDU commands length[%d bytes] is too long than the APDU buffer maximum[%d bytes] length.\n",len,SIM_APDU_LENGTH);
        return  -1;
    }

    uint8_t i;
    for(i = 0;i < len;i++)
    {
        sim->Tx_APDU[i] = data[i];
    }
    sim->Tx_Length = len;
    return 0;
}
