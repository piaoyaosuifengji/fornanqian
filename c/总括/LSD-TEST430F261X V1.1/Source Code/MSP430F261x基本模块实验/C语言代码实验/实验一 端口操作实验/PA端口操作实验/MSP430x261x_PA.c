/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430F261x_PA.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者:Huanglongsong
// 描述：MSP430F261x演示程序，向PA(P7、P8)写入一个字的操作，通过
//       看门狗定时唤醒CPU，每唤醒一次改变一次PAOUT的状态,在
//       LSD_TESTMSP430F261x学习板上能看到PA端口的LED状态!
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO
//
//                MSP430F261x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST         PA.0 |-- LED
//            |             |   | |
//            |             |   | |
//            |           PA.15 |-- LED
//
//
//
******************************************************************************/
#include  <msp430x26x.h>

void main(void)
{
    WDTCTL = WDT_ADLY_250;                   // 定时WDT 0.5秒复位
    IE1 |= WDTIE;
    P3OUT &= ~BIT2;   
    P3DIR |= BIT2;                           // 开电源
    PASEL = 0;                               // 复位Pa的第二功能，普通端口模式
    PAOUT = 0xAAAA;
    PADIR |= 0xFFFF;                         // PA.x 输出
    while(1)
    {
        PAOUT ^= 0x0FFFF;                    // 所有PA 取反 
        _BIS_SR( LPM3_bits + GIE );          // 进入LPM3;
    }
}

//唤醒系统
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    __bic_SR_register_on_exit(LPM3_bits);    // 退出LPM3;
}