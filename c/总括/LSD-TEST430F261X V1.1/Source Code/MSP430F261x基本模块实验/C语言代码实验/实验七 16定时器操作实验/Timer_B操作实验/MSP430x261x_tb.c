/*******************************************************************************
// ��Ȩ:   ����������Ƽ����޹�˾
// �ļ���:MSP430x261x_tb.c
// �汾��V1.0
// ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
// ����:Huanglongsong
// ������Timer_B��ʾ��TB��ʼ��Ϊ��������ģʽ��CPU����ֹ��TBR��ʱ������ACLK(32768HZ),
//      ��TBR��50000��ʼ������0xFFFFʱ������һ���жϣ��жϷ����ӳ�����ȡ��һ
//      ��LED״̬��
// ��������:2008��11��
//
//           MSP430F261x
//         ---------------
//     /|\|            XIN|-
//      | |               | 32kHz
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
    TBR = 50000;                              // TBR�Ӵ�ֵ�����Ͽ�ʼ����
    TBCTL = TBSSEL_1 + MC_2 + TBIE;           // TBʱ������ACLK,��������ģʽ�����ж�
    _BIS_SR(LPM3_bits + GIE);                 // ����LPM3,�����ж�
}

// Timer_B7 ��Դ�жϴ���
#pragma vector=TIMERB1_VECTOR
__interrupt void Timer_B(void)
{
    switch( TBIV )
    {
    case  2: break;                          // CCR1 ����
    case  4: break;                          // CCR2 ����
    case 14: P5OUT ^= BIT7;                  // TBR�������
    break;
    }
    TBR = 50000;                             // ��ʼ��������ֵ
    TBCTL &= ~TBIFG;
}

