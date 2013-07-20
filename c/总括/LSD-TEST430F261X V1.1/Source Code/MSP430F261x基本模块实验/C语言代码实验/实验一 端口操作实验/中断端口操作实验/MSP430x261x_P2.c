/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430F261x_P2.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者: Huanglongsong 
// 描述：按键中断实验例程，初始化P2.5(Enter键)为输入模式，内部电阻上拉，下降沿中
//       断有效，系统进入LPM4,当按下Enter键后，进入中断服务子程式序，改变一次LED状态
// 生成日期:2008年11月
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//               MSP430F261x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//     /|\   |                 |
//      --o--|P2.5         P5.7|-->LED
//     \|/
//
//
//
******************************************************************************/
#include "msp430x26x.h" 

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // 停止开门狗
  P5DIR |= BIT7;                            // P5.7为输出模式
  P2REN |= BIT5;                            // 内部电阻使能
  P2IE  |= BIT5;                            // P2.5 中断使能
  P2IES |= BIT5;                            // P2.5 从高到低的变化触发中断
  P2IFG &= ~BIT5;                           // P2.5 IFG清零

  __bis_SR_register(LPM4_bits + GIE);       // 进入LPM4 并开总中断
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  P5OUT ^= BIT7;                            // P5.7 取反
  P2IFG &= ~BIT5;                          // P2.5 IFG清零
}

