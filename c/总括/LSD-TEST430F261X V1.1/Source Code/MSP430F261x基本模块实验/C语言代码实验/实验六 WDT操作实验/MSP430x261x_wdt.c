/*******************************************************************************
// ��Ȩ:   ����������Ƽ����޹�˾
// �ļ���:MSP430F261x_WDT.c
// �汾��V1.0
// ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
// ����:Huanglongsong 
// ������MSP430ϵ��WDTģ��ʵ�����̣�����WDTΪ��ʱ��ģʽ�����1S�ı�һ��Led״̬
//  ACLK = LFXT1/4 = 32768/4, MCLK = SMCLK = default DCO ~1.045MHz
// ��������:2008��11��
//
//                 MSP430F261x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P5.7|-->LED
//
//
******************************************************************************/
#include <msp430x26x.h>

void main(void)
{
  WDTCTL =  WDT_ADLY_250;                   // WDT��ʱ��Դ��ACLK������Ƶ��õ�4HZ��ʱ��
  IE1 |= WDTIE;				    // ��WDT��ʱ���ж�
  P5DIR |= BIT7;                            // ����P5.7Ϊ���ģʽ

  __bis_SR_register(LPM3_bits + GIE);       // ���жϣ�ϵͳ����͹���ģʽ3
}

// ���Ź��жϷ����ӳ���
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
  P5OUT ^= BIT7;                            // ����ȡ��һ��P5.7�����״̬
}

