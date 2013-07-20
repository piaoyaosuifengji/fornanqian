/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430x261x_rosc.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者:Huanglongsong
// 描述：通过外部电阻调节内部DCO的电流源，改变电流的大小DCO的频率随之变化，MCLK默认
//      选择DCO直接通过P5.4输出，可以通过示波器或者频率计测试DCO的频率随外部电阻的变
//      化而变化。同时，P5.7口上的LED闪烁也随之改变。
//  R=100K -> ~2MHz (参考器件手据手册)
// 生成日期:2008年11月
//
//               MSP430F261x
//            -----------------
//   /|\  /|\|              XIN|-
//    |    | |                 |
//    R    --|RST          XOUT|-
//    |      |                 |
//    -------|P2.5/ROSC    P5.7|-->LED
//           |        P5.4/MCLK|-->MCLK = DCOCLK
//
//
******************************************************************************/
#include "msp430x26x.h"

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // 停看门狗
  P5DIR |= BIT7;                            // P5.7为输出模式
  BCSCTL2 |= DCOR; //+ DIVM_3;                 // 外部电阻，MCLK/8
  BCSCTL1 &= ~(RSEL1 + RSEL0);              // RSEL = 4
  
  P5DIR |= 0x010;                           // P5.4为输出模式
  P5SEL |= 0x010;                           // P5.4为MCLK输出
  __bis_SR_register(OSCOFF );               // 停止XT1

  while(1)                                  
  {
      P5OUT ^= BIT7;                        // LED取反
      for( unsigned int i = 0;i<20000;i++ ) 
          _NOP();                           // LED取反间隔
  }
}
