/*
 * hx_711.h
 *
 *  Created on: 2014-9-8
 *      Author: my
 */

#ifndef HX_711_H_
#define HX_711_H_
//******************HX711���Ŷ���ͱ�������*******************************************************
unsigned long  SUM,weight_OFSET,weight_value;             //��Ų���ֵ֮�͵�ȫ�ֱ���
unsigned char weight_w,weight_q,weight_b,weight_s,weight_g;
#define DATA_H P1OUT |= BIT4;   //����1.2��Ϊ����ת����
#define DATA_L P1OUT &= ~BIT4;   //����1.2��Ϊ����ת����
#define SCLK_H  P1OUT |= BIT5;    //����1.3Ϊʱ�������
#define SCLK_L  P1OUT &= ~BIT5;    //����1.3Ϊʱ�������

extern void ADInit();
extern unsigned long int ReadAD();
extern void zhuanhuan();
extern void weight_rav();
extern void  weight_ofset();
extern void display_wight(void);
extern void delay_hx711_us(unsigned int t);


#endif /* HX_711_H_ */
