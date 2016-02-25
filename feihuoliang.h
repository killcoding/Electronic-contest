/*
 * feihuoliang.h
 *
 *  Created on: 2014-9-8
 *      Author: my
 */

#ifndef FEIHUOLIANG_H_
#define FEIHUOLIANG_H_
//************************************肺活量所用变量定义************************************
unsigned int temp,temp_his,wenpiao,i;
unsigned long int rec,rec1,fhl,ad_fhl,fhl_his;			//肺活量临时值，肺活量计算出理论值,肺活量转换成的ad代表值
//unsigned int high=165;
//unsigned int wight=53;
unsigned char bili,zhibiao;					//测量所占优秀值得百分比，1:优秀2：良好3：及格4：不及格5：极差
unsigned char xingbie;					//性别1：男2：女
unsigned int ceshi,ceshi_his;
unsigned char qian,bai,shi,ge;

extern void ta0_init();
extern void adc_init();


#endif /* FEIHUOLIANG_H_ */
