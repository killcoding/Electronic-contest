#include"include.h"


//**************************1ms定时器中断初始化***************************
void ta0_init()
{
	  TA0CCTL0 = CCIE;                          // CCR0 interrupt enabled
	  TA0CCR0 = 3277;							//100ms
	  TA0CTL = TASSEL_1 + MC_1 + TACLR;         // ACLK, upmode, clear TAR
}
//***************************毫秒延时*********************************
void delay(unsigned int ms)//1ms延时函数
{
	unsigned int i,x;
	for(x=0;x<ms;x++)
	{
		for(i=0;i<10;i++);
	}
}
//*********************串口初始化***********************************
void uart_init()
{
	  P3SEL = BIT3+BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
	  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
	  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	  UCA0BR0 = 6;                              // 1MHz 9600 (see User's Guide)
	  UCA0BR1 = 0;                              // 1MHz 9600
	  UCA0MCTL = UCBRS_0 + UCBRF_13 + UCOS16;   // Modln UCBRSx=0, UCBRFx=0,
	                                            // over sampling
	  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}
//******************adc初始化***************************************
void adc_init()
{
	  P6SEL |= 0x01;                            // P6.0 ADC option select
	  REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control to
	                                            // ADC12_A ref control registers
	ADC12CTL0 = ADC12SHT02 + ADC12ON+ ADC12REFON+ADC12REF2_5V;         // Sampling time, ADC12 on vref=1.5
	  ADC12CTL1 = ADC12SHP;                     // Use sampling timer
	  ADC12MCTL0 = ADC12SREF_1;
	  ADC12IE = 0x01;                           // Enable interrupt
	  ADC12CTL0 |= ADC12ENC;

}
