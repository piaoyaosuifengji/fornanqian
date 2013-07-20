/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:Sound_System.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：语音实验的液晶界面初始化，按键响应等
** 生成日期:2008年12月
*****************************************************************/
#include "LCD_Graphic.h"
#include "Sound_Head_File.h"
/************************************************************************
函数名：Nothing
入口参数：
出口参数：无
功能：空操作
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Nothing( void )
{
    return;
}
/************************************************************************
函数名：Aduio_Main_Face
入口参数：
出口参数：无
功能：初始化开机后的主菜单,等待按键调用相关功能
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Aduio_Main_Face( void )
{
    ClearRAM();
    Display_String( 22,0, "语音测试系统" );
    Draw_Line( 0,18,240);
    Draw_Line( 0,138,240);
    Display_String( 10,24,"当您按下<录音>,系统通过");
    Display_String( 0,42, "MIC录制语音信息,您可以对着");
    Display_String( 0,60, "MIC说话或唱歌等;当录音完毕");
    Display_String( 0,78, "按下<播放>,系统将录制的语");
    Display_String( 0,96, "音信息播放出来,您必须插上");
    Display_String( 0,116, "耳机才能听到播放内容!");
    Display_String( 0,142, "录音(Left)     播放(Right)");
}
/************************************************************************
函数名：Audio_Play_Face
入口参数：
出口参数：无
功能：初始化播放界面菜单
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Play_Mode( void )
{
    Audio_Menu_Ptr = &Audio_Menu[2];
    ClearRAM();
    Display_String( 22,0,  "语音测试系统" );
    Draw_Line( 0,18,240);
    Draw_Line( 0,138,240);
    Display_String( 10,40, "模式: 播放");
    Display_String( 10,60, "状态: 播放中...");
    Display_String( 0,142, "退出(Left)     暂停(Right)");
    Play();
}
/***********************************************************************
函数名：Record_Mode
入口参数：
出口参数：无
功能：初始化录音界面菜单
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Record_Mode( void )
{ 
    Audio_Menu_Ptr = &Audio_Menu[0];
    ClearRAM();
    Display_String( 22,0, "语音测试系统" );
    Draw_Line( 0,18,240);
    Draw_Line( 0,138,240);
    Display_String( 10,40,"模式: 录音");
    Display_String( 10,60,"状态: 录音中...");
    Display_String( 0,142,"暂停(Left)     退出(Right)");
    Record();
}
/***********************************************************************
函数名：Record_Pause
入口参数：
出口参数：无
功能：录音恢复/暂停
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Record_Pause_Resume( void )
{
    static uchar Record_State = 1;
    
    if( Record_State ) 
    {
        Display_String( 10,60,"状态: 暂停中...");
        Display_String( 0,142, "恢复(Left)     退出(Right)");
        Record_State = 0;
        Record_Switch( OFF );                   // 发送关闭录音命令
    }
    else
    {
        Display_String( 10,60,"状态: 录音中...");
        Display_String( 0,142, "暂停(Left)     退出(Right)");
        Record_State = 1;
        Record_Switch( ON );                    // 发送打开录音命令
    }
}
/***********************************************************************
函数名：Play_Pause
入口参数：
出口参数：无
功能：播放恢复/暂停
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Play_Pause_Resume( void )
{
    static uchar Play_State = 1;
    
    if( Play_State ) 
    {
        Display_String( 10,60, "状态: 暂停中...");
        Display_String( 0,142, "退出(Left)     恢复(Right)");
        Play_State = 0;
        Play_Switch( OFF );
    }
    else
    {
        Display_String( 10,60, "状态: 播放中...");
        Display_String( 0,142, "退出(Left)     暂停(Right)");
        Play_State = 1;
        Play_Switch( ON );
    }
}
/***********************************************************************
函数名：Auido_Exit
入口参数：
出口参数：无
功能：退出录音/播放
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Audio_Exit( void )
{
    Audio_Menu_Ptr = &Audio_Menu[1];
    Aduio_Main_Face();
    Exit_Record();
    Exit_Play();  
}
/*************************************************************************
函数名：main
入口参数：
出口参数：无
功能：主函数入口
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void main( void )
{
    Init_MCU();
    InitLCD();
    ShowBMP( 0,0,240,160 ,Lierda_Logo );             //公司产品 LOGO
    _EINT();
    OFF_CPU();
    Aduio_Main_Face();   
    do{
        switch( Scan_Key() )
        {   
        case UP_KEY:
            Audio_Menu_Ptr -> Up();
            break;
        case DOWN_KEY:
            Audio_Menu_Ptr -> Down();
            break;
        case LEFT_KEY:
            Audio_Menu_Ptr -> Left();
            break;
        case RIGHT_KEY:
            Audio_Menu_Ptr -> Right();
            break;
        default:
            break;
        };
    }while(1);
    
}
