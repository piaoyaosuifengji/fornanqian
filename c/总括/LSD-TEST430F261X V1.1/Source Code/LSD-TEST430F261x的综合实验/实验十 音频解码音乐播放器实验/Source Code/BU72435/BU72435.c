/******************************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名: BU72435.C
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 生成日期:2008年12月
**  描述：此文件包含BU72435模块的主程序，用于实现BU72435音频解码基本功能，包括播放、暂
**  停、停止、音效选择、播放设备选择、播放文件夹选择、重复模式选择、随机模式选择音量大
**  小选择等。 BU9435工作在从机、MODE2模式，播放音乐格式为MP3，详情参考DEMO原理图及详
**  细代码设置。
*******************************************************************************/
#include "BU72435.h"
/*****************************************************************
*** 函数名：Write_PKG                                                      
** 功能：写BU27435的控制指令包，长度由指令结构包决定
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
******************************************************************/
void Write_Data_PKG( const Struct_PKG_Type *Ptr_PKG )
{
    uchar PKG_Length = Ptr_PKG -> Data_CNT;
    
    I2C_Start();
    I2C_Senduchar( Write_BU72435 );
    I2C_Write_ACK();
    for( uchar i = 0;i< PKG_Length;i++ )
    {
        I2C_Senduchar( *( Ptr_PKG -> Com_PKG + i ) );
        I2C_Write_ACK();
    }
    I2C_Stop();
}
/*************************************************
** 函数(模块)名称:Key_Process
** 功能: 音乐播放模块操作进程处理，参考BU72435数据手册发送相应指令实现各种功能
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：		
*************************************************/
#pragma vector = PORT1_VECTOR
__interrupt void Key_Process(void)
{
    static uchar Play_Flag = 0;
    static Struct_PKG_Type *Struct_Prt = &SET_EQ_Cmd[1];
    
    switch( KeyScan())
    {
    case K1:
        if( !Play_Flag )            
        {
            Write_Data_PKG( &PlayCmd );         // 播放模式
            Play_Flag = 1;
        }
        else
        {
            Write_Data_PKG( &PauseCmd );        // 暂停模式
            Play_Flag = 0;
        }
        break;   
    case K2:
        Write_Data_PKG( &StopCmd );             // 停止模式
        Play_Flag = 0;
        break;   
    case K3:
        Write_Data_PKG( &ChngDevCmd );          // USB、SD切换
        Play_Flag = 0;
        break;
    case K4:
        Write_Data_PKG( &VolAddCmd );           // 音量增
        break;
    case K5:
        Write_Data_PKG( &FB_PlayCmd );          // 上一首
        while( BU72435_Port & MCHNG );
        Delays(10);
        Write_Data_PKG( &FB_PlayCmd );
        break;
    case K6:
        Write_Data_PKG( &FF_PlayCmd );          // 下一首
        break;
    case K7:
        Write_Data_PKG( &TenAdd_PlayCmd );      // 跳到下十首
        break;
    case K8:
        Write_Data_PKG( &VolDecCmd );           // 音量减
        break;    
    case K9:
        Write_Data_PKG( &FOL_AddPlayCmd );      // 访问下一个文件夹
        break;   
    case K10:
        Write_Data_PKG( &FOL_DecPlayCmd );      // 访问上一个文件夹
        break;   
    case K11:
        Write_Data_PKG( &TenDec_PlayCmd );      // 跳到上十首    
        break;
        
    case K12:
        Write_Data_PKG( Struct_Prt++ );         // 音效切换
        if( Struct_Prt >= SET_EQ_Cmd + EQ_CNT )
        {
            Struct_Prt = SET_EQ_Cmd;
        }
        break;
    default:
        break; 
    }
    P1IFG = 0;
}