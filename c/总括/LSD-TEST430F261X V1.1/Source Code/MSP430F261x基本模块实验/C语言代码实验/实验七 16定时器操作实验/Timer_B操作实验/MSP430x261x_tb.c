/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430x261x_tb.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者:Huanglongsong
// 描述：Timer_B演示，TB初始化为连续计数模式，CPU被禁止，TBR的时钟来自ACLK(32768HZ),
//      当TBR从50000开始计数到0xFFFF时，产生一次中断，中断服务子程序中取反一
//      次LED状态。
// 生成日期:2008年11月
//
//           MSP430F261x
//         ---------------
//     /|\|            XIN|-
//      | |               | 32kHz
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
    TBR = 50000;                              // TBR从此值基础上开始计数
    TBCTL = TBSSEL_1 + MC_2 + TBIE;           // TB时钟来自ACLK,连续计数模式，开中断
    _BIS_SR(LPM3_bits + GIE);                 // 进入LPM3,开总中断
}

// Timer_B7 多源中断处理
#pragma vector=TIMERB1_VECTOR
__interrupt void Timer_B(void)
{
    switch( TBIV )
    {
    case  2: break;                          // CCR1 不用
    case  4: break;                          // CCR2 不用
    case 14: P5OUT ^= BIT7;                  // TBR计数益出
    break;
    }
    TBR = 50000;                             // 初始化基计数值
    TBCTL &= ~TBIFG;
}

