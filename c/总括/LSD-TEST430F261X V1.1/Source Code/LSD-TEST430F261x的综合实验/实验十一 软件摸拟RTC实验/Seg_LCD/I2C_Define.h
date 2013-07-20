/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���:Segment_Lcd.c
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ��������:2008��11��
*****************************************************************/
#ifndef _I2C_Define_
#define _I2C_Define_

#include <msp430x26x.h>
#include "General_File.h"

#define       SDA_1              P8OUT |= BIT4                             //SDA = 1
#define       SDA_0              P8OUT &= ~BIT4                           //SDA = 0
#define       SCL_1              P8OUT |= BIT3                             //SCL = 1
#define       SCL_0              P8OUT &= ~BIT3                            //SCL = 0
#define       Power_ON           P4OUT |= BIT4                             //WP = 1
#define       Power_OFF          P4OUT &= ~BIT4                            //WP = 0
#define       DIR_IN             P8DIR &= ~BIT4                            //I/O ��Ϊ����
#define       DIR_OUT            P8DIR |= BIT4                             //SDA �������
#define       SDA_IN             ( P8IN & BIT4 )                          //��P8IN��BIT4 
#define       I2C_Delay();   { _NOP();_NOP();_NOP();}
              //_NOP();_NOP();_NOP();_NOP();}

#endif //_I2C_Header_


