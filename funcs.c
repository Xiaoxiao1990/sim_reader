/**************************************************************************************
 * Date:2016-11-15
 * Author:Zhou Linlin
 * E-mail:461146760@qq.com
 * Description:
 * Defines some common functions used by other files.
 * ***********************************************************************************/

#include "inc/includes.h"
#include <memory.h>
/*********************************************************
 * Function:flush spi transfer buffer.
 * Params:buffer structure
*********************************************************/
#ifdef CODING_DEBUG         //Functions for debug.

void print_array_r(uint8_t *arr,uint16_t arr_len)
{
    uint16_t i;
//    printf("Array is:\n");
    for(i = 0;i < arr_len;i++)
    {
        printf("%.2X ",arr[i]);
        if(i%15 == 14)puts("");
    }
}

void bin_echo(uint8_t byte)
{
    uint8_t i = 0;
    printf("0B");
    for(i = 0;i < 8;i++)
    {
        if(byte & 0x80)printf("1");
        else printf("0");
        byte <<= 1;
    }
}

void print_sim(SIM_TypeDef *sim)
{
    printf("SIM State:%d\tAD:%d\n",(uint16_t)sim->state,(uint16_t)sim->AD);
    printf("ICCID:\n");
    print_array(sim->ICCID);
    puts("");
    printf("IMSI:\n");
    print_array(sim->IMSI);
    puts("");
    printf("APDU Rx Buffer Length:%d\n",(uint16_t)sim->Rx_Length);
    printf("APDU Rx Buffer:\n");
    print_array(sim->Rx_APDU);
    puts("");
    printf("APDU Tx Buffer Length:%d\n",(uint16_t)sim->Tx_Length);
    printf("APDU Tx Buffer:\n");
    print_array(sim->Tx_APDU);
}

void print_MCU(MCU_TypeDef *mcu)
{
    uint8_t i;
    SPI_Buf_TypeDef *buf = &mcu->RxBuf;
    //print SIMs
    for(i = 0;i < SIM_NUMS;i++)
    {
        printf("============ SIM[%d] ============\n",i+1);
        print_sim(&mcu->SIM[i]);
        puts("\n");
    }
    //Print Rx Buffer
    printf("RxBuf:\n");
    printf("Buffer State:%d\n",buf->state);
    printf("Buffer Length:%d\n",buf->Length);
    printf("Buffer Left Length:%d\n",buf->Buf_Len_Left);
    printf("Buffer Checksum:%d\n",buf->checksum);
    printf("Buffer pre-checksum:%d\n",buf->pre_sum);
    printf("Buffer:\n");
    print_array(buf->Buf);

    buf = &mcu->TxBuf;
    //Print Tx Buffer
    printf("TxBuf:\n");
    printf("Buffer State:%d\n",buf->state);
    printf("Buffer Length:%d\n",buf->Length);
    printf("Buffer Left Length:%d\n",buf->Buf_Len_Left);
    printf("Buffer Checksum:%d\n",buf->checksum);
    printf("Buffer pre-checksum:%d\n",buf->pre_sum);
    printf("Buffer:\n");
    print_array(buf->Buf);
    //Print event flag
    printf("SIM_APDUTblR:");bin_echo(mcu->SIM_APDUTblR);puts("");
    printf("SIM_APDUTblW:");bin_echo(mcu->SIM_APDUTblW);puts("");
    printf("SIM_CheckErrR:");bin_echo(mcu->SIM_CheckErrR);puts("");
    printf("SIM_CheckErrW:");bin_echo(mcu->SIM_CheckErrW);puts("");
    printf("SIM_InfoTblR:");bin_echo(mcu->SIM_InfoTblR);puts("");
    printf("SIM_InfoTblW:");bin_echo(mcu->SIM_InfoTblW);puts("");
    printf("SIM_StateTblR:");bin_echo(mcu->SIM_StateTblR);puts("");
    printf("SIM_StateTblW:");bin_echo(mcu->SIM_StateTblW);puts("");
    printf("SIM_VersionR:");bin_echo(mcu->SIM_VersionR);puts("");
    printf("SIM_VersionW:");bin_echo(mcu->SIM_VersionW);puts("");
    //Print version
    printf("Hardware version:");print_array(mcu->HardWare_Version);puts("");
    printf("Software version:");print_array(mcu->SoftWare_Version);puts("");
}

#endif

void flush_array_r(uint8_t *arr,uint16_t arr_len)
{
#ifdef CODING_DEBUG_NO_PRINT
    uint16_t i;
    printf("Before Initial:\n");
    for(i = 0;i < arr_len;i++)
    {
        printf("%.2X ",arr[i]);
        if(i%15 == 14)puts("");
    }
    puts("");
#endif

     memset(arr,0xff,arr_len);

#ifdef CODING_DEBUG_NO_PRINT
    printf("After Initial:\n");
    for(i = 0;i < arr_len;i++)
    {
      printf("%.2X ",arr[i]);
       if(i%15 == 14)puts("");
    }
    puts("");
#endif
}

/*************************************************************
 * process errors.
 * **********************************************************/
void pabort(const char *s)
{
    perror(s);
    abort();
}
