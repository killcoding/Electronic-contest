/*
 * include.h
 *
 *  Created on: 2014-9-8
 *      Author: my
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_
#define CPU_F                 ((double)12000000)
//定义使用主频
#define delayus(x)                    __delay_cycles((long)(CPU_F*(double)x/1000000.00))
#define delayms(x)                    __delay_cycles((long)(CPU_F*(double)x/1000.00))
#include "msp430.h"
#include "lcd_5110.h"
#include "hx_711.h"
#include "feihuoliang.h"
#include "RFID.h"
#include "chaoshengbo.h"


#endif /* INCLUDE_H_ */
