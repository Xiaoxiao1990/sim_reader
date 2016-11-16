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


//static unsigned long RepeatTimes = 0;

int main(int argc, char *argv[])
{
    _SPI_Dev_Init(argc,argv);
//    printf("MCUs[] size:%d\tSIMs[] size:%d\t SIM ICCID[] size:%d\n",ARRAY_SIZE(MCUs),ARRAY_SIZE(MCUs[0].SIM),ARRAY_SIZE(MCUs[0].SIM[0].ICCID));
    _SIMs_Table_init();
    //printf("SIM_InfoTblR:%d\t",MCUs[0].SIM_InfoTblR);bin_echo(MCUs[0].SIM_InfoTblR);puts("");
    MCUs[0].SIM_StateTblR |= SIM_NO_1_BIT;
    MCUs[0].SIM_InfoTblR |= SIM_NO_2_BIT;
    MCUs[0].SIM_CheckErrR |= SIM_NO_3_BIT;
    MCUs[0].SIM_VersionR |= SIM_NO_4_BIT;
    MCUs[0].SIM_APDUTblR |= SIM_NO_5_BIT;
    MCUs[0].SIM[SIM_NO_5 - 1].Tx_APDU[0] = 0x00;
    MCUs[0].SIM[SIM_NO_5 - 1].Tx_APDU[1] = 0x88;
    MCUs[0].SIM[SIM_NO_5 - 1].Tx_APDU[2] = 0x00;
    MCUs[0].SIM[SIM_NO_5 - 1].Tx_APDU[3] = 0x81;
    MCUs[0].SIM[SIM_NO_5 - 1].Tx_APDU[4] = 0x22;
    MCUs[0].SIM[SIM_NO_5 - 1].Tx_Length = 0x05;//TxAPDU Length
    printf("Tx_APDU Length:%d\nTx_APDU:\n",MCUs[0].SIM[SIM_NO_5 - 1].Tx_Length);
    print_array(MCUs[0].SIM[SIM_NO_5 - 1].Tx_APDU);puts("");
    printf("TxBuf:\n");
    print_array(MCUs[0].TxBuf.Buf);puts("");
    frame_package(&MCUs[0]);
    sleep(1);
    printf("+++++++++++++++++++ After package something to the Tx buffer. ++++++++++++++++\n");
    print_array(MCUs[0].TxBuf.Buf);
    //while(1)
    {
        //return 0;
    }
    exit(0);
}
