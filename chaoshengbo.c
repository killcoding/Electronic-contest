/*
 * chaoshengbo.c
 *
 *  Created on: 2014-9-9
 *      Author: my
 */
#include "include.h"

void TA_init()
{
	TA0CTL |= TASSEL_2 + MC_2 + TACLR;         // SMCLK, ����������, clear TAR
	TA0CCTL2 |= CAP+CM_1+CM_2+CCIS_0+SCS+CCIE;        //����ģʽ������,�½��ز���CCI0A���룬ͬ�������ж�����
}
//�������˿ڳ�ʼ�� 1.3�������   3.0����
void IO_init()
{
	P3DIR |= BIT0;    //P30 ���  Trig
	P1DIR &= ~BIT3;  //P13����   Echo
	P1SEL |= BIT3;   // �ڶ����� ��������
	P3OUT &= ~BIT0;
	delayms(1);
}

//**********������10us�ߵ�ƽ����********************��
void send(void)
{
	P3OUT |= BIT0;
	delayus(10);         //�ṩ10us�ߵ�ƽ
	P3OUT &= ~BIT0;
	delayms(100);
}
