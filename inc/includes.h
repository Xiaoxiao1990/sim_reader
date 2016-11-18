#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "spi.h"
#include "simcard.h"
#include "funcs.h"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define CODING_DEBUG

/******************* end ************************/

MCU_TypeDef MCUs[MCU_NUMS];
int frame_package(MCU_TypeDef *mcu,uint8_t mcu_num);
int frame_parse(MCU_TypeDef *mcu);
int transfer(uint8_t mcu_num);
#endif // INCLUDES_H
