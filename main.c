#include "include.h"


//************************主频设置12M***********************************************
void set_mck()
{
	UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
	  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

	  __bis_SR_register(SCG0);                  // Disable the FLL control loop
	  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	  UCSCTL1 = DCORSEL_5;                      // Select DCO range 24MHz operation
	  UCSCTL2 = FLLD_1 + 374;                   // Set DCO Multiplier for 12MHz
	                                            // (N + 1) * FLLRef = Fdco
	                                            // (374 + 1) * 32768 = 12MHz
	                                            // Set FLL Div = fDCOCLK/2
	  __bic_SR_register(SCG0);                  // Enable the FLL control loop

	  // Worst-case settling time for the DCO when the DCO range bits have been
	  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	  // UG for optimization.
	  // 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
	  __delay_cycles(375000);

	  // Loop until XT1,XT2 & DCO fault flag is cleared
	  do
	  {
	    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                            // Clear XT2,XT1,DCO fault flags
	    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
}

/*
 * main.c
 */
unsigned char status_shuaka;						//rf卡读取完成定义
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
//*************系统初始化*********************/
    set_mck();									//主频12M初始化
    LCD_INIT();									//5110初始化
//***************RF卡调用函数*******************/
    RFIO_Init();
    PcdAntennaOff();
    delayms(5);
    PcdAntennaOn();
 /***************称重部分调用******************/
/*    ADInit();
    weight_ofset();    //偏置重量
    weight_rav();      //采样 求平均
/****************超声波初始化******************/
    IO_init();
    TA_init();
 //**************肺活量部分函数调用**************/
/*    adc_init();
    ta0_init();*/
    _EINT();
	while(1)
	{

		status_shuaka = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡 第二个参数代表读出来的信息
			      //会直接执行到这里
		if (status_shuaka != MI_OK)
		{
			 PcdReset();
			 delaylcd(3);
			 PcdAntennaOff();
			 delaylcd(3);
		     PcdAntennaOn();
			//会直接执行到这里
		     continue;//这样做 跳到上面 的循环开始处
		}
		//刷卡就会执行到这里
		status_shuaka = PcdAnticoll(g_ucTempbuf);//防冲撞
		if (status_shuaka != MI_OK)
		{
			continue;
		}
		//**************重量显示***********************/
		//display_wight();
		//**************肺活量显示*********************/
		display_char6x8(0,1,qian+0x30);
		display_char6x8(2,1,bai+0x30);
		display_char6x8(4,1,shi+0x30);
		display_char6x8(6,1,ge+0x30);
		/***************超声波测距*********************/
		send();
		display_char6x8(0,2,S/10+0x30);
		display_char6x8(1,2,S%10+0x30);
		delayms(300);
		P1OUT ^= BIT0;
		if(g_ucTempbuf[0]==192)
		display_str6x8(0,3,"20120017134");

	}
}
//****************转换后需要显示的量fhl，指标**********************
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
  switch(__even_in_range(ADC12IV,34))
  {
  case  0: break;                           // Vector  0:  No interrupt
  case  2: break;                           // Vector  2:  ADC overflow
  case  4: break;                           // Vector  4:  ADC timing overflow
  case  6:                                  // Vector  6:  ADC12IFG0
		  rec=ADC12MEM0;
		  if(ADC12MEM0 > 0x3a9)
		 	  {
		 	  ad_fhl=7000*(rec -0x3a6);						//无输入时电压0.555，对应ad值为912即0x29a,对应0.5555v
		 	  fhl=(ad_fhl/4096)*2.5;
		 	  fhl=fhl+fhl_his;
		 	  fhl_his=fhl;
		 	  }
//		  if()										//清零测试数据备用
	//	  {
//
	//	  }
  default: break;
  }
}
//****************串口发送数据测试***********************
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
	 while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?

		//  UCA0TXBUF=rec;
		  UCA0TXBUF=wenpiao;
	  break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}
//****************定时器中断，查询肺活量，积分肺活量值**************************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    _EINT();
    ADC12CTL0 |= ADC12SC;
    //以下为收集数据转换（转换int型数为千百十个位）
    qian = ceshi/1000;
    bai = (ceshi%1000)/100;;
    shi = ((ceshi%1000)%100)/10;
    ge = ((ceshi%1000)%100)%10;
    //以下为肺活量指标换算
    	  if(xingbie==1)
    	  {
    		  bili = fhl/(weight_value/100);	//指标所占百分比
    		  if(bili>=78)
    			  zhibiao = 1;
    		  if(bili<78 && bili >=68)
    			  zhibiao = 2;
    		  if(bili<68 && bili >=55)
    			  zhibiao = 3;
    		  if(bili<55 && bili >= 47)
    			  zhibiao = 4;
    		  if(bili<47)
    			  zhibiao = 5;
    	  }
    	  if(xingbie==2)
    	  {
    		 bili = fhl/(weight_value/100);		//指标所占百分比
    		 if(bili>=64)
    		 	zhibiao = 1;
    		 if(bili<64 && bili >=54)
    		 	zhibiao = 2;
    		 if(bili<54 && bili >=43)
    		 	 zhibiao = 3;
    		 if(bili<43 && bili >= 35)
    		 	 zhibiao = 4;
    		 if(bili<35)
    		 	 zhibiao = 5;
    	  }
}
// Timer0_A5 Interrupt Vector (TAIV) handler
//************超声波传播时间测量****************/
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
  switch(__even_in_range(TA0IV,14))
  {
    case  0: break;                          // No interrupt
    case  2: break;                          // CCR1 not used
    case  4:
    	    RiseCapVal_his=RiseCapVal;
    	    RiseCapVal=TA0CCR2;
    	    if(RiseCapVal < RiseCapVal_his)      //计数重新开始
    	    {
    	    	time=65535-RiseCapVal_his+RiseCapVal;
    	    }
    	    else
    	    {
    	    	time=RiseCapVal-RiseCapVal_his;
    	    }
    	   	S=time/12/58;   //单位为厘米
    	break;                          // CCR2
    case  6: break;                          // reserved
    case  8: break;                          // reserved
    case 10: break;                          // reserved
    case 12: break;                          // reserved
    case 14: break;                                // overflow
    default: break;
  }
}
