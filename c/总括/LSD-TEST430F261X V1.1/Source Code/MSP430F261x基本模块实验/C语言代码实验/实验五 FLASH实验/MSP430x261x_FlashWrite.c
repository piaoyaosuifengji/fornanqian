/*******************************************************************************
// ��Ȩ:   ����������Ƽ����޹�˾
// �ļ���:MSP430x261_flshwrite.c
// �汾��V1.0
// ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
// ����:Huanglongsong
// ������MSP430x26x���߱����ʾ���ӶΣø��Ƶ��Σģ�
//     �����Ȳ����Σ�,�ٽ�������ֵд�룬Ȼ���ٲ����Σģ����Σø��Ƶ��Σ�
//       �Σõ���ʼ��ַ��0x1040
//       �Σĵ���ʼ��ַ��0x1000
// ACLK = n/a, MCLK = SMCLK = CALxxx_1MHZ = 1MHz
//
// ��������:2008��11��
//
//            MSP430F261x/241x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//
******************************************************************************/
#include "msp430x26x.h"


char value;                                 // �ֽ��ͱ�������

// ����ԭ������
void write_SegC(char value);
void copy_C2D(void);

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // ֹͣ���Ź�

  P1DIR |= 0x10;                            // P1.4 ���ģʽ
  P1SEL |= 0x10;                            // P1.4 SMCLK ���
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 ��ʱ����ΪFlash���ڲ�ʱ��
  value = 0;                                // ��ʼ������

  while(1)                                 
  {
    write_SegC(value++);                    // дֵ���Σ�
    copy_C2D();                             // ���ƶ�C����D
    __no_operation();                       // ����ʱ�ڴ˴����öϵ�
  }
}
// �Ȳ�����д��
void write_SegC(char value)
{
  char *Flash_ptr;                          // Flashָ��
  unsigned int i;

  Flash_ptr = (char *)0x1040;               // ��ʼ��ָ�룬ָ��0x1040
  FCTL3 = FWKEY;                            // ����
  FCTL1 = FWKEY + ERASE;                    // ����������
  *Flash_ptr = 0;                           // һ�ο�д�𶯲���C��

  FCTL1 = FWKEY + WRT;                      // �ֽ�д��������

  for (i = 0; i < 64; i++)
  {
    *Flash_ptr++ = value;                   // дֵ��Flash
  }

  FCTL1 = FWKEY;                            // �ֽ�д���ܽ�ֹ
  FCTL3 = FWKEY + LOCK;                     // ��������FLash
}

void copy_C2D(void)
{
  char *Flash_ptrC;                         // C�ε�ָ��
  char *Flash_ptrD;                         // D�ε�ָ��
  unsigned int i;

  Flash_ptrC = (char *)0x1040;              // ��ʼ��C�ε�ָ��
  Flash_ptrD = (char *)0x1000;              // ��ʼ��D�ε�ָ��
  FCTL3 = FWKEY;                            // ����
  FCTL1 = FWKEY + ERASE;                    // ����������
  *Flash_ptrD = 0;                          //  һ�ο�д�𶯲���D��
  FCTL1 = FWKEY + WRT;                      // �ֽ�д��������

  for (i = 0; i < 64; i++)
  {
    *Flash_ptrD++ = *Flash_ptrC++;          // �ӣöΰ�ֵ���Ƶ��Ķ�
  }

  FCTL1 = FWKEY;                            // �ֽ�д���ܽ�ֹ
  FCTL3 = FWKEY + LOCK;                     // ��������FLash
}

