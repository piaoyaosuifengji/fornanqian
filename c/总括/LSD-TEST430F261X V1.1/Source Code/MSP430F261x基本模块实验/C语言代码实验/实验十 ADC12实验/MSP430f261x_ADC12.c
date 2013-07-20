/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430F261x_ADC12.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者:Huanglongsong
// 描述：基于MSP430F261x内部ADC12模块，从A1采集外部模拟量经转换后的值与一基值比较，
//    如果比基值大，LED点亮，反之熄灭！
//
//              MSP430F261x/241x
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//      Vin-->|P6.1/A1      P5.7|--> LED
//          
//
// 生成日期:2008年11月
******************************************************************************/

#include  <msp430x26x.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // 停看门狗
    ADC12CTL0 = SHT0_2 + ADC12ON;             // 设置采样时间，开ADC12，Vref = VACC
    ADC12CTL1 = SHP;                          // 使用定时器采样
    ADC12MCTL0 = INCH_1;                      // 选用A1通道
    ADC12IE = 0x01;                           // 开ADC12MCTL0中断
    ADC12CTL0 |= ENC;                         // 启动转换
    ADC12MCTL0 = INCH_1;
    P5DIR |= BIT7;                            // P5.7输出-LED
    
    for (;;)
    {
        ADC12CTL0 |= ADC12SC;                 // 软件启动转换
        _BIS_SR(CPUOFF + GIE);                // LPM0模式，由ADC12中断唤醒
    }
}

// ADC12 interrupt service routine
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR (void)
{
    if (ADC12MEM0 < 0x7FF)                    // ADC12MEM = A0 > 0.5AVcc?
        P5OUT &= ~BIT7;                       // 复位P5.7，LED灭
    else
        P5OUT |= BIT7;                        // 置位P5.7，LED亮
    _BIC_SR_IRQ(CPUOFF);                      // 退出LMP0
}
