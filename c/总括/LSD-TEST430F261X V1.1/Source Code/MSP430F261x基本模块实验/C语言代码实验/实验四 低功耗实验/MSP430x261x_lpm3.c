/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430F261x_lpm3.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者:Huanglongsong 
// 描述：MSP430系列特有的低功耗试验例程，初始化完系统后进入低功耗模式三，由WDT定时
//      器定时４秒后唤醒CPU,执行一次LED闪烁，继续进入低功耗模式三，这样周而复始的
//      进行着。
//  ACLK = LFXT1/4 = 32768/4, MCLK = SMCLK = default DCO ~1.045MHz
// 生成日期:2008年11月
//
//        MSP430F261x
//         ---------------
//     /|\|            XIN|-
//      | |               | 32kHz
//      --|RST        XOUT|-
//        |               |
//        |           P5.7|-->LED
//
//
******************************************************************************/
#include "msp430x26x.h"

volatile unsigned int i;

void main(void)
{
    BCSCTL1 |= DIVA_2;                        // ACLK/4 = 8.192 kHz
    WDTCTL = WDT_ADLY_1000;                   // WDT时钟来自ACLK/8,定时时间4秒进入中断
    // 中断周期：4sec=1/4Hz(freq)
    IE1 |= WDTIE;                             // 开 WDT 中断
    P1DIR = 0xFF;                             // 所有 P1.x 输出
    P1OUT = 0;                                // 所有 P1.x 复位
    P2DIR = 0xFF;                             // 所有 P2.x 输出
    P2OUT = 0;                                // 所有 P2.x 复位
    P3DIR = 0xFF;                             // 所有 P3.x 输出
    P3OUT = 0;                                // 所有 P3.x 复位
    P4DIR = 0xFF;                             // 所有 P4.x 输出
    P4OUT = 0;                                // 所有 P4.x 复位
    P5DIR = 0xFF;                             // 所有 P5.x 输出
    P5OUT = 0;                                // 所有 P5.x 复位
    P6DIR = 0xFF;                             // 所有 P6.x 输出
    P6OUT = 0;                                // 所有 P6.x 复位
    
    while(1)
    {
        __bis_SR_register(LPM3_bits + GIE);     // 进入LPM3, 开总中断
        P5OUT |= BIT7;                          // 置位P5.7 LED 开
        for (i = 20000; i > 0; i--);            // 延时
        P5OUT &= ~BIT7;                         // 复位P5.7 LED 关
    }
}

//唤醒系统
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    __bic_SR_register_on_exit(LPM3_bits);       // 退出LPM3;
}
