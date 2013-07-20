/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430x261x_ta.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者:Huanglongsong
// 描述：Timer_A演示，TA初始化为连续计数模式，CPU被禁止，当TAR计数到CCR0(50000)时，
// 产生一次中断(TA_ISR)，触发P5.7输出翻转
//  ACLK = n/a, MCLK = SMCLK = TACLK = default DCO ~1.045MHz
// 生成日期:2008年11月
//
//           MSP430F261x
//         ---------------
//     /|\|            XIN|-
//      | |               |
//      --|RST        XOUT|-
//        |               |
//        |           P5.7|-->LED
//
//
******************************************************************************/

#include  <msp430x26x.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // 停看门狗
  P5DIR |= BIT7;                            // P5.7为输出模式
  CCTL0 = CCIE;                             // CCR0 中断使能
  CCR0 = 50000;
  TACTL = TASSEL_2 + MC_2;                  // SMCLK, 连续计数模式

  _BIS_SR(LPM0_bits + GIE);                 // 关CPU,使能总中断
}

// Timer A0 中断服务子程序
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  P5OUT ^= BIT7;                            // 翻转LED状态
  CCR0 += 50000;                            // 添加CCR0偏移量
}

