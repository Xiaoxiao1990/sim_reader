/*
 * SIM Card Reader (Version 0.0 for test.)
 *
 * Copyright (c) 2016  TGT, Inc. Ltd.
 * Copyright (c) 2016  Linlin Zhou <461146760@qq.com>
 *
 * This program is software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include "inc/includes.h"


static unsigned long RepeatTimes = 0;

int main(int argc, char *argv[])
{
    uint8_t i;
    _SPI_Dev_Init(argc,argv);
//    printf("MCUs[] size:%d\tSIMs[] size:%d\t SIM ICCID[] size:%d\n",ARRAY_SIZE(MCUs),ARRAY_SIZE(MCUs[0].SIM),ARRAY_SIZE(MCUs[0].SIM[0].ICCID));
    _SIMs_Table_init();
    while(1)
    {
    //printf("SIM_InfoTblR:%d\t",MCUs[0].SIM_InfoTblR);bin_echo(MCUs[0].SIM_InfoTblR);puts("");
    MCUs[0].SIM_StateTblR |= (SIM_NO_1_BIT|SIM_NO_2_BIT|SIM_NO_3_BIT|SIM_NO_4_BIT|SIM_NO_5_BIT);
    // MCUs[0].SIM_StateTblR |=;
    //MCUs[0].SIM_InfoTblR |= SIM_NO_2_BIT;
    //MCUs[0].SIM_CheckErrR |= SIM_NO_3_BIT;
//    MCUs[0].SIM_InfoTblR |= (SIM_NO_1_BIT|SIM_NO_2_BIT|SIM_NO_3_BIT|SIM_NO_4_BIT|SIM_NO_5_BIT);
    //MCUs[0].SIM_VersionR |= (SIM_NO_1_BIT|SIM_NO_2_BIT|SIM_NO_3_BIT|SIM_NO_4_BIT|SIM_NO_5_BIT);
//    MCUs[0].SIM_StopTbl |= (SIM_NO_1_BIT|SIM_NO_2_BIT|SIM_NO_3_BIT|SIM_NO_4_BIT|SIM_NO_5_BIT);
    MCUs[0].VersionR |= (SIM_NO_1_BIT|SIM_NO_2_BIT|SIM_NO_3_BIT|SIM_NO_4_BIT|SIM_NO_5_BIT);
    printf("+++++++++++++++++++++++++++++++++++++++ Read SIM Info. & Send Read Version Read SIM state. +++++++++++++++++++++++++++++++++++++\n");
//    sleep(3);
    transfer(0);
    sleep(1);
    /*
    MCUs[0].SIM[SIM_NO_4 - 1].Tx_APDU[0] = 0x00;
    MCUs[0].SIM[SIM_NO_4 - 1].Tx_APDU[1] = 0x88;
    MCUs[0].SIM[SIM_NO_4 - 1].Tx_APDU[2] = 0x00;
    MCUs[0].SIM[SIM_NO_4 - 1].Tx_APDU[3] = 0x81;
    MCUs[0].SIM[SIM_NO_4 - 1].Tx_APDU[4] = 0x22;
    MCUs[0].SIM[SIM_NO_4 - 1].Tx_Length = 0x05;//TxAPDU Length
    */
    //printf("Tx_APDU Length:%d\nTx_APDU:\n",MCUs[0].SIM[SIM_NO_5 - 1].Tx_Length);
    //print_array(MCUs[0].SIM[SIM_NO_5 - 1].Tx_APDU);puts("");
    //printf("TxBuf:\n");
    //print_array(MCUs[0].TxBuf.Buf);puts("");
    //sleep(1);
    //printf("+++++++++++++++++++ After package something to the Tx buffer. ++++++++++++++++\n");
    //print_array(MCUs[0].TxBuf.Buf);
    //printf("+++++++++++++++++++++++++++++ Transimitting ++++++++++++++++++++++++++++++++++\n");
//    transfer(0);
    MCUs[0].SIM_ResetTbl |= (SIM_NO_1_BIT|SIM_NO_2_BIT|SIM_NO_3_BIT|SIM_NO_4_BIT|SIM_NO_5_BIT);
    printf("+++++++++++++++++++++++++++++++++++++++ Restartting SIM Cards +++++++++++++++++++++++++++++++++++++\n");
//    sleep(3);
    transfer(0);
    sleep(1);
    printf("Please Wait ...\n");
    sleep(1);
    printf("Please Wait ...\n");
    sleep(1);
    printf("Please Wait ...\n");
    sleep(1);
    MCUs[0].SIM_APDUTblR |= (SIM_NO_1_BIT|SIM_NO_2_BIT|SIM_NO_3_BIT|SIM_NO_4_BIT|SIM_NO_5_BIT);
    MCUs[0].SIM_StateTblR |= (SIM_NO_1_BIT|SIM_NO_2_BIT|SIM_NO_3_BIT|SIM_NO_4_BIT|SIM_NO_5_BIT);
    uint8_t apdu1[] = {
        0x00,0x88,0x00,0x81,0x22
    },
    apdu2[] = {
        0x10,0xA6,0x75,0x25,0xAE,0x62,0x13,0x74,0x98,0x06,0x8F,0x5D,0x55,0x97,0x4E,0xD7,0x94,
        0x10,0x8A,0x87,0xE1,0x50,0xF2,0xB0,0x82,0x34,0xF1,0xA3,0x49,0x9D,0x91,0xA3,0x8F,0xAD
    };
    for(i = 0;i < SIM_NUMS;i++)
    _apdu_onload(apdu1,&MCUs[0].SIM[i],ARRAY_SIZE(apdu1));

    printf("+++++++++++++++++++++++++++++++++++++++ Send Authentication Request. +++++++++++++++++++++++++++++++++++++\n");
//    sleep(3);
    transfer(0);
    sleep(1);
    printf("+++++++++++++++++++++++++++++++++++++++ Read Authentication Request Acknowledge. & Send Random Number +++++++++++++++++++++++++++++++++++++\n");
//    sleep(3);
    MCUs[0].SIM_APDUTblR |= (SIM_NO_1_BIT|SIM_NO_2_BIT|SIM_NO_3_BIT|SIM_NO_4_BIT|SIM_NO_5_BIT);
    for(i = 0;i < SIM_NUMS;i++)
    _apdu_onload(apdu2,&MCUs[0].SIM[i],ARRAY_SIZE(apdu2));
    transfer(0);
    sleep(1);
    printf("+++++++++++++++++++++++++++++++++++++++ Read Authentication State +++++++++++++++++++++++++++++++++++++\n");
//    sleep(3);
    transfer(0);
    MCUs[0].SIM_StopTbl |= (SIM_NO_1_BIT|SIM_NO_2_BIT|SIM_NO_3_BIT|SIM_NO_4_BIT|SIM_NO_5_BIT);
    sleep(1);
    printf("+++++++++++++++++++++++++++++++++++++++ Stop SIM Cards +++++++++++++++++++++++++++++++++++++\n");
//    sleep(3);
    transfer(0);
    sleep(1);
    printf("+++++++++++++++++++++++++++++++++++++++ Read SIM State +++++++++++++++++++++++++++++++++++++\n");
//    sleep(2);
    transfer(0);
    sleep(1);
    printf("Repeat Times:%d\n",++RepeatTimes);
    sleep(3);
    }
    exit(0);
}
