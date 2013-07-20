//*************************************************
//** 版权:    杭州利尔达科技有限公司
//** 文件名:  main_UART.C
//** 版本：
//** 工作环境: IAR EmbeddedWorkbench 4.11B
//** 作者:     LSD
//** 生成日期: 08.11.10
//** 功能:     UART的的点对点通讯，串口调试助手上发数据给单片机
//**           单片机把该数据又重新发回给口调试助手
//*****************************************************
#include <msp430x26x.h>

#define  TXD0   BIT4
#define  RXD0   BIT5

void main(void)
{
  
  WDTCTL   =  WDTPW + WDTHOLD;       //关看门狗
  BCSCTL1 = CALBC1_1MHZ;               
  DCOCTL = CALDCO_1MHZ;
  P4DIR  |= BIT1;
  P4OUT  &= ~BIT1;           //UART_V打开（注意点：此句为打开通讯模块的电源）
   
  UCA0CTL1 =  UCSWRST;       //置位UCSWRST位，使UART模块的寄存器处于初始状态
  UCA0CTL1 |= UCSSEL1;       //UCLK=SMCLK=1MHz;
  UCA0BR0  = 104;
  UCA0BR1  = 0;
  UCA0MCTL = UCBRS0;         //设置拨特率 9600
  P3DIR |=TXD0;
  P3SEL |=TXD0+RXD0;         //发送和接收引脚为第2功能
  UCA0CTL1 &= ~UCSWRST;
  IE2 |= UCA0RXIE;           //接收中断使能
  _EINT();
  while(1)
  {
    LPM0;                    //进入低功耗
  }
  
}
//**********************************************************
//**函数(模块)名称:USCI0RX_ISR
//**功能:	UART接收中断内把接收缓存内的数据赋值给发送缓存
//**输入参数: 无     	          		
//**输出参数: 无  	
//**函数返回值说明:无 		
//**使用的资源：无   		
//**其它说明: 无
//************************************************************

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  while (!(IFG2&UCA0TXIFG));
  UCA0TXBUF=UCA0RXBUF;     //把接收到的数据返回给串口调试助手
}
