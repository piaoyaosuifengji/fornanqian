/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���: General_File.h
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ��������:2008��11��
*****************************************************************/
#ifndef  _GT23L32S4W_
#define  _GT23L32S4W_

#include "General_File.h"

#define       Read_Com            0x03
#define       Fast_Read_Com       0x0B
#define       CH_BaseAdd          0x2C9D0
#define       ASCII_BaseAdd       0x1DD780
#define       SPI_Delay()         //{_NOP();_NOP();_NOP();}//\
                                   _NOP();_NOP();_NOP();\
                                   _NOP();_NOP();_NOP();}

#endif // _General_File_