/*
 * feihuoliang.h
 *
 *  Created on: 2014-9-8
 *      Author: my
 */

#ifndef FEIHUOLIANG_H_
#define FEIHUOLIANG_H_
//************************************�λ������ñ�������************************************
unsigned int temp,temp_his,wenpiao,i;
unsigned long int rec,rec1,fhl,ad_fhl,fhl_his;			//�λ�����ʱֵ���λ������������ֵ,�λ���ת���ɵ�ad����ֵ
//unsigned int high=165;
//unsigned int wight=53;
unsigned char bili,zhibiao;					//������ռ����ֵ�ðٷֱȣ�1:����2������3������4��������5������
unsigned char xingbie;					//�Ա�1����2��Ů
unsigned int ceshi,ceshi_his;
unsigned char qian,bai,shi,ge;

extern void ta0_init();
extern void adc_init();


#endif /* FEIHUOLIANG_H_ */
