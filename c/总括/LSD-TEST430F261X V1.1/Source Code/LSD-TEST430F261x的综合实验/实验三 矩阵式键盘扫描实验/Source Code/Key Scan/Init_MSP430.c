/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:Init_MSP430.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：MSP430相关函数的定义，配置相关模块的工作模式
** 生成日期:2008年11月
*****************************************************************/
#include <msp430x26x.h>
#include "General_File.h"
/****************************************************************
** 函数名：Init_CLK                                                
** 功能：初始化系统时钟
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void Init_CLK( void )
{
  BCSCTL3 |= XT2S_2;                         // XT2频率范围设置
  BCSCTL1 &= ~XT2OFF;                        // 打开XT2振荡器
  BCSCTL1 |= DIVA_1;                         // ACLK/2
  do
  {
    IFG1 &= ~OFIFG;                        // 清振荡器失效标志
    BCSCTL3 &= ~XT2OF;                     // 清XT2失效标志
    for( uint i = 0x47FF; i > 0; i-- );    // 等待XT2频率稳定
  }while (IFG1 & OFIFG);                     // 外部时钟源正常起动了吗？
  BCSCTL2 |= SELM_2 + SELS + DIVS_2 ;        // 设置MCLK、SMCLK为XT2 
}
/****************************************************************
** 函数名：Init_Port                                               
** 功能：初始化系统时钟
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void Init_Port( void )
{
  P4OUT &= ~( BIT4+BIT3 + BIT6);
  P4SEL |= BIT3;
  P4DIR |= BIT4 + BIT3 + BIT6;
  P7DIR |= 0xFF;
  P7OUT =  0xFF;
  P8DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5;
  P8OUT = 0xFF;
  P5DIR |= BIT7;
  P5OUT |= BIT7;
  
  P1DIR = 0x0f;   //P1.7--p1.4为输入状态  P1.3--P1.0为输出
  P1OUT = 0xf0;   
  P1IES = 0xf0;   //下降沿中断
  P1IE  = 0xf0;   //P1.7--p1.4中断使能
  P1IFG = 0x00;   //标志位清0  
}


