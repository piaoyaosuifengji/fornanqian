//**********************************************************
//** 版权:  杭州利尔达科技有限公司
//** 文件名: MSP430irDA.c
//** 版本:1.0
//** 工作环境: IAR 4.11B 
//** 作者:     LSD
//** 生成日期: 2008.09.22
//** 功能: 红外发送接收
//** 修改日志：无
//*********************************************************
#include "msp430x26x.h"

#define  TXD0  BIT4
#define  RXD0  BIT5   //红外

#define  TXD1  BIT6
#define  RXD1  BIT7   //上传给PC

char data[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC};
char qq;
void main( void )
{
  WDTCTL   =  WDTPW + WDTHOLD;       //关看门狗
  BCSCTL1  =  CALBC1_1MHZ;               
  DCOCTL   =  CALDCO_1MHZ;
  P4DIR    |= BIT2+BIT1;
  P4OUT    &= ~(BIT1 + BIT2);    //BIT2->红外头可接收;BIT1->提供UART_V
  
//=========================USCI A0内的UART==========================
  UCA0CTL1 =  UCSWRST;
  UCA0CTL1 |= UCSSEL1;           //UCLK=SMCLK;
  UCA0BR0  = 104;
  UCA0BR1  = 0;
  UCA0MCTL = UCBRS0;             //设置拨特率 9600
  P3DIR |= TXD0;
  P3SEL |= TXD0 + RXD0;
  UCA0IRTCTL = UCIREN + UCIRTXPL5+UCIRTXPL4;
  UCA0IRRCTL = 0;
  //UCA0IRRCTL = UCIRRXPL;
  UCA0CTL1 &= ~UCSWRST;
  IE2 |= UCA0RXIE;
  
//===================USCI A1内的UART=================================  
  UCA1CTL1 =  UCSWRST;
  UCA1CTL1 |= UCSSEL1;           //UCLK=SMCLK=1MHz;
  UCA1BR0  = 52;
  UCA1BR1  = 0;
  UCA1MCTL = 0;                  //设置拨特率 19200
  P3DIR |=TXD1;
  P3SEL |=TXD1; //+RXD1;
  UCA1CTL1 &= ~UCSWRST;
  
  TACTL = TASSEL0 + TACLR;       //TA时钟源是ACLK=32768Hz
  CCR0  = 32768>>1;               
  TACTL |= MC0;                  //增计数模式
  
  _EINT();
  
  while(1)
  {
    while( !(TAIFG & TACTL));    //0.5秒定时到
    TACTL &= ~TAIFG;             //清标志位
    while( !(IFG2 & UCA0TXIFG));
    UCA0TXBUF = data[qq++];       //发送数据
    if(qq==12)                   //数组内的12个数据发送完毕？
    {
     qq =  0;                    
    }
    LPM0;
  }
  
}
//*************************************************
//**函数(模块)名称:USCI0RX_ISR
//**功能:	UART接收中断内把接收缓存内的数据赋值给发送缓存
//**输入参数: 无     	          		
//**输出参数: 无  	
//**函数返回值说明:无 		
//**使用的资源：无   		
//**其它说明: 无
//*************************************************

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  while (!(UC1IFG & UCA1TXIFG));
  UCA1TXBUF=UCA0RXBUF;                //把接收到的数据返回给串口调试助手
  LPM0_EXIT;                          //退出低功耗
}