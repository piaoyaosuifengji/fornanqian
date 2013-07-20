/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:LM35T_Head.h
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：关于LM35T液晶模块的相关函数的定义，纯底层液晶驱动，供外部函数调用
** 生成日期:2008年12月
**************************************************************************/
#ifndef     _LM35T_Head_
#define     _LM35T_Head_

#include <msp430X26x.h>
//*******************************************************************

#define     uchar               unsigned char
#define     uint                unsigned int
#define     ulong               unsigned long
#define     ushort              unsigned short
#define     True                1          
#define     False               0

#define     LCDBUS          P7OUT
#define     LCDCOM          P8OUT
#define     A1              BIT1
#define     CS              BIT0
#define     REST            BIT2
#define     Wait            BIT5
#define     RD              BIT4
#define     WR              BIT3

#define	    RED		    0xf800
#define	    GREEN	    0x07e0
#define	    BLUE	    0x001f
#define	    YELLOW	    0xffe0
#define	    CYAN	    0x07ff
#define	    MAGENTA	    0xf81f
#define	    BLACK	    0x0000
#define	    WHITE	    0xffff

#endif