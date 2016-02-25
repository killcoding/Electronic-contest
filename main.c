#include "include.h"


//************************��Ƶ����12M***********************************************
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
unsigned char status_shuaka;						//rf����ȡ��ɶ���
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
//*************ϵͳ��ʼ��*********************/
    set_mck();									//��Ƶ12M��ʼ��
    LCD_INIT();									//5110��ʼ��
//***************RF�����ú���*******************/
    RFIO_Init();
    PcdAntennaOff();
    delayms(5);
    PcdAntennaOn();
 /***************���ز��ֵ���******************/
/*    ADInit();
    weight_ofset();    //ƫ������
    weight_rav();      //���� ��ƽ��
/****************��������ʼ��******************/
    IO_init();
    TA_init();
 //**************�λ������ֺ�������**************/
/*    adc_init();
    ta0_init();*/
    _EINT();
	while(1)
	{

		status_shuaka = PcdRequest(PICC_REQALL, g_ucTempbuf);//Ѱ�� �ڶ��������������������Ϣ
			      //��ֱ��ִ�е�����
		if (status_shuaka != MI_OK)
		{
			 PcdReset();
			 delaylcd(3);
			 PcdAntennaOff();
			 delaylcd(3);
		     PcdAntennaOn();
			//��ֱ��ִ�е�����
		     continue;//������ �������� ��ѭ����ʼ��
		}
		//ˢ���ͻ�ִ�е�����
		status_shuaka = PcdAnticoll(g_ucTempbuf);//����ײ
		if (status_shuaka != MI_OK)
		{
			continue;
		}
		//**************������ʾ***********************/
		//display_wight();
		//**************�λ�����ʾ*********************/
		display_char6x8(0,1,qian+0x30);
		display_char6x8(2,1,bai+0x30);
		display_char6x8(4,1,shi+0x30);
		display_char6x8(6,1,ge+0x30);
		/***************���������*********************/
		send();
		display_char6x8(0,2,S/10+0x30);
		display_char6x8(1,2,S%10+0x30);
		delayms(300);
		P1OUT ^= BIT0;
		if(g_ucTempbuf[0]==192)
		display_str6x8(0,3,"20120017134");

	}
}
//****************ת������Ҫ��ʾ����fhl��ָ��**********************
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
		 	  ad_fhl=7000*(rec -0x3a6);						//������ʱ��ѹ0.555����ӦadֵΪ912��0x29a,��Ӧ0.5555v
		 	  fhl=(ad_fhl/4096)*2.5;
		 	  fhl=fhl+fhl_his;
		 	  fhl_his=fhl;
		 	  }
//		  if()										//����������ݱ���
	//	  {
//
	//	  }
  default: break;
  }
}
//****************���ڷ������ݲ���***********************
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
//****************��ʱ���жϣ���ѯ�λ��������ַλ���ֵ**************************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    _EINT();
    ADC12CTL0 |= ADC12SC;
    //����Ϊ�ռ�����ת����ת��int����Ϊǧ��ʮ��λ��
    qian = ceshi/1000;
    bai = (ceshi%1000)/100;;
    shi = ((ceshi%1000)%100)/10;
    ge = ((ceshi%1000)%100)%10;
    //����Ϊ�λ���ָ�껻��
    	  if(xingbie==1)
    	  {
    		  bili = fhl/(weight_value/100);	//ָ����ռ�ٷֱ�
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
    		 bili = fhl/(weight_value/100);		//ָ����ռ�ٷֱ�
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
//************����������ʱ�����****************/
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
    	    if(RiseCapVal < RiseCapVal_his)      //�������¿�ʼ
    	    {
    	    	time=65535-RiseCapVal_his+RiseCapVal;
    	    }
    	    else
    	    {
    	    	time=RiseCapVal-RiseCapVal_his;
    	    }
    	   	S=time/12/58;   //��λΪ����
    	break;                          // CCR2
    case  6: break;                          // reserved
    case  8: break;                          // reserved
    case 10: break;                          // reserved
    case 12: break;                          // reserved
    case 14: break;                                // overflow
    default: break;
  }
}
