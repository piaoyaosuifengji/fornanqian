/*******************************************************************************
// ��Ȩ:   ����������Ƽ����޹�˾
// �ļ���:MSP430F261x_LED.c
// �汾��V1.0
// ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
// ����:Huanglongsong 
// ������ͨ��������ʾ�������I,����P5.7�˿ڵ���ؼĴ�����ʵ��LED��˸
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
//******************************************************************************/

#include  <msp430x26x.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // �رտ��Ź�
    P5DIR |= BIT7;                            // P5.7������Ϊ��� 

    while (1)
    {
        volatile unsigned int i;
        P5OUT ^= BIT7;                        // P5.7��ת
        i = 50000;                            // ��ʱ
        do (i--);
        while (i != 0);
    }
}
