/*
 * chaoshengbo.c
 *
 *  Created on: 2014-9-9
 *      Author: my
 */
#include "include.h"

void TA_init()
{
	TA0CTL |= TASSEL_2 + MC_2 + TACLR;         // SMCLK, 连续增计数, clear TAR
	TA0CCTL2 |= CAP+CM_1+CM_2+CCIS_0+SCS+CCIE;        //捕获模式，上升,下降沿捕获，CCI0A输入，同步捕获，中断允许
}
//超声波端口初始化 1.3输出方波   3.0接收
void IO_init()
{
	P3DIR |= BIT0;    //P30 输出  Trig
	P1DIR &= ~BIT3;  //P13输入   Echo
	P1SEL |= BIT3;   // 第二功能 捕获输入
	P3OUT &= ~BIT0;
	delayms(1);
}

//**********超声波10us高电平发射********************、
void send(void)
{
	P3OUT |= BIT0;
	delayus(10);         //提供10us高电平
	P3OUT &= ~BIT0;
	delayms(100);
}
