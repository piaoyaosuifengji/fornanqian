//*************************************************
//** ��Ȩ:    ����������Ƽ����޹�˾
//** �ļ���:  main_UART.C
//** �汾��
//** ��������: IAR EmbeddedWorkbench 4.11B
//** ����:     LSD
//** ��������: 08.11.10
//** ����:     UART�ĵĵ�Ե�ͨѶ�����ڵ��������Ϸ����ݸ���Ƭ��
//**           ��Ƭ���Ѹ����������·��ظ��ڵ�������
//*****************************************************
#include <msp430x26x.h>

#define  TXD0   BIT4
#define  RXD0   BIT5

void main(void)
{
  
  WDTCTL   =  WDTPW + WDTHOLD;       //�ؿ��Ź�
  BCSCTL1 = CALBC1_1MHZ;               
  DCOCTL = CALDCO_1MHZ;
  P4DIR  |= BIT1;
  P4OUT  &= ~BIT1;           //UART_V�򿪣�ע��㣺�˾�Ϊ��ͨѶģ��ĵ�Դ��
   
  UCA0CTL1 =  UCSWRST;       //��λUCSWRSTλ��ʹUARTģ��ļĴ������ڳ�ʼ״̬
  UCA0CTL1 |= UCSSEL1;       //UCLK=SMCLK=1MHz;
  UCA0BR0  = 104;
  UCA0BR1  = 0;
  UCA0MCTL = UCBRS0;         //���ò����� 9600
  P3DIR |=TXD0;
  P3SEL |=TXD0+RXD0;         //���ͺͽ�������Ϊ��2����
  UCA0CTL1 &= ~UCSWRST;
  IE2 |= UCA0RXIE;           //�����ж�ʹ��
  _EINT();
  while(1)
  {
    LPM0;                    //����͹���
  }
  
}
//**********************************************************
//**����(ģ��)����:USCI0RX_ISR
//**����:	UART�����ж��ڰѽ��ջ����ڵ����ݸ�ֵ�����ͻ���
//**�������: ��     	          		
//**�������: ��  	
//**��������ֵ˵��:�� 		
//**ʹ�õ���Դ����   		
//**����˵��: ��
//************************************************************

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  while (!(IFG2&UCA0TXIFG));
  UCA0TXBUF=UCA0RXBUF;     //�ѽ��յ������ݷ��ظ����ڵ�������
}
