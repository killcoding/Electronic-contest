/*
 * hx_711.c
 *
 *  Created on: 2014-9-8
 *      Author: my
 */
#include "include.h"


//**************************************************
////初始化ad芯片所使用的IO端口
//*************************************************
void ADInit()
{
	P1SEL &= ~(BIT4+BIT5);
	P1DIR &= ~BIT4;		//SDA
	P1DIR |= BIT5;		//SCL

}
//*****************************************
//延时函数
//*******************************************
void delay_hx711_us(unsigned int t)
{
	while(t--);
}
//*************************************
//读取AD
//**************************************
unsigned long int ReadAD()		//读取AD芯片输出数据,增益128
{
	unsigned long count = 0;
	unsigned char i = 0;
	DATA_H;	//SDA +
	delay_hx711_us(1);
	SCLK_L		//SCL -
	count =0;
	while(P1IN&BIT4);		//等待数据变低
	for(i=0;i<24;i++)
	{
		SCLK_H;		//SCL  +
		count = count << 1;
		SCLK_L;		//SCL
		if(P1IN&BIT4)
			{
			count++;
			}
	}
	SCLK_H;			//SCL
	//count = count^0x800000;
	delay_hx711_us(1);
	SCLK_L;			//SCL
	delay_hx711_us(1);
	return(count);
}
//******************************************
//重量转换
//****************************************
void zhuanhuan()
{
	weight_w = (weight_value/10000)%10;
	weight_q = (weight_value/1000)%10;
	weight_b = (weight_value/100)%10;
	weight_s = (weight_value/10)%10;
	weight_g = weight_value%10;
}
//************************************
//偏置重量求算
//*******************************
void  weight_ofset()
{
	int i;
	 for(i=0;i<20;i++)
	    {
	    	delay_hx711_us(4);
	    	weight_OFSET+=ReadAD();
	    	weight_OFSET/=2;
	    	delay_hx711_us(4);
	    }
	 weight_OFSET/=228;     //单位 十克
}
//***************************************
//采样求平均
//******************************************
void weight_rav()
{
    int i;
    for(i=0;i<30;i++)
    	    {
    	        SUM = ReadAD();
    			SUM=(SUM/228);     //单位 十克
    			if((SUM-3)<weight_OFSET)
    			{
    				SUM=weight_OFSET;
    			}
    			weight_value+=(SUM-weight_OFSET);
    		//	if(i>0)
    		//		{
    				weight_value/=2;
    		//		}
    	    }

}
//****************显示屏显示中重量****************
void display_wight(void)
{
	zhuanhuan();
	display_char6x8(0,0,weight_w+0x30);
	display_char6x8(1,0,weight_q+0x30);
	display_char6x8(2,0,weight_b+0x30);
	display_char6x8(3,0,'.');
	display_char6x8(4,0,weight_s+0x30);
	display_char6x8(5,0,weight_g+0x30);
	weight_rav();
}
