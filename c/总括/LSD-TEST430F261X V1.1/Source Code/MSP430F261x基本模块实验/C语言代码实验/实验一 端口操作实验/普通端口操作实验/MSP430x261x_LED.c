/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430F261x_LED.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者:Huanglongsong 
// 描述：通过例程演示如何设置I,配置P5.7端口的相关寄存器，实现LED闪烁
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
//******************************************************************************/

#include  <msp430x26x.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // 关闭看门狗
    P5DIR |= BIT7;                            // P5.7口设置为输出 

    while (1)
    {
        volatile unsigned int i;
        P5OUT ^= BIT7;                        // P5.7翻转
        i = 50000;                            // 延时
        do (i--);
        while (i != 0);
    }
}
