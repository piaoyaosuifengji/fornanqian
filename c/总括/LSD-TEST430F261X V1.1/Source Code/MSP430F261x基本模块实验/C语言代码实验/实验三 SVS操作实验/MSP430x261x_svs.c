/*******************************************************************************
// ��Ȩ:   ����������Ƽ����޹�˾
// �ļ���:MSP430x261x_SVS.c
// �汾��V1.0
// ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
// ����:Huanglongsong
// ������SVSģ���������ʾ��ͨ��SVS���VCC,��VCC����2.5Vʱ��MSP430�ڲ�����P5.7
//      ��ת��˸����VCC����2.5Vʱ����SVS����һ����λ�ź�(POR)��λMSP430!
//  ACLK= n/a, MCLK= SMCLK= default DCO ~ 1.045MHz
// ��������:2008��11��
//
//                MSP430F261x
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P5.7|-->LED
//
//
******************************************************************************/

#include  <msp430x26x.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // ͣ���Ź�
  P5DIR |= BIT7;                            // P5.7Ϊ���ģʽ
  SVSCTL = 0x60 + PORON;                    // SVS��λʹ��@ 2.5V

  for (;;)
  {
    volatile unsigned int i;
    i = 50000;                              // ��ʱ
    do 
    (i--);while (i != 0);
    P5OUT ^= BIT7;                          // VCC����ʱP5.7�˿��ϵ�LED��˸
  }
}
