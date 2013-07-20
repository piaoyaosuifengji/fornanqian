/*************************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:Horologe.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 生成日期:2008年11月
** 功能:软件模拟RTC功能，实现秒、分、时、日、月、年(闰年)的自动计算！并在段式LCD上显示，
**　　　每隔5秒在日期与时间之间切换，操作说明在点阵式LCD显示
** 相关文件:
** 修改日志
*************************************************************************/
#include "RTC_Head_File.h"

uchar Enter_Key = 0;                // 用于参数校正判断
uchar Face_Flag = TIME_FACE;        // 用于界面切换标志
uchar Key_Flag = 0;
uchar Display_Page = 0;
uchar LCD_State = 1;
uchar Month[12]= { 31,0,31,30,31,30,31,31,30,31,30,31 };
/*****************************************************************
函数名：Operation_Help
入口参数：无
出口参数：无
功能:在LCD上显示RTC操作说明
代码编制：黄龙松
日期：2008年12月
******************************************************************/
void Operation_Help( void )
{
    ClearRAM();
    Display_String( 18,0, "RTC按键功能说明" );
    Draw_Line( 0,18,240);
    Display_String( 0,24,"Up:切换时间与日期显示");
    Display_String( 0,42, "Down:切换时间与日期显示");
    Display_String( 0,60, "Enter:参数校准,按<Up/Down>");
    Display_String( 0,78, "键增减待调整值,当按<Enter>");
    Display_String( 0,96, "时,当前页面被调值闪烁,连续");
    Display_String( 0,116,"按将切换对像,全部按完自动");
    Display_String( 0,134, "退出参数校准!");
}

/*****************************************************************
函数名：void Delays(uint m)
入口参数：无
出口参数：无
功能:延时子程序
代码编制：黄龙松
日期：2008年12月
******************************************************************/
void Delays(uint m )  
{ 
    uint j; uint i;
    
    for(i=0; i<m; i++)
        for(j=0;j<2000; j++) 
            _NOP();
}
/*************************************************************************
函数名：Display_LCD
入口参数：const uchar Data,const uchar Add
出口参数：无
功能：在对应位上显示单个数字
代码编制：黄龙松
日期：2008年11月
*************************************************************************/
void Display_LCD( const uchar Data,const uchar Add )
{  
    Segment_Bit_Display( Add,Data % 10 );
    Segment_Bit_Display( Add - 1, Data / 10 );
}//End Display_LCD
/*************************************************************************
函数名：Display_NULL
入口参数：const uchar Add
出口参数：无
功能：在对应位上显示空白
代码编制：黄龙松
日期：2008年11月
*************************************************************************/
void Display_NULL( const uchar Add )
{  
    Segment_Bit_NULL( Add);
    Segment_Bit_NULL( Add - 1 );
}//End Display_LCD
/*************************************************************************
函数名：Find_Leap_year
入口参数：const uchar Year
出口参数：无
功能：由年份算出是闰年还是平年
代码编制：黄龙松
日期：2008年11月
*************************************************************************/
uchar Find_Leap_year( const uchar Year )
{
    uint Real_year = 2000 + Year;
    
    return (( Real_year%4 == 0 && Real_year%100 != 0 ) || Real_year%400 == 0 )?1:0;
}
/*************************************************************************
函数名：Real_Time
入口参数：
出口参数：无
功能：RTC，时钟计算，每隔一秒更新各时钟量，由WDT_Timer中断调用
代码编制：黄龙松
日期：2008年11月
*************************************************************************/
void Real_Time( void )
{
    Str_Calen *P_RTC = &RTC;
    if(  ++P_RTC -> Time_Type.Sec > 59 )
    {
        P_RTC -> Time_Type.Sec = 0;
        if( ++P_RTC -> Time_Type.Min > 59 )
        {
            P_RTC -> Time_Type.Min = 0;
            if( ++P_RTC -> Time_Type.Hou > 23 )
            {
                P_RTC -> Time_Type.Hou = 0; 
                if( Find_Leap_year( P_RTC -> Date_Type.Yea ))
                    Month[ Feb ] = 29;
                else
                    Month[ Feb ] = 28; 
                if( ++P_RTC -> Date_Type.Dat > Month[ P_RTC -> Date_Type.Mon - 1 ] )
                {
                    P_RTC -> Date_Type.Dat = 1;
                    if( ++P_RTC -> Date_Type.Mon > 12 )
                    {
                        P_RTC -> Date_Type.Mon = 1;
                        P_RTC -> Date_Type.Yea++;
                    }
                }
            }
        }
    }
}
/*************************************************************************
函数名：Display_Time
入口参数：
出口参数：无
功能：在LCD上显示时间格式
代码编制：黄龙松
日期：2008年11月
*************************************************************************/
void Display_Time( void )
{
    Str_Calen *P_RTC = &RTC;
    Segment_Bit_Display( 6,0x0B );               //时间格式初始化
    Display_LCD( P_RTC -> Time_Type.Sec,5);
    Display_LCD( P_RTC -> Time_Type.Min,3);
    Display_LCD( P_RTC -> Time_Type.Hou,1);
}
/*************************************************************************
函数名：Display_Date
入口参数：
出口参数：无
功能：在LCD上显示日期格式
代码编制：黄龙松
日期：2008年11月
*************************************************************************/
void Display_Date( void )
{
    Str_Calen *P_RTC = &RTC;
    Segment_Bit_NULL( 6 );
    Display_LCD( P_RTC -> Date_Type.Dat,5);
    Display_LCD( P_RTC -> Date_Type.Mon,3);
    Display_LCD( P_RTC -> Date_Type.Yea,1);
}
/*************************************************************************
函数名：Display_Object
入口参数：
出口参数：无
功能：在给定的时间内周期性的显示时间、日期、闹钟
代码编制：黄龙松
日期：2008年11月
*************************************************************************/
void Display_Object( void )
{ 
    if( Face_Flag == DATE_FACE )
        Display_Date();
    else
        Display_Time();
}
/*************************************************************************
函数名：main
入口参数：无
出口参数：无
功能：主程序入口
代码编制：黄龙松
日期：2008年11月
*************************************************************************/  
void main( void )
{
    System_Init(); 
    Init_BU9796FS();
    InitLCD();
    Operation_Help();
    _BIS_SR( LPM3_bits + GIE );
    _NOP(); 
}
/*************************************************************************
函数名：Basic_Timer
入口参数：无
出口参数：无
功能：中断入口函数，500ms间隔
代码编制：黄龙松
日期：2008年11月
*************************************************************************/
#pragma vector = WDT_VECTOR
__interrupt void WDT_Timer(void)
{
    static uchar WDT_Face_Flag = 0;
    
    if( ++WDT_Face_Flag > 1 )                 //1S时间到，就计算一次RTC
    {
        Real_Time();
        WDT_Face_Flag = 0;
    }
    if( Key_Flag )
    {
        if( Face_Flag == DATE_FACE )
        {
            switch( Enter_Key )               // 动态闪烁要设置的对象
            {
            case 1 :                          // 闪烁日
                if( WDT_Face_Flag )
                    Display_Date();
                else
                    Display_NULL( 5 ); 
                break;
            case 2 :                          // 闪烁月
                if( WDT_Face_Flag )
                    Display_Date();
                else
                    Display_NULL( 3 ); 
                break; 
            case 3 :                          // 闪烁年
                if( WDT_Face_Flag )
                    Display_Date();
                else
                    Display_NULL( 1 ); 
                break; 
                
            default:
                Face_Flag = DATE_FACE;         // 重复按ENTER_KEY就恢复到日期界面并停闪烁
                Key_Flag = 0;
                Enter_Key = 0;
                break;
            }
        }
        else
        {
            switch( Enter_Key )
            {
            case 1 :                            // 闪烁秒
                if( WDT_Face_Flag )
                    Display_Time();
                else
                    Display_NULL( 5 ); 
                break;
            case 2 :                            // 闪烁分
                if( WDT_Face_Flag )
                    Display_Time();
                else
                    Display_NULL( 3 ); 
                break; 
            case 3 :                            // 闪烁时
                if( WDT_Face_Flag )
                    Display_Time();
                else
                    Display_NULL( 1 );
                break; 
            default:
                Face_Flag = TIME_FACE;          // 重复按ENTER_KEY就恢复到时间界面并停闪烁
                Enter_Key = 0;
                Key_Flag = 0;
                break;
            }
        }
        Display_Page = 0;
    }
    else if( WDT_Face_Flag == 0 )           
    { 
        if( ++Display_Page >= 10 )               // 每隔10秒更换一次页面
        {
            if( Face_Flag == TIME_FACE )
                Face_Flag = DATE_FACE;
            else
                Face_Flag = TIME_FACE;
            Display_Page = 0;
        }
       Display_Object();                     // 间隔一秒更新一次时间值
    }
}
/*************************************************************************
函数名：Key
入口参数：无
出口参数：无
功能：按键中断，响应Enter、Up、Down键功能，直接在这里计算年月日时分秒的校准
代码编制：黄龙松
日期：2008年8月28日
*************************************************************************/
#pragma vector = PORT2_VECTOR
__interrupt void Key(void)
{
    Str_Calen *P_RTC = &RTC;
    
    KEY_IE = 0;
    _BIS_SR( GIE );
    switch( KEY_IFG & 0x13 )
    {
    case ENTER:                                   //ENTER键
        Delays(20);
        if( ~KEY_IN & ENTER )
        {
            Enter_Key++;
            Key_Flag = 1;
        }
        break;
    case DOWN:                                    //DOWN键
        Delays(20);
        if( ~KEY_IN & DOWN )
        {
            if( Key_Flag )
            {
                if( Face_Flag == DATE_FACE )
                {
                    switch( Enter_Key )
                    {
                    case 1 :
                        if( --P_RTC -> Date_Type.Dat < 1 )
                        {
                            if( --P_RTC -> Date_Type.Mon < 1 )
                            {
                                P_RTC -> Date_Type.Mon = 12;
                                P_RTC -> Date_Type.Yea--;
                            }
                            P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon - 1 ]; 
                        }
                        break; 
                    case 2:
                        P_RTC -> Date_Type.Mon--; 
                        if( P_RTC -> Date_Type.Mon < 1 )
                        {
                            P_RTC -> Date_Type.Mon = 12;
                            P_RTC -> Date_Type.Yea --;   
                        }
                        if( P_RTC -> Date_Type.Dat > Month[ P_RTC -> Date_Type.Mon -1 ])
                            P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon -1 ];
                        break; 
                    case 3:
                        P_RTC -> Date_Type.Yea--;
                        if( Find_Leap_year( P_RTC -> Date_Type.Yea ))
                        {
                            Month[ Feb ] = 29;
                        }
                        else
                        {
                            Month[ Feb ] = 28;
                        }
                        if( P_RTC -> Date_Type.Dat > Month[ P_RTC -> Date_Type.Mon -1 ])
                            P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon -1 ];
                        if( P_RTC -> Date_Type.Yea > 99 )
                            P_RTC -> Date_Type.Yea = 99;
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    switch( Enter_Key )
                    {
                    case 1 :
                        if( --P_RTC -> Time_Type.Sec > 59 )
                        {
                            P_RTC -> Time_Type.Sec = 59;
                            if( --P_RTC -> Time_Type.Min > 59 )
                            {
                                P_RTC -> Time_Type.Min  = 59;
                                if( --P_RTC -> Time_Type.Hou > 23 )
                                {
                                    P_RTC -> Time_Type.Hou = 23;
                                    P_RTC -> Date_Type.Dat--;
                                }
                            }
                        } 
                        break; 
                    case 2:
                        if( --P_RTC -> Time_Type.Min > 59 )
                        {
                            P_RTC -> Time_Type.Min = 59;
                            if( --P_RTC -> Time_Type.Hou > 23 )
                            {
                                P_RTC -> Time_Type.Hou  = 23;
                                if( --P_RTC -> Date_Type.Dat < 1 )
                                {
                                    P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon-1 ];
                                    if( --P_RTC -> Date_Type.Mon < 1 )
                                    {
                                        P_RTC -> Date_Type.Mon = 12;
                                        P_RTC -> Date_Type.Yea--;
                                    }
                                }
                            }
                        }
                        break; 
                    case 3:
                        if( --P_RTC -> Time_Type.Hou > 23 )
                        {
                            P_RTC -> Time_Type.Hou = 23;
                            if( --P_RTC -> Date_Type.Dat < 1 )
                            {
                                P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon-1 ];
                                if( --P_RTC -> Date_Type.Mon < 1 )
                                {
                                    P_RTC -> Date_Type.Mon = 12;
                                    P_RTC -> Date_Type.Yea--;
                                }
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                if( Find_Leap_year( P_RTC -> Date_Type.Yea ))         // 闰年查找
                {
                    Month[ Feb ] = 29;
                }
                else
                {
                    Month[ Feb ] = 28;
                }
            }
            else                                            // 手动切换显示界面
            {
                if( Face_Flag == TIME_FACE )
                {
                    Face_Flag = DATE_FACE;
                }
                else
                {
                    Face_Flag = TIME_FACE;
                }
                Display_Page = 0;
            }
            break;
        }
    case UP:                                     //INC
        Delays(20);
        if( ~KEY_IN & UP )
        {
            if( Key_Flag )
            {
                if( Face_Flag == DATE_FACE )
                {
                    switch( Enter_Key )
                    {
                    case 1 :
                        if( ++P_RTC -> Date_Type.Dat > Month[ P_RTC -> Date_Type.Mon-1 ] )
                        {
                            P_RTC -> Date_Type.Dat = 1;
                            if( ++P_RTC -> Date_Type.Mon > 12 )
                            {
                                P_RTC -> Date_Type.Mon = 1;
                                P_RTC -> Date_Type.Yea++;
                            }
                        }
                        break; 
                    case 2:
                        P_RTC -> Date_Type.Mon++; 
                        if( P_RTC -> Date_Type.Mon > 12 )
                        {
                            P_RTC -> Date_Type.Mon = 1;
                            P_RTC -> Date_Type.Yea++;
                        }
                        if( P_RTC -> Date_Type.Dat > Month[ P_RTC -> Date_Type.Mon -1 ])
                            P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon -1 ];
                        break; 
                    case 3:
                        P_RTC -> Date_Type.Yea++;
                        if( Find_Leap_year( P_RTC -> Date_Type.Yea ))
                            Month[ Feb ] = 29;
                        else
                            Month[ Feb ] = 28;
                        if( P_RTC -> Date_Type.Dat > Month[ P_RTC -> Date_Type.Mon -1 ])
                            P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon -1 ];
                        if( P_RTC -> Date_Type.Yea > 99 )
                            P_RTC -> Date_Type.Yea = 0;
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    switch( Enter_Key )
                    {
                    case 1 :
                        if( ++P_RTC -> Time_Type.Sec > 59 )
                        {
                            P_RTC -> Time_Type.Sec = 0;
                            if( ++P_RTC -> Time_Type.Min > 59 )
                            {
                                P_RTC -> Time_Type.Min = 0;
                                if( ++P_RTC -> Time_Type.Hou > 23 )
                                {
                                    P_RTC -> Time_Type.Hou = 0;
                                    P_RTC -> Date_Type.Dat++;
                                }
                            }
                        }
                        break; 
                    case 2:
                        if( ++P_RTC -> Time_Type.Min > 59 )
                        {
                            P_RTC -> Time_Type.Min = 0;
                            if( ++P_RTC -> Time_Type.Hou > 23 )
                            {
                                P_RTC -> Time_Type.Hou = 0;
                                P_RTC -> Date_Type.Dat++; 
                            }
                        }
                        break; 
                    case 3:
                        if( ++P_RTC -> Time_Type.Hou > 23 )
                        {
                            P_RTC -> Time_Type.Hou = 0;
                            P_RTC -> Date_Type.Dat++;
                        }
                        break;
                    default:
                        break;
                    }
                } 
            }
            else                                            // 手动切换显示界面
            {
                if( Face_Flag == TIME_FACE )
                {
                    Face_Flag = DATE_FACE;
                }
                else
                {
                    Face_Flag = TIME_FACE;
                }
                Display_Page = 0;
            }
            break;   
        default:
            break;
        }
    }
    LED_OUT |= LED_BIT;                          //按键指示
    while( ~KEY_IN & 0x13 );                     //等待按键释放
    LED_OUT &= ~LED_BIT;
    KEY_IFG = 0;
    KEY_IE |= UP + DOWN + ENTER;
}
