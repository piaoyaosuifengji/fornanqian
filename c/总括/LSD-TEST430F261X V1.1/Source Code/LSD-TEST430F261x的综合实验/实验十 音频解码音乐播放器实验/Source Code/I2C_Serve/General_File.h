/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���: General_File.h
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ��������:2008��11��
*****************************************************************/
#ifndef  _General_File_
#define  _General_File_

#include <msp430x26x.h>

#define        uchar    unsigned char
#define        uint     unsigned int
#define        ulong    unsigned long
#define        ushort   unsigned short
#define        True         1          
#define        False        0 
#define        LCDBUS              P7OUT
#define        LCDCOM              P8OUT
#define        CS                  BIT0
#define        RS                  BIT1
#define        WR                  BIT2
#define        RD                  BIT3
#define        RST                 BIT5
#define        SPI_CS_H            P8OUT |= BIT4                            //��λSPI_CS
#define        SPI_CS_L            P8OUT &= ~BIT4                           //��λSPI_CS
#define        SPI_CLK_H           P7OUT |= BIT0                            //��λSPI_CLK
#define        SPI_CLK_L           P7OUT &= ~BIT0                           //��λSCL
#define        SI_DIR_IN           P7DIR &= ~BIT2
#define        SI_DIR_OUT          P7DIR |= BIT2
#define        SPI_Read_SI         ( P7IN & BIT2 )                          //��SPI������
#define        SPI_SO_H            P7OUT |= BIT1                            //��λSPI_SO
#define        SPI_SO_L            P7OUT &= ~BIT1                           //��λSPI_SO

#endif // _General_File_
