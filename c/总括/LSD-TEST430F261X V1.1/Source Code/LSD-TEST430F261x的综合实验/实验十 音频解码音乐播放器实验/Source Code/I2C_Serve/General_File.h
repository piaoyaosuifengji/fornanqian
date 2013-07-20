/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名: General_File.h
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 生成日期:2008年11月
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
#define        SPI_CS_H            P8OUT |= BIT4                            //置位SPI_CS
#define        SPI_CS_L            P8OUT &= ~BIT4                           //复位SPI_CS
#define        SPI_CLK_H           P7OUT |= BIT0                            //置位SPI_CLK
#define        SPI_CLK_L           P7OUT &= ~BIT0                           //复位SCL
#define        SI_DIR_IN           P7DIR &= ~BIT2
#define        SI_DIR_OUT          P7DIR |= BIT2
#define        SPI_Read_SI         ( P7IN & BIT2 )                          //读SPI口数据
#define        SPI_SO_H            P7OUT |= BIT1                            //置位SPI_SO
#define        SPI_SO_L            P7OUT &= ~BIT1                           //复位SPI_SO

#endif // _General_File_
