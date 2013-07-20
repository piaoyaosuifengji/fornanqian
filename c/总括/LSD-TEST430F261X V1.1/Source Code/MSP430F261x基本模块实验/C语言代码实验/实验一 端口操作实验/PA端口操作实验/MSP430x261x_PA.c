/*******************************************************************************
// ��Ȩ:   ����������Ƽ����޹�˾
// �ļ���:MSP430F261x_PA.c
// �汾��V1.0
// ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
// ����:Huanglongsong
// ������MSP430F261x��ʾ������PA(P7��P8)д��һ���ֵĲ�����ͨ��
//       ���Ź���ʱ����CPU��ÿ����һ�θı�һ��PAOUT��״̬,��
//       LSD_TESTMSP430F261xѧϰ�����ܿ���PA�˿ڵ�LED״̬!
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO
//
//                MSP430F261x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST         PA.0 |-- LED
//            |             |   | |
//            |             |   | |
//            |           PA.15 |-- LED
//
//
//
******************************************************************************/
#include  <msp430x26x.h>

void main(void)
{
    WDTCTL = WDT_ADLY_250;                   // ��ʱWDT 0.5�븴λ
    IE1 |= WDTIE;
    P3OUT &= ~BIT2;   
    P3DIR |= BIT2;                           // ����Դ
    PASEL = 0;                               // ��λPa�ĵڶ����ܣ���ͨ�˿�ģʽ
    PAOUT = 0xAAAA;
    PADIR |= 0xFFFF;                         // PA.x ���
    while(1)
    {
        PAOUT ^= 0x0FFFF;                    // ����PA ȡ�� 
        _BIS_SR( LPM3_bits + GIE );          // ����LPM3;
    }
}

//����ϵͳ
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    __bic_SR_register_on_exit(LPM3_bits);    // �˳�LPM3;
}