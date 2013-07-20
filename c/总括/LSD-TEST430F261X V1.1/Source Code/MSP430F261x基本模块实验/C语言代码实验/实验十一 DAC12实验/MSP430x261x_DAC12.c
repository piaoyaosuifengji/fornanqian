/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430F261x_DAC12.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者:Huanglongsong
// 描述：基于MSP430F261x内部DAC12模块，使用内部ADC12REF(2.5V)参考源，待电源电压稳定后，
//      向DAC12_0DAT写入一值，经转换后从DAC0(P6.6)端输出(约1.0V左右)
//
//
//                MSP430x261x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |        DAC0/P6.6|--> 1V
//            |                 |
//
// 作者:Huanglongsong
// 生成日期:2008年11月
******************************************************************************/
#include "msp430x26x.h"

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // 停看门狗
    ADC12CTL0 = REF2_5V + REFON;              // 内部参考基准打开
    TACCR0 = 13600;                           // 等待电源稳定所需时间
    TACCTL0 |= CCIE;                          // 开比较模式中断
    TACTL = TACLR + MC_1 + TASSEL_2;          // 增计数模式, SMCLK
    __bis_SR_register(LPM0_bits + GIE);       // 进入LPM0,开总中断
    TACCTL0 &= ~CCIE;                         // 关计数器中断
    __disable_interrupt();                    // 关总中断
    DAC12_0CTL = DAC12IR + DAC12AMP_5 + DAC12ENC; // 使用内部DAC0
    DAC12_0DAT = 0x0666;                      // 1.0V (2.5V = 0x0FFFh)
    __bis_SR_register(LPM0_bits + GIE);       // 关CPU,处于待机模式
}

//一直等到电源稳定所需时间后执行
#pragma vector = TIMERA0_VECTOR
__interrupt void TA0_ISR(void)
{
    TACTL = 0;                                // 清Timer_A定时器值
    __bic_SR_register_on_exit(LPM0_bits);     // 退出低功耗模式0
}



