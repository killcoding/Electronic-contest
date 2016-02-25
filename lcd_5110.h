/*
 * lcd_5110.h
 *
 *  Created on: 2014-9-6
 *      Author: my
 */

#ifndef LCD_5110_H_
#define LCD_5110_H_
#define CLK_H P7OUT|=BIT0
#define CLK_L P7OUT&=~BIT0
#define DIN_H P6OUT |= BIT1
#define DIN_L P6OUT &= ~BIT1
#define DC_H   P6OUT|=BIT2
#define DC_L   P6OUT&=~BIT2
#define CE_H  P6OUT|=BIT3
#define CE_L  P6OUT&=~BIT3
#define REST_H  P6OUT|=BIT4
#define REST_L  P6OUT&=~BIT4
#define uchar unsigned char

extern void LCD_INIT(void);
extern void display_char6x8(uchar x, uchar y,uchar c);
extern void display_str6x8(uchar x, uchar y, uchar *str);
extern void display_chinese(unsigned char row, unsigned char page,unsigned char *c) ;
extern void display_chinese_str(uchar x, uchar y,uchar n, uchar *str);
extern void display_picture(uchar x, uchar y, uchar lon,uchar high);
extern void delaylcd(unsigned int ms);

#endif /* LCD_5110_H_ */
