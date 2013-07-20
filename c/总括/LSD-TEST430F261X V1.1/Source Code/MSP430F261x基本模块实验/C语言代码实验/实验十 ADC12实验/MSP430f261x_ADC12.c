/*******************************************************************************
// ��Ȩ:   ����������Ƽ����޹�˾
// �ļ���:MSP430F261x_ADC12.c
// �汾��V1.0
// ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
// ����:Huanglongsong
// ����������MSP430F261x�ڲ�ADC12ģ�飬��A1�ɼ��ⲿģ������ת�����ֵ��һ��ֵ�Ƚϣ�
//    ����Ȼ�ֵ��LED��������֮Ϩ��
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
// ��������:2008��11��
******************************************************************************/

#include  <msp430x26x.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // ͣ���Ź�
    ADC12CTL0 = SHT0_2 + ADC12ON;             // ���ò���ʱ�䣬��ADC12��Vref = VACC
    ADC12CTL1 = SHP;                          // ʹ�ö�ʱ������
    ADC12MCTL0 = INCH_1;                      // ѡ��A1ͨ��
    ADC12IE = 0x01;                           // ��ADC12MCTL0�ж�
    ADC12CTL0 |= ENC;                         // ����ת��
    ADC12MCTL0 = INCH_1;
    P5DIR |= BIT7;                            // P5.7���-LED
    
    for (;;)
    {
        ADC12CTL0 |= ADC12SC;                 // �������ת��
        _BIS_SR(CPUOFF + GIE);                // LPM0ģʽ����ADC12�жϻ���
    }
}

// ADC12 interrupt service routine
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR (void)
{
    if (ADC12MEM0 < 0x7FF)                    // ADC12MEM = A0 > 0.5AVcc?
        P5OUT &= ~BIT7;                       // ��λP5.7��LED��
    else
        P5OUT |= BIT7;                        // ��λP5.7��LED��
    _BIC_SR_IRQ(CPUOFF);                      // �˳�LMP0
}
