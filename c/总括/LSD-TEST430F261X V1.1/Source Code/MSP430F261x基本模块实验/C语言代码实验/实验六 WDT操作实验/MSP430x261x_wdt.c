/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430F261x_WDT.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者:Huanglongsong 
// 描述：MSP430系列WDT模块实验例程，配置WDT为定时器模式，间隔1S改变一次Led状态
//  ACLK = LFXT1/4 = 32768/4, MCLK = SMCLK = default DCO ~1.045MHz
// 生成日期:2008年11月
//
//                 MSP430F261x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P5.7|-->LED
//
//
******************************************************************************/
#include <msp430x26x.h>

void main(void)
{
  WDTCTL =  WDT_ADLY_250;                   // WDT的时钟源自ACLK，经分频后得到4HZ的时钟
  IE1 |= WDTIE;				    // 开WDT定时器中断
  P5DIR |= BIT7;                            // 设置P5.7为输出模式

  __bis_SR_register(LPM3_bits + GIE);       // 开中断，系统进入低功耗模式3
}

// 看门狗中断服务子程序
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
  P5OUT ^= BIT7;                            // 触发取反一次P5.7的输出状态
}

