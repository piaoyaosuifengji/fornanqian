/*******************************************************************************
// ��Ȩ:   ����������Ƽ����޹�˾
// �ļ���:MSP430F261x_DAC12.c
// �汾��V1.0
// ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
// ����:Huanglongsong
// ����������MSP430F261x�ڲ�DAC12ģ�飬ʹ���ڲ�ADC12REF(2.5V)�ο�Դ������Դ��ѹ�ȶ���
//      ��DAC12_0DATд��һֵ����ת�����DAC0(P6.6)�����(Լ1.0V����)
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
// ����:Huanglongsong
// ��������:2008��11��
******************************************************************************/
#include "msp430x26x.h"

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // ͣ���Ź�
    ADC12CTL0 = REF2_5V + REFON;              // �ڲ��ο���׼��
    TACCR0 = 13600;                           // �ȴ���Դ�ȶ�����ʱ��
    TACCTL0 |= CCIE;                          // ���Ƚ�ģʽ�ж�
    TACTL = TACLR + MC_1 + TASSEL_2;          // ������ģʽ, SMCLK
    __bis_SR_register(LPM0_bits + GIE);       // ����LPM0,�����ж�
    TACCTL0 &= ~CCIE;                         // �ؼ������ж�
    __disable_interrupt();                    // �����ж�
    DAC12_0CTL = DAC12IR + DAC12AMP_5 + DAC12ENC; // ʹ���ڲ�DAC0
    DAC12_0DAT = 0x0666;                      // 1.0V (2.5V = 0x0FFFh)
    __bis_SR_register(LPM0_bits + GIE);       // ��CPU,���ڴ���ģʽ
}

//һֱ�ȵ���Դ�ȶ�����ʱ���ִ��
#pragma vector = TIMERA0_VECTOR
__interrupt void TA0_ISR(void)
{
    TACTL = 0;                                // ��Timer_A��ʱ��ֵ
    __bic_SR_register_on_exit(LPM0_bits);     // �˳��͹���ģʽ0
}



