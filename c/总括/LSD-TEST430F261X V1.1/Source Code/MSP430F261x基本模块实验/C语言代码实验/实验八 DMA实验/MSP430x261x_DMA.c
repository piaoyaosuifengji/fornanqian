/*******************************************************************************
// ��Ȩ:   ����������Ƽ����޹�˾
// �ļ���:MSP430F261x_DMA.c
// �汾��V1.0
// ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
// ����:Huanglongsong
// ���������� MSP430x26x��DMA��ʾ���̣�ͨ��TA����DMA0��һ���ַ������Ե��ֽ��ظ�
//    ���䵽Ŀ�ĵ�ַP5OUT�Ĵ����У�ÿ����һ�θı�һ��P5OUT�Ĵ�����ֵ��P5.7������
//    ��LED����˸���ֳ��仯״̬��
//   ACLK = 32kHz, MCLK = SMCLK = TACLK = default DCO 1.045MHz
//
//               MSP430F261x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P5.7|--> LED
//            |                 | 
//
//
******************************************************************************/
#include "msp430x26x.h"

const char testconst[2] = { 0x0, 0x80 };

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // ͣ���Ź�
    P5DIR |= BIT7;                            // P5.7���ģʽ
    DMACTL0 = DMA0TSEL_1;                     // TACCR2ΪDMA0�Ĵ���Դ
    DMA0SA = (int)testconst;                  // Դ��ַ��ʼ��
    DMA0DA = (int)&P5OUT;                     // Ŀ�ĵ�ַ��ʼ��
    DMA0SZ = sizeof testconst;                // ���䳤��
    DMA0CTL = DMADT_4 + DMASRCINCR_3 + DMASBDB + DMAEN; // ���ֽ��ظ����䣬Դ��ַ���ӣ�����DMA
    TACTL = TASSEL_2 + MC_2;                  // ��ʼ��TA��ʱ��Դ��SMCLK����������ģʽ
    __bis_SR_register(LPM0_bits + GIE);       // ����͹���ģʽ0( ��CPU)
}
