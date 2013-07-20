/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430F261x_DMA.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者:Huanglongsong
// 功能描述： MSP430x26x的DMA演示例程，通过TA触发DMA0将一组字符串量以单字节重复
//    传输到目的地址P5OUT寄存器中，每传输一次改变一次P5OUT寄存器的值，P5.7上连接
//    的LED的闪烁体现出变化状态！
//   ACLK = 32kHz, MCLK = SMCLK = TACLK = default DCO 1.045MHz
//
//               MSP430F261x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P5.7|--> LED
//            |                 | 
//
//
******************************************************************************/
#include "msp430x26x.h"

const char testconst[2] = { 0x0, 0x80 };

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // 停看门狗
    P5DIR |= BIT7;                            // P5.7输出模式
    DMACTL0 = DMA0TSEL_1;                     // TACCR2为DMA0的触发源
    DMA0SA = (int)testconst;                  // 源地址初始化
    DMA0DA = (int)&P5OUT;                     // 目的地址初始化
    DMA0SZ = sizeof testconst;                // 传输长度
    DMA0CTL = DMADT_4 + DMASRCINCR_3 + DMASBDB + DMAEN; // 单字节重复传输，源地址增加，启动DMA
    TACTL = TASSEL_2 + MC_2;                  // 初始化TA，时钟源自SMCLK，连续计数模式
    __bis_SR_register(LPM0_bits + GIE);       // 进入低功耗模式0( 关CPU)
}
