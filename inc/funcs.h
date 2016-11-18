/**************************************************************************************
 * Date:2016-11-15
 * Author:Zhou Linlin
 * E-mail:461146760@qq.com
 * Description:
 * This file is the header file of funcs.c. Defines some common functions used by
 * other files.
 * ***********************************************************************************/

#ifndef FUNCS_H
#define FUNCS_H

void pabort(const char *s);

void flush_array_r(uint8_t *arr,uint16_t arr_len);
#define flush_array(a) flush_array_r(a,ARRAY_SIZE(a))

void print_array_r(uint8_t *arr,uint16_t arr_len);
#define print_array(a) print_array_r(a,ARRAY_SIZE(a))

void bin_echo(uint8_t byte);
void print_sim(SIM_TypeDef *sim);
void print_MCU(MCU_TypeDef *mcu);
uint8_t slot_parse(uint8_t *actionTbl);
void clear_flag(uint8_t *ActionTbl, uint8_t slot);
void _SIMs_Printer(void);

#endif // FUNCS_H
