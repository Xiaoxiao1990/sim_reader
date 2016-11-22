/************************************************************************
 * SPI opreation
 * Single thread mode/Multi-thread mode.
 * 1)package data
 * 2)parse data
 * 3)transfer
 * *********************************************************************/
#include "inc/includes.h"
/**************************************************************
 * SPI Property
 * ***********************************************************/
//#define SPI0                0
//#define SPI1                1
typedef enum{
    SPI0 = 0,
    SPI1 = 1
}SPI_NUM_TypeDef;

static const char *dev_path[2] ={
    "/dev/spidev0.0",
    "/dev/spidev2.0"
};
static const char *device = &dev_path[SPI0];
static uint8_t spi_mode;
static uint8_t spi_bits = 8;
static uint32_t spi_speed = 5000000;
static uint16_t trans_delay;

/*********************************************************
 * Theres is something wrong.
 * ******************************************************/
static void print_usage(const char *prog)
{
    printf("Usage: %s [-DsbdlHOLC3]\n", prog);
    puts("  -D --device   device to use (default /dev/spidev0.0)\n"
         "  -s --speed    max speed (Hz)\n"
         "  -d --delay    delay (usec)\n"
         "  -b --bpw      bits per word \n"
         "  -l --loop     loopback\n"
         "  -H --cpha     clock phase\n"
         "  -O --cpol     clock polarity\n"
         "  -L --lsb      least significant bit first\n"
         "  -C --cs-high  chip select active high\n"
         "  -3 --3wire    SI/SO signals shared\n");
    exit(1);
}

/**********************************************************
 * Function:Parse the options.
 * Params.:argv[]
 * *******************************************************/
static void parse_opts(int argc, char *argv[])
{
    while (1) {
        static const struct option lopts[] = {
            { "device",  1, 0, 'D' },
            { "speed",   1, 0, 's' },
            { "delay",   1, 0, 'd' },
            { "bpw",     1, 0, 'b' },
            { "loop",    0, 0, 'l' },
            { "cpha",    0, 0, 'H' },
            { "cpol",    0, 0, 'O' },
            { "lsb",     0, 0, 'L' },
            { "cs-high", 0, 0, 'C' },
            { "3wire",   0, 0, '3' },
            { "no-cs",   0, 0, 'N' },
            { "ready",   0, 0, 'R' },
            { NULL, 0, 0, 0 },
        };
        int c;

        c = getopt_long(argc, argv, "D:s:d:b:lHOLC3NR", lopts, NULL);

        if (c == -1)break;

        switch (c){
        case 'D':device = optarg;break;
        case 's':spi_speed = atoi(optarg);break;
        case 'd':trans_delay = atoi(optarg);break;
        case 'b':spi_bits = atoi(optarg);break;
        case 'l':spi_mode |= SPI_LOOP;break;
        case 'H':spi_mode |= SPI_CPHA;break;
        case 'O':spi_mode |= SPI_CPOL;break;
        case 'L':spi_mode |= SPI_LSB_FIRST;break;
        case 'C':spi_mode |= SPI_CS_HIGH;break;
        case '3':spi_mode |= SPI_3WIRE;break;
        case 'N':spi_mode |= SPI_NO_CS;break;
        case 'R':spi_mode |= SPI_READY;break;
        default:print_usage(argv[0]);break;
        }
    }
}

/******************************************************************
 * SPI buffer inital
 * ***************************************************************/
void _SPI_Buf_init(SPI_Buf_TypeDef *buf)
{
    buf->block = 0x00;
    buf->Length = 0x00;
    buf->Buf_Len_Left = SPI_TRANSFER_MTU;
    buf->checksum = 0x00;
    buf->pre_sum = 0x00;
    buf->state = SPI_BUF_STATE_EMPTY;
    flush_array(buf->Buf);
}

/*******************************************************************
 * Function:SPI device initial
 * ****************************************************************/
void _SPI_Dev_Init(int argc, char *argv[])
{
    int ret = 0;
    int fd;
#ifdef CODING_DEBUG_NO_PRINT
    printf("==================start[_SPI_Dev_Init()]=================\n");
#endif
    //Parse options
    parse_opts(argc, argv);

    //SPI0 init
    fd = open(dev_path[SPI0], O_RDWR);
    if (fd < 0)pabort("Can't open SPI[0]");

    //set SPI mode
    ret = ioctl(fd, SPI_IOC_WR_MODE, &spi_mode);
    if(ret == -1)pabort("Can't set SPI[0] write mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &spi_mode);
    if(ret == -1)pabort("Can't get SPI[0] read mode");

    //set bits per word.
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits);
    if(ret == -1)pabort("Can't set  SPI_IOC_WR_BITS_PER_WORD of SPI[0]");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits);
    if(ret == -1)pabort("Can't set SPI_IOC_RD_BITS_PER_WORD of SPI[0]");

    // max speed hz

    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(ret == -1)pabort("Can't set SPI_IOC_WR_MAX_SPEED_HZ of SPI[0]");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(ret == -1)pabort("Can't set SPI_IOC_RD_MAX_SPEED_HZ of SPI[0]");

    printf("SPI[0]:\n");
    printf("SPI Mode: %d\n", spi_mode);
    printf("Bits Per Word: %d\n", spi_bits);
    printf("Max Speed: %d Hz (%d KHz)\n", spi_speed, spi_speed/1000);

    //close SPI0
    close(fd);

    //SPI1 init
    fd = open(dev_path[SPI1], O_RDWR);
    if (fd < 0)pabort("Can't open SPI[1]");

    //set SPI mode
    ret = ioctl(fd, SPI_IOC_WR_MODE, &spi_mode);
    if(ret == -1)pabort("Can't set SPI[1] write mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &spi_mode);
    if(ret == -1)pabort("Can't get SPI[1] read mode");

    //set bits per word.
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits);
    if(ret == -1)pabort("Can't set  SPI_IOC_WR_BITS_PER_WORD of SPI[1]");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits);
    if(ret == -1)pabort("Can't set SPI_IOC_RD_BITS_PER_WORD of SPI[1]");

    // max speed hz

    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(ret == -1)pabort("Can't set SPI_IOC_WR_MAX_SPEED_HZ of SPI[1]");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(ret == -1)pabort("Can't set SPI_IOC_RD_MAX_SPEED_HZ of SPI[1]");

    printf("SPI[1]:\n");
    printf("SPI Mode: %d\n", spi_mode);
    printf("Bits Per Word: %d\n", spi_bits);
    printf("Max Speed: %d Hz (%d KHz)\n", spi_speed, spi_speed/1000);

    //close SPI1
    close(fd);

#ifdef CODING_DEBUG_NO_PRINT
    printf("==================End [_SPI_Dev_Init()]=================\n");
#endif
}

/**************************************************************************************
 * Function:Message Package.
 * ***********************************************************************************/

/***********************************************************************************
 * pack the read software&hardware version command.
 * ********************************************************************************/

int block_length_check(DataType_TypeDef datatype,uint8_t data_len)
{
    switch(datatype)
    {
        case DUMMY_READ:return SPI_TRANSFER_MTU;break;
        case READ_SWHW:return 3;break;
        case STOP_SIM:return 3;break;
        case RESET_SIM:return 3;break;
        case READ_INFO:return 3;break;
        case READ_STATE:return 3;break;
        case APDU_CMD:return data_len + 3;break;
        case TRANS_ERR:return 3;break;
        default:;
    }
    return SPI_TRANSFER_MTU + SPI_TRANSFER_MTU;//wrong type can't allocate.
}

#define DATA_FRAME_HEAD1                                (uint8_t)0xA5
#define DATA_FRAME_HEAD2                                (uint8_t)0xA5
/*************************************************************************************
 * Function:Transmit data packaging.
 * Params:MCU event flag.
 * return:Process status.
 * Note:All data in one frame should be have the same distination.
 * **********************************************************************************/

int frame_package(MCU_TypeDef *mcu,uint8_t mcu_num)//(uint8_t sim_no, DataType_TypeDef datatype, uint8_t *data)
{//Running in an independent thread.
    uint8_t i;
    uint8_t sim_no = 0;
    uint8_t *data,data_len;
    uint8_t *actionTbl;
    DataType_TypeDef datatype;
    SPI_Buf_TypeDef *MCU_Tx_WriteP = &(mcu->TxBuf);

    if(MCU_Tx_WriteP->state == SPI_BUF_STATE_TRANSMITING)
    {
        printf("Tx buffer is busy now, try again by next time.\n");
        return -1;
    }
    //printf("Tx Buffer can be load data.\n");
    //To avoid transmit a packaging buffer.
    //MCU_Tx_WriteP->state = SPI_BUF_STATE_PACKAGING;
    while(mcu->SIM_StateTblR | mcu->SIM_ResetTbl | mcu->SIM_StopTbl | mcu->SIM_APDUTblR | mcu->SIM_InfoTblR | mcu->VersionR| mcu->SIM_CheckErrR)
    {
     //   printf("There is something new need to pack to the buffer.\n");
        //SIM_NO. allocate
        if(mcu->SIM_StateTblR)
        {
            actionTbl = &(mcu->SIM_StateTblR);
            sim_no = slot_parse(actionTbl);
            datatype = READ_STATE;
            printf("Read SIM[%d] State.\n",((mcu_num*MCU_NUMS)+sim_no));
        }
        else if(mcu->SIM_ResetTbl)
        {
            actionTbl = &(mcu->SIM_ResetTbl);
            sim_no = slot_parse(actionTbl);
            datatype = RESET_SIM;
            printf("Reset SIM[%d].\n",((mcu_num*MCU_NUMS)+sim_no));
        }
        else if(mcu->SIM_StopTbl)
        {
            actionTbl = &(mcu->SIM_StopTbl);
            sim_no = slot_parse(actionTbl);
            datatype = STOP_SIM;
            printf("Stop SIM[%d].\n",((mcu_num*MCU_NUMS)+sim_no));
        }
        else if(mcu->SIM_APDUTblR)
        {
            actionTbl = &(mcu->SIM_APDUTblR);
            sim_no = slot_parse(actionTbl);
            datatype = APDU_CMD;
            printf("APDU Commands To SIM[%d].\n",((mcu_num*MCU_NUMS)+sim_no));
        }
        else if(mcu->SIM_CheckErrR)
        {
            actionTbl = &(mcu->SIM_CheckErrR);
            sim_no = slot_parse(actionTbl);
            datatype = TRANS_ERR;
            printf("SIM[%d] Checksum Error.\n",((mcu_num*MCU_NUMS)+sim_no));
        }
        else if(mcu->SIM_InfoTblR)
        {
            actionTbl = &(mcu->SIM_InfoTblR);
            sim_no = slot_parse(actionTbl);
            datatype = READ_INFO;
            printf("Read Information From SIM[%d].\n",((mcu_num*MCU_NUMS)+sim_no));
        }
        else if(mcu->VersionR)
        {
            actionTbl = &(mcu->VersionR);
            mcu->VersionR &= 0x00;
            sim_no = 0;//slot_parse(actionTbl);
            datatype = READ_SWHW;
            printf("Read HW&SW Version From MCU[%d].\n",mcu_num + 1);
        }

        //Prepare data for APDU
        //sim_no 0~5
        if(sim_no > 0)
        {
            data_len = mcu->SIM[sim_no - 1].Tx_Length; // offset 1. array start from 0
            data = mcu->SIM[sim_no - 1].Tx_APDU;
        }//else for all 5 SIM cards
        //calculate Tx buffer size free space.
        if(MCU_Tx_WriteP->state == SPI_BUF_STATE_EMPTY)
        {//The first time come to this buffer.
            MCU_Tx_WriteP->Buf_Len_Left = SPI_TRANSFER_MTU - 5;
#ifdef CODING_DEBUG_NO_PRINT
            printf("This is an empty buffer.\n");
#endif
        }
        else
        {//There is something packed in this buffer.
            //re-calculate the Buffer space left.
            MCU_Tx_WriteP->Buf_Len_Left = SPI_TRANSFER_MTU - MCU_Tx_WriteP->Length - 1;
#ifdef CODING_DEBUG_NO_PRINT
            printf("This buffer has %d bytes left.\n",MCU_Tx_WriteP->Buf_Len_Left);
#endif
        }

        if(mcu->TxBuf.Buf_Len_Left < block_length_check(datatype,data_len))
        {
            //There is no space to load this block,wait for next package.
            mcu->TxBuf.state = SPI_BUF_STATE_FULL;
            printf("There is no space to allocate data block, try again by next time.\n");
            return -1;
        }
        else
        {
            //Clear the flag
            clear_flag(actionTbl,sim_no);
        }
        switch(MCU_Tx_WriteP->state)
        {
            case SPI_BUF_STATE_EMPTY:
                //Init SPI transfer buffer before load data.
                _SPI_Buf_init(MCU_Tx_WriteP);
                MCU_Tx_WriteP->state = SPI_BUF_STATE_PACKAGING;
                //Frame Head;
                MCU_Tx_WriteP->Buf[0] = DATA_FRAME_HEAD1;
                MCU_Tx_WriteP->pre_sum += MCU_Tx_WriteP->Buf[0];
                MCU_Tx_WriteP->Buf[1] = DATA_FRAME_HEAD2;
                MCU_Tx_WriteP->pre_sum += MCU_Tx_WriteP->Buf[1];

                //Locate to block
                MCU_Tx_WriteP->Length = 5;//Length is Buf[] position.

                if(MCU_Tx_WriteP->Buf_Len_Left < data_len)
                {
                    printf("The data length is too long beyond the transfer Maximum [%d] bytes.\n",(uint16_t)ARRAY_SIZE(MCU_Tx_WriteP->Buf));
                    return -1;
                }
            case SPI_BUF_STATE_PACKAGING:               //Reserved by multi-thread.
            case SPI_BUF_STATE_READY:
            {
                //data block begin
                MCU_Tx_WriteP->Buf[MCU_Tx_WriteP->Length] = sim_no;                       //slot
                MCU_Tx_WriteP->pre_sum += MCU_Tx_WriteP->Buf[MCU_Tx_WriteP->Length++];

                //printf("Length = %d\n",MCU_Tx_WriteP->Length);

                MCU_Tx_WriteP->Buf[MCU_Tx_WriteP->Length] = (datatype == APDU_CMD)?(data_len+1):0x01;//sub length
               // printf("sub length = %d\n",data_len);

                MCU_Tx_WriteP->pre_sum += MCU_Tx_WriteP->Buf[MCU_Tx_WriteP->Length++];

                MCU_Tx_WriteP->Buf[MCU_Tx_WriteP->Length] = datatype;                     //data type
                MCU_Tx_WriteP->pre_sum += MCU_Tx_WriteP->Buf[MCU_Tx_WriteP->Length++];

                if(datatype == APDU_CMD)                                                    //Others are all 1 byte length.
                {//Here,SPI_Tx_WriteP->Length point to block data.
                    for(i = 0;i < data_len;i++)
                    {
                        MCU_Tx_WriteP->Buf[MCU_Tx_WriteP->Length++] = data[i];
                        data[i] = 0xFF;//flush APDU tx buffer.
                        MCU_Tx_WriteP->pre_sum += data[i];
                    }
                    //flush APDU buffer?
                    mcu->SIM[sim_no - 1].Tx_Length = 0;//clear
                }
                //Here,SPI_Tx_WriteP->Length point to checksum.
                //Block quantity
                MCU_Tx_WriteP->block++;
                MCU_Tx_WriteP->Buf[4] = MCU_Tx_WriteP->block;

                //pre_sum stop to calculate.
                MCU_Tx_WriteP->checksum = MCU_Tx_WriteP->pre_sum;
                MCU_Tx_WriteP->checksum += MCU_Tx_WriteP->Buf[4];

                //Data total length = Length - checksum(1 byte) - frame head(2 bytes) - total length(2 bytes) + 1(start with 0) = Length - 4
                MCU_Tx_WriteP->Buf[2] = (uint8_t)((MCU_Tx_WriteP->Length - 4) >> 8);
                MCU_Tx_WriteP->checksum += MCU_Tx_WriteP->Buf[2];
                MCU_Tx_WriteP->Buf[3] = (uint8_t)((MCU_Tx_WriteP->Length - 4) & 0xFF);
                MCU_Tx_WriteP->checksum += MCU_Tx_WriteP->Buf[3];
                //data block end

                MCU_Tx_WriteP->Buf[MCU_Tx_WriteP->Length] = MCU_Tx_WriteP->checksum;
                MCU_Tx_WriteP->state = SPI_BUF_STATE_READY;
            }break;
            case SPI_BUF_STATE_FULL:
            {
                if(MCU_Tx_WriteP->Buf_Len_Left >= block_length_check(datatype,data_len))
                {//Load something more.
                    MCU_Tx_WriteP->state = SPI_BUF_STATE_READY;
                }
            }break;
            case SPI_BUF_STATE_TRANSMITING:break;//Never been here.
            default:;
        }
    }
    return 0;
}

/******************************************************************************************
 * Function:Frame parse
 * Params:Rx_Buf
 * ***************************************************************************************/
#define PARSE_FRAME_HEAD1                                   (uint8_t)0x00
#define PARSE_FRAME_HEAD2                                   (uint8_t)0x01
#define PARSE_TOTAL_LENGTH                                  (uint8_t)0x02
#define PARSE_BLOCK_NUMS                                    (uint8_t)0x03
#define PARSE_BLOCK_SLOT                                    (uint8_t)0x04
#define PARSE_BLOCK_SUBLEN                                  (uint8_t)0x05
#define PARSE_BLOCK_TYPE                                    (uint8_t)0x06
#define PARSE_BLOCK_DATA                                    (uint8_t)0x07
#define PARSE_FRAME_CHECKSUM                                (uint8_t)0x08
/******************************************************************************************
 * Function: Set flags while get information from card boards.
 * ***************************************************************************************/
static void set_flag(uint8_t *ActionTbl, uint8_t slot)
{
    //printf("flag before set:%d\n",*ActionTbl);
    switch(slot)
    {
        case SIM_NO_ALL:*ActionTbl |= SIM_NO_ALL;break;
        case SIM_NO_1:*ActionTbl |= SIM_NO_1_BIT;break;
        case SIM_NO_2:*ActionTbl |= SIM_NO_2_BIT;break;
        case SIM_NO_3:*ActionTbl |= SIM_NO_3_BIT;break;
        case SIM_NO_4:*ActionTbl |= SIM_NO_4_BIT;break;
        case SIM_NO_5:*ActionTbl |= SIM_NO_5_BIT;break;
        default:;
    }
    //printf("flag after set:%d\n",*ActionTbl);
}

int frame_parse(MCU_TypeDef *mcu)
{
    uint16_t i,total_length,j;
    uint8_t blocks,checksum,slot,sublen,k;
    DataType_TypeDef datatype;
    uint8_t step = 0;
    SPI_Buf_TypeDef *Rx = &(mcu->RxBuf);

    switch(Rx->state)
    {
        case SPI_BUF_STATE_TRANSMITING:
        case SPI_BUF_STATE_EMPTY:
        {
            //Nothing to do.
            printf("It seems nothing to parse.\n");
        }break;
        case SPI_BUF_STATE_READY:
        case SPI_BUF_STATE_FULL:
        {
            for(i = 0;i < ARRAY_SIZE(Rx->Buf);i++)
            {//Frame start
                switch(step)
                {
                    case PARSE_FRAME_HEAD1:
                    {
                        checksum = 0;
                        if(Rx->Buf[i] == DATA_FRAME_HEAD1)
                        {
                            step++;
                        }

                    }break;
                    case PARSE_FRAME_HEAD2:
                    {
                        if(Rx->Buf[i] == DATA_FRAME_HEAD2)
                        {
                            step++;
                        }
                        else step--;

                    }break;
                    case PARSE_TOTAL_LENGTH:
                    {
                        total_length = Rx->Buf[i++] << 8;
                        total_length += Rx->Buf[i];
#ifdef CODING_DEBUG_NO_PRINT
                        printf("Total data length:%d\n",total_length);
#endif
                        if(total_length < 2)//data length should be great than 2 bytes.
                        {
                            //Here ,May something wrong.Report an Error?
                            step = PARSE_FRAME_HEAD1;
                            flush_array(Rx->Buf);
#ifdef CODING_DEBUG
                            printf("Total Data length is too short.\n");
#endif
                            return -1;
                        }
                        else if(total_length > ARRAY_SIZE(Rx->Buf))
                        {
                            step = PARSE_FRAME_HEAD1;
                            flush_array(Rx->Buf);
#ifdef CODING_DEBUG
                            printf("Total Data length is too long.\n");
#endif
                            return -1;
                        }
                        else
                        {
                            //Calculate checksum.
#ifdef CHECKSUM_ON
                            for(j = 0;j < total_length + 2;j++)
                            {
                                checksum += Rx->Buf[j];
                            }
                            if(checksum != Rx->Buf[j])
                            {
                                printf("Received Data Checksum Error!\n");
                            }
                            return -1;
#endif
                            step++;
                        }
                    }break;
                    case PARSE_BLOCK_NUMS:
                    {
                        blocks = Rx->Buf[i];
                        if(blocks < 1)//data blocks should be great than 0.
                        {
                            step = PARSE_FRAME_HEAD1;
                            flush_array(Rx->Buf);
                            return -1;
                        }
                        else step++;

                    }break;
                    case PARSE_BLOCK_SLOT:
                    {
                        slot = Rx->Buf[i];
                        step++;
                    }break;
                    case PARSE_BLOCK_SUBLEN:
                    {
                        sublen = Rx->Buf[i];
                        step++;
                    }break;
                    case PARSE_BLOCK_TYPE:
                    {
                        datatype = Rx->Buf[i];
                        step++;
#ifdef CODING_DEBUG_NO_PRINT
                        printf("Datatype:");
                        switch(datatype)
                        {
                            case DUMMY_READ:puts("DUMMY_READ");break;
                            case READ_SWHW:puts("READ_SWHW");break;
                            case STOP_SIM:puts("STOP_SIM");break;
                            case RESET_SIM:puts("RESET_SIM");break;
                            case READ_INFO:puts("READ_INFO");break;
                            case READ_STATE:puts("READ_STATE");break;
                            case APDU_CMD:puts("APDU_CMD");break;
                            case TRANS_ERR:puts("TRANS_ERR");break;
                            default:;
                        }
#endif
                    }break;
                    case PARSE_BLOCK_DATA:
                    {
                        switch (datatype)
                        {
                            case DUMMY_READ:break;          //Never been here
                            case READ_SWHW:
                            {
                                mcu->SoftWare_Version[0] = Rx->Buf[i++];
                                mcu->SoftWare_Version[1] = Rx->Buf[i++];
                                mcu->SoftWare_Version[2] = Rx->Buf[i++];
                                mcu->HardWare_Version[0] = Rx->Buf[i++];
                                mcu->HardWare_Version[1] = Rx->Buf[i++];
                                mcu->HardWare_Version[2] = Rx->Buf[i];

                                //Get new version
                                mcu->VersionW |= 0x1F;
                            }break;
                            case STOP_SIM:break;            //Never been here
                            case RESET_SIM:break;           //Never been here
                            case READ_INFO:
                            {
                                for(k = 0;k < ICCID_LENGTH;k++)//ICCID
                                {
                                    mcu->SIM[slot-1].ICCID[k] = Rx->Buf[i++];
                                }
                                for(k = 0;k < IMSI_LENGTH;k++)//IMSI
                                {
                                    mcu->SIM[slot-1].IMSI[k] = Rx->Buf[i++];
                                }
                                mcu->SIM[slot-1].AD = Rx->Buf[i];
                                set_flag(&mcu->SIM_InfoTblW,slot);
                            }break;
                            case READ_STATE:
                            {
                                mcu->SIM[slot-1].state = Rx->Buf[i];
                                set_flag(&mcu->SIM_StateTblW,slot);
                            }break;
                            case APDU_CMD:
                            {
                                for(k = 0;k < (sublen - 1);k++)mcu->SIM[slot-1].Rx_APDU[k] = Rx->Buf[i++];
                                mcu->SIM[slot-1].Rx_Length = k;
                                set_flag(&mcu->SIM_APDUTblW,slot);
                                i--;//back to data(use for PARSE_FRAME_CHECKSUM
                            }break;
                            case TRANS_ERR:
                            {
                                set_flag(&mcu->SIM_CheckErrW,slot);
                            }break;               //Maybe should do something here.
                            default:/*Error*/;
                        }
                        if(--blocks > 0)step = PARSE_BLOCK_SLOT;
                        else step++;
                    }break;
                    case PARSE_FRAME_CHECKSUM:
                    {//This state has been do before parsing information. Here just recovery the status machine.
                        step = PARSE_FRAME_HEAD1;
                        //Flush Rx buffer for next time.
                        flush_array(mcu->RxBuf.Buf);
                        mcu->RxBuf.state = SPI_BUF_STATE_EMPTY;
                    }break;
                    default:;
                }
            }//Frame end
        }break;
        default:;
    }
    return 0;
}

/***************************************************************************
 * Function:transmit.
 * Params:
 * dev:SPI0/SPI1
 * tx:transfer buffer
 * rx:receive buffer
 * ************************************************************************/
int transfer(uint8_t mcu_num)
{
    int ret;
    MCU_TypeDef *mcu = &MCUs[mcu_num];
    //Prepare to transmit.
//    mcu->RxBuf.state = SPI_BUF_STATE_FULL;
    /*
    switch(mcu->RxBuf.state)
    {
        case SPI_BUF_STATE_EMPTY:break;//That's OK.
        case SPI_BUF_STATE_PACKAGING:break;//Never been here.Used in multi-thread.
        case SPI_BUF_STATE_READY://There is something in the Rx Buffer.
        case SPI_BUF_STATE_FULL:frame_parse(mcu);break;
        case SPI_BUF_STATE_TRANSMITING:break;//Never been here if it's work in single thread mode.
        default:;
    }
    */
    _SPI_Buf_init(&mcu->RxBuf);  //Back to initial.
//    mcu->RxBuf.state = SPI_BUF_STATE_TRANSMITING;

/*
    switch(mcu->TxBuf.state)
    {
        case SPI_BUF_STATE_EMPTY:flush_array(mcu->TxBuf.Buf);//Just do a dummy read or package something.
        case SPI_BUF_STATE_PACKAGING:frame_package(mcu);break;//
        case SPI_BUF_STATE_READY:
        case SPI_BUF_STATE_FULL:mcu->TxBuf.state = SPI_BUF_STATE_TRANSMITING;break;
        case SPI_BUF_STATE_TRANSMITING:break;
        default:;
    }
*/
    _SPI_Buf_init(&mcu->TxBuf);
    if(mcu->SIM_StateTblR | mcu->SIM_ResetTbl | mcu->SIM_StopTbl | mcu->SIM_APDUTblR | mcu->SIM_CheckErrR | mcu->SIM_InfoTblR | mcu->VersionR)
    {
#ifdef  CODING_DEBUG

#endif
      frame_package(mcu,mcu_num);
#ifdef CODING_DEBUG_NO_PRINT
      printf("======================== Tx Buffer ==========================\n");
      print_array(mcu->TxBuf.Buf);
#endif
    }
//    mcu->TxBuf.state = SPI_BUF_STATE_TRANSMITING;

    struct spi_ioc_transfer tr={
            .tx_buf = (unsigned long)mcu->TxBuf.Buf,
            .rx_buf = (unsigned long)mcu->RxBuf.Buf,
            .len = ARRAY_SIZE(mcu->TxBuf.Buf),
            .delay_usecs = trans_delay,
            .speed_hz = spi_speed,
            .bits_per_word = spi_bits,
    };
    //struct spi_ioc_transfer tr;
    //Transfer
    int fd;
    uint8_t spi_num;
    if(mcu_num < 56)spi_num = SPI0;
    else spi_num = SPI1;
    //SPI0==>MCU[0~55]   SPI1 ==> MCU[56~103]
    fd = open(dev_path[spi_num], O_RDWR);
    if(fd < 0)
    {
        printf("Can't open SPI[%d] !\n",spi_num);
        perror("");
        abort();
    }
    //Transmitting.
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if(ret < 1)
    {
        printf("Can't send message by SPI[%d].\n",spi_num);
        perror("");
        abort();
    }

    _SPI_Buf_init(&mcu->TxBuf);
    mcu->TxBuf.state = SPI_BUF_STATE_EMPTY;

#ifdef CODING_DEBUG_NO_PRINT
    printf("Recevied data:\n");
    printf("=======================================================\n");
    print_array(mcu->RxBuf.Buf);
    printf("=======================================================\n");
#endif
    mcu->RxBuf.state = SPI_BUF_STATE_FULL;//Enable frame parse.
    frame_parse(mcu);
    _SIMs_Printer();
//    printf("++++++++++++++++++++++++++++++++++++++ Parse +++++++++++++++++++++++++++++++++++++\n");
//    print_MCU(mcu);
    close(fd);
    return 0;
}
