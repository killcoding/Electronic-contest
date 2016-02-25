/*
 * hx_711.h
 *
 *  Created on: 2014-9-8
 *      Author: my
 */

#ifndef HX_711_H_
#define HX_711_H_
//******************HX711引脚定义和变量定义*******************************************************
unsigned long  SUM,weight_OFSET,weight_value;             //存放采样值之和的全局变量
unsigned char weight_w,weight_q,weight_b,weight_s,weight_g;
#define DATA_H P1OUT |= BIT4;   //定义1.2口为数据转换口
#define DATA_L P1OUT &= ~BIT4;   //定义1.2口为数据转换口
#define SCLK_H  P1OUT |= BIT5;    //定义1.3为时钟输出口
#define SCLK_L  P1OUT &= ~BIT5;    //定义1.3为时钟输出口

extern void ADInit();
extern unsigned long int ReadAD();
extern void zhuanhuan();
extern void weight_rav();
extern void  weight_ofset();
extern void display_wight(void);
extern void delay_hx711_us(unsigned int t);


#endif /* HX_711_H_ */
