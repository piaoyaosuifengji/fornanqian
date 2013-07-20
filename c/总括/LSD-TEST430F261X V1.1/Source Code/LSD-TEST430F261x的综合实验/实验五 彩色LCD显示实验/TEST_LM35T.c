/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:TEST_LM35T.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 生成日期:2008年12月
** 描述：测试LM35T液晶屏,在LCD上显示一些图片，文字等信息
**  LCD Module:		  Color TFT 320x240 + TCB8000A
*****************************************************************/
#include "LM35T_Head.h"
#include "Picture.h"

//#define     OPEN_BLINK

/***********************      外部函数调用    ***********************************/
extern void Init_MCU( void );
extern void InitLCDM( void );
extern void ShowSingleBMP(ulong X, ulong Y,const uchar *Pic);
extern void ShowBMP160(ulong X, ulong Y,const uchar *Pic);
extern void PrintGB(uint X, uint Y, uchar *pstr);
extern void OFF_CPU( void );
extern void OPEN_ISR( void );
extern void SetFontBgColor(uint color);
extern void SetFontFgColor(uint color);
/*******************************************************************************/
const uint Font_FGColor[] = {  RED,GREEN,BLUE,YELLOW,CYAN,MAGENTA,BLACK };
const uint *Font_Color = Font_FGColor;
/*********************************************************************************
函数名：main
入口参数：
出口参数：无
功能：主程序
代码编制：黄龙松
日期：2008年12月
**********************************************************************************/
void main( void )
{
    Init_MCU(); 
    InitLCDM();
    ShowSingleBMP( 0,0,Lierda );                    // 开机直接显示一幅图片
    
#ifdef   OPEN_BLINK                                 // 示例附加功能：在LCD上显示文本，并更换颜色
    
    SetFontBgColor( WHITE );                        // 文本底色
    do{
        SetFontFgColor( *Font_Color );              // 文本前景色
        PrintGB( 72,5,"杭州利尔达科技有限公司");    // 文本信息
        OPEN_ISR();
        OFF_CPU();
        if( ++Font_Color >( Font_FGColor + sizeof( Font_FGColor ) / sizeof( uint ) ))
        {
            Font_Color = Font_FGColor;
        }
    }while(1);
    
#else
    
    OFF_CPU();
    
#endif
    
}


