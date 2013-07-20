/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430x261x_SVS.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者:Huanglongsong
// 描述：SVS模块的特性演示，通过SVS监测VCC,当VCC高于2.5V时，MSP430内部触发P5.7
//      翻转闪烁，当VCC低于2.5V时，由SVS产生一个复位信号(POR)复位MSP430!
//  ACLK= n/a, MCLK= SMCLK= default DCO ~ 1.045MHz
// 生成日期:2008年11月
//
//                MSP430F261x
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P5.7|-->LED
//
//
******************************************************************************/

#include  <msp430x26x.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // 停看门狗
  P5DIR |= BIT7;                            // P5.7为输出模式
  SVSCTL = 0x60 + PORON;                    // SVS复位使能@ 2.5V

  for (;;)
  {
    volatile unsigned int i;
    i = 50000;                              // 延时
    do 
    (i--);while (i != 0);
    P5OUT ^= BIT7;                          // VCC正常时P5.7端口上的LED闪烁
  }
}
