/*******************************************************************************
// ��Ȩ:   ����������Ƽ����޹�˾
// �ļ���:MSP430x261x_ta.c
// �汾��V1.0
// ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
// ����:Huanglongsong
// ������Timer_A��ʾ��TA��ʼ��Ϊ��������ģʽ��CPU����ֹ����TAR������CCR0(50000)ʱ��
// ����һ���ж�(TA_ISR)������P5.7�����ת
//  ACLK = n/a, MCLK = SMCLK = TACLK = default DCO ~1.045MHz
// ��������:2008��11��
//
//           MSP430F261x
//         ---------------
//     /|\|            XIN|-
//      | |               |
//      --|RST        XOUT|-
//        |               |
//        |           P5.7|-->LED
//
//
******************************************************************************/

#include  <msp430x26x.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // ͣ���Ź�
  P5DIR |= BIT7;                            // P5.7Ϊ���ģʽ
  CCTL0 = CCIE;                             // CCR0 �ж�ʹ��
  CCR0 = 50000;
  TACTL = TASSEL_2 + MC_2;                  // SMCLK, ��������ģʽ

  _BIS_SR(LPM0_bits + GIE);                 // ��CPU,ʹ�����ж�
}

// Timer A0 �жϷ����ӳ���
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  P5OUT ^= BIT7;                            // ��תLED״̬
  CCR0 += 50000;                            // ���CCR0ƫ����
}

