/*******************************************************************************
// ��Ȩ:   ����������Ƽ����޹�˾
// �ļ���:MSP430F261x_P2.c
// �汾��V1.0
// ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
// ����: Huanglongsong 
// �����������ж�ʵ�����̣���ʼ��P2.5(Enter��)Ϊ����ģʽ���ڲ������������½�����
//       ����Ч��ϵͳ����LPM4,������Enter���󣬽����жϷ����ӳ�ʽ�򣬸ı�һ��LED״̬
// ��������:2008��11��
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//               MSP430F261x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//     /|\   |                 |
//      --o--|P2.5         P5.7|-->LED
//     \|/
//
//
//
******************************************************************************/
#include "msp430x26x.h" 

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // ֹͣ���Ź�
  P5DIR |= BIT7;                            // P5.7Ϊ���ģʽ
  P2REN |= BIT5;                            // �ڲ�����ʹ��
  P2IE  |= BIT5;                            // P2.5 �ж�ʹ��
  P2IES |= BIT5;                            // P2.5 �Ӹߵ��͵ı仯�����ж�
  P2IFG &= ~BIT5;                           // P2.5 IFG����

  __bis_SR_register(LPM4_bits + GIE);       // ����LPM4 �������ж�
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  P5OUT ^= BIT7;                            // P5.7 ȡ��
  P2IFG &= ~BIT5;                          // P2.5 IFG����
}

