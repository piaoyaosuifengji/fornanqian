//**********************************************************
//** ��Ȩ:  ����������Ƽ����޹�˾
//** �ļ���: MSP430irDA.c
//** �汾:1.0
//** ��������: IAR 4.11B 
//** ����:     LSD
//** ��������: 2008.09.22
//** ����: ���ⷢ�ͽ���
//** �޸���־����
//*********************************************************
#include "msp430x26x.h"

#define  TXD0  BIT4
#define  RXD0  BIT5   //����

#define  TXD1  BIT6
#define  RXD1  BIT7   //�ϴ���PC

char data[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC};
char qq;
void main( void )
{
  WDTCTL   =  WDTPW + WDTHOLD;       //�ؿ��Ź�
  BCSCTL1  =  CALBC1_1MHZ;               
  DCOCTL   =  CALDCO_1MHZ;
  P4DIR    |= BIT2+BIT1;
  P4OUT    &= ~(BIT1 + BIT2);    //BIT2->����ͷ�ɽ���;BIT1->�ṩUART_V
  
//=========================USCI A0�ڵ�UART==========================
  UCA0CTL1 =  UCSWRST;
  UCA0CTL1 |= UCSSEL1;           //UCLK=SMCLK;
  UCA0BR0  = 104;
  UCA0BR1  = 0;
  UCA0MCTL = UCBRS0;             //���ò����� 9600
  P3DIR |= TXD0;
  P3SEL |= TXD0 + RXD0;
  UCA0IRTCTL = UCIREN + UCIRTXPL5+UCIRTXPL4;
  UCA0IRRCTL = 0;
  //UCA0IRRCTL = UCIRRXPL;
  UCA0CTL1 &= ~UCSWRST;
  IE2 |= UCA0RXIE;
  
//===================USCI A1�ڵ�UART=================================  
  UCA1CTL1 =  UCSWRST;
  UCA1CTL1 |= UCSSEL1;           //UCLK=SMCLK=1MHz;
  UCA1BR0  = 52;
  UCA1BR1  = 0;
  UCA1MCTL = 0;                  //���ò����� 19200
  P3DIR |=TXD1;
  P3SEL |=TXD1; //+RXD1;
  UCA1CTL1 &= ~UCSWRST;
  
  TACTL = TASSEL0 + TACLR;       //TAʱ��Դ��ACLK=32768Hz
  CCR0  = 32768>>1;               
  TACTL |= MC0;                  //������ģʽ
  
  _EINT();
  
  while(1)
  {
    while( !(TAIFG & TACTL));    //0.5�붨ʱ��
    TACTL &= ~TAIFG;             //���־λ
    while( !(IFG2 & UCA0TXIFG));
    UCA0TXBUF = data[qq++];       //��������
    if(qq==12)                   //�����ڵ�12�����ݷ�����ϣ�
    {
     qq =  0;                    
    }
    LPM0;
  }
  
}
//*************************************************
//**����(ģ��)����:USCI0RX_ISR
//**����:	UART�����ж��ڰѽ��ջ����ڵ����ݸ�ֵ�����ͻ���
//**�������: ��     	          		
//**�������: ��  	
//**��������ֵ˵��:�� 		
//**ʹ�õ���Դ����   		
//**����˵��: ��
//*************************************************

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  while (!(UC1IFG & UCA1TXIFG));
  UCA1TXBUF=UCA0RXBUF;                //�ѽ��յ������ݷ��ظ����ڵ�������
  LPM0_EXIT;                          //�˳��͹���
}