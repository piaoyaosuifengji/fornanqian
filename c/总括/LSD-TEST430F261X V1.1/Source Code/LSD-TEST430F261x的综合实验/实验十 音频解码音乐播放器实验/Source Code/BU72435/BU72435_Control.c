/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:BU72435_Control.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：音乐播放机的控制类功能程序，由MSP430控制BU73435的相关工作状态
** 生成日期:2008年12月
*****************************************************************/
#include "General_File.h"
#include "LCD_Graphic.h"

extern void Delays( uint m );
extern void ShowBMP( uchar x,uchar y,uchar width,uchar high,const uchar *bmp );
extern void InitLCD( void );
extern void Draw_Line( const uchar start_x,const uchar start_y,const uchar End_x );
extern void Display_String( uchar x,uchar y, const uchar *String );
extern void ClearRAM( void );

uchar Process_Flag = 0;
uchar Last_Flag = 1;
/****************************************************************
** 函数名：Init_CLK                                                
** 功能：初始化系统时钟
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void Init_CLK( void )
{
    DCOCTL  = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;
}
/****************************************************************
** 函数名：Init_Port                                               
** 功能：MSP430端口初始化
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
******************************************************************/
void Init_Port( void )
{
/***************  BU72435端口初始化 *******************************/                            
    P6OUT &= ~( BIT3 + BIT4 + BIT5 + BIT6 );            // BU72435的A0、A1定义为0
    P6OUT |= BIT2 + BIT7;
    P6DIR |= BIT0 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;   // BU72435的控制端口定义 
    P6REN |= BIT2 + BIT4;    
/***************  液晶接口初始化 *******************************/    
    P4OUT &= ~( BIT4 + BIT3 + BIT6);
    P4SEL |= BIT3;
    P4DIR |= BIT4 + BIT3 + BIT6;
    P7DIR |= 0xFF;
    P7OUT =  0xFF;
    P8DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5;
    P8OUT = 0xFF;
/***************  按键端口初始化 *****************************/
    P1OUT = 0;
    P1DIR |= BIT0 + BIT1 + BIT2 + BIT3;
    P1IFG = 0; 
    P1IES = 0xFF;
    P1IE |= BIT4 + BIT5 + BIT6 + BIT7;
}
/****************************************************************
** 函数名：Init_MCU                                             
** 功能：初始化系统时钟
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void Init_MCU( void )
{
    WDTCTL = WDT_ADLY_250;                 //关看门狗
    IE1 |= WDTIE;
    Init_CLK( );
    Init_Port( );
}
/****************************************************************
** 函数名：main                                            
** 功能：系统入口主程序
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void main( void )
{
    Init_MCU();
    InitLCD();
    ShowBMP( 0,0,240,160 ,Lierda_Logo );             //公司产品 LOGO
    Delays(2000);
    _EINT();
    LPM3;
}
/****************************************************************
** 函数名：main                                            
** 功能：WDT中断服务子程序，定期检测模块是否正确连接
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR( void )
{
    static uchar Sta_Flag = 2;
    uchar Connect_Sta = P3IN & BIT3 ? 1 : 0;
    
    if( Sta_Flag != Connect_Sta )
    {
        if( Connect_Sta )
        {
            ClearRAM();
            Display_String( 25,0, "音乐播放机" );
            Display_String( 0,142,"参考指导书操作音乐播放器" );
            Display_String( 10,50,"无法找到音乐播放器模块,");
            Display_String( 0,68, "请检查硬件连接!"); 
            Draw_Line( 0,18,240);
            Draw_Line( 0,138,240);
            Sta_Flag = Connect_Sta;
        }
        else
        {
            Delays(1000);
            P6OUT &= ~BIT0;                                     // 复位音频解码模块
            ClearRAM();
            P6OUT |= BIT0;
            ShowBMP( 0,0,240,160 ,Music_Sign );
            Display_String( 25,0, "音乐播放机" );
            Display_String( 0,142,"参考指导书操作音乐播放器" );
            Draw_Line( 0,18,240);
            Draw_Line( 0,138,240);
            Sta_Flag = Connect_Sta;
        }
    }
}