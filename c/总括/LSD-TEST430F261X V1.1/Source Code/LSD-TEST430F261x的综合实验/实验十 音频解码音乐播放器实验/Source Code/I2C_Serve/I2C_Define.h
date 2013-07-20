/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���: I2C_Define.h
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ��������:2008��11��
*****************************************************************/
#ifndef _I2C_Define_
#define _I2C_Define_

#include "General_File.h"

#define       SDA_1              P6OUT |= BIT4                           //SDA = 1
#define       SDA_0              P6OUT &= ~BIT4                          //SDA = 0
#define       SCL_1              P6OUT |= BIT3                           //SCL = 1
#define       SCL_0              P6OUT &= ~BIT3                          //SCL = 0
#define       DIR_IN             P6DIR &= ~BIT4                          //I/O ��Ϊ����
#define       DIR_OUT            P6DIR |= BIT4                           //SDA �������
#define       SDA_IN             ( P6IN & BIT4 )                         //��P8IN��BIT4 
#define       I2C_Delay();       for( uchar i = 0;\
                                  i<50;i++ );


#define       I2C_BUS            P6OUT
#define       SDA                BIT4
#define       SCL                BIT3

#endif //_I2C_Header_


