/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:TEST_LM240160.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 生成日期:2008年12月
*****************************************************************/
#include "LCD_Graphic.h"
/*******************      外部函数调用   ****************************/
extern void Display_String( uchar x,uchar y, const uchar *String );
extern void Init_MCU( void );
extern void InitLCD( void );
extern void ShowBMP( uchar x,uchar y,uchar width,uchar high,const uchar *bmp );
extern void ClearRAM( void );
extern void OFF_CPU( void );
/*****************************************************************
** 函数名：main                                                   
** 功能：系统入口主函数,调用外部子程序，在LCD上显示利尔达的图标与
        LSD_TESTMSP430F261x的相关信息
** 作者: HuangLongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
******************************************************************/
void main( void )
{
    Init_MCU();
    InitLCD();
    while(1)
    {
        ShowBMP(0,0,240,160 ,Lierda_Logo);             //公司产品 LOGO
        OFF_CPU();
        ClearRAM();                                    // 清屏
        Display_String(20,20,"MSP430单片机");
        Display_String(16,50,"MSP430F261X系列");
        Display_String(25,70,"综合试验仪");
        Display_String(16,110,"尽心尽力 成就您我");
        Display_String(8,140,"杭州利尔达科技有限公司");
        OFF_CPU();  
    }
}
