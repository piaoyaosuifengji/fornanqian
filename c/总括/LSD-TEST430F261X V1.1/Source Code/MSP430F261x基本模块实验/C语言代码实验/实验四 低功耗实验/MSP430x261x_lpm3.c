/*******************************************************************************
// ��Ȩ:   ����������Ƽ����޹�˾
// �ļ���:MSP430F261x_lpm3.c
// �汾��V1.0
// ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
// ����:Huanglongsong 
// ������MSP430ϵ�����еĵ͹����������̣���ʼ����ϵͳ�����͹���ģʽ������WDT��ʱ
//      ����ʱ�������CPU,ִ��һ��LED��˸����������͹���ģʽ���������ܶ���ʼ��
//      �����š�
//  ACLK = LFXT1/4 = 32768/4, MCLK = SMCLK = default DCO ~1.045MHz
// ��������:2008��11��
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
    WDTCTL = WDT_ADLY_1000;                   // WDTʱ������ACLK/8,��ʱʱ��4������ж�
    // �ж����ڣ�4sec=1/4Hz(freq)
    IE1 |= WDTIE;                             // �� WDT �ж�
    P1DIR = 0xFF;                             // ���� P1.x ���
    P1OUT = 0;                                // ���� P1.x ��λ
    P2DIR = 0xFF;                             // ���� P2.x ���
    P2OUT = 0;                                // ���� P2.x ��λ
    P3DIR = 0xFF;                             // ���� P3.x ���
    P3OUT = 0;                                // ���� P3.x ��λ
    P4DIR = 0xFF;                             // ���� P4.x ���
    P4OUT = 0;                                // ���� P4.x ��λ
    P5DIR = 0xFF;                             // ���� P5.x ���
    P5OUT = 0;                                // ���� P5.x ��λ
    P6DIR = 0xFF;                             // ���� P6.x ���
    P6OUT = 0;                                // ���� P6.x ��λ
    
    while(1)
    {
        __bis_SR_register(LPM3_bits + GIE);     // ����LPM3, �����ж�
        P5OUT |= BIT7;                          // ��λP5.7 LED ��
        for (i = 20000; i > 0; i--);            // ��ʱ
        P5OUT &= ~BIT7;                         // ��λP5.7 LED ��
    }
}

//����ϵͳ
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    __bic_SR_register_on_exit(LPM3_bits);       // �˳�LPM3;
}
