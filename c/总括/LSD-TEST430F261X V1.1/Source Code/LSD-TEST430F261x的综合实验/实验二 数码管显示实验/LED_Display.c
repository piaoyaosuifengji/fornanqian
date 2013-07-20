/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:LED_Display.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：基于CD74HC595的特性，由MSP430F2619模拟SPI时序，发送待显示数据到
        LED显示屏上，由WDT定时器定时更新显示地址与显示内容！
** 生成日期:2008年12月
*****************************************************************/
#include <msp430x26x.h>
#include "General_File.h"

#define     HC595_DS_H           P7OUT |= BIT1
#define     HC595_DS_L           P7OUT &= ~BIT1
#define     HC595_ST_H           P7OUT |= BIT2
#define     HC595_ST_L           P7OUT &= ~BIT2
#define     HC595_MR_H           P7OUT |= BIT4
#define     HC595_MR_L           P7OUT &= ~BIT4
#define     HC595_SH_H           P7OUT |= BIT3
#define     HC595_SH_L           P7OUT &= ~BIT3
#define     HC595_OE_H           P8OUT |= BIT4
#define     HC595_OE_L           P8OUT &= ~BIT4
#define     Buzz_ON              P3OUT |= BIT0
#define     Buzz_OFF             P3OUT &= ~BIT0
#define     MSB                  0x80
#define     HC595_Delay()        //{_NOP();_NOP();_NOP();\
                                   _NOP();_NOP();_NOP();\
                                   _NOP();_NOP();_NOP();}     
const uchar NumCode[11] = 
{ 
    0x3F,   //0
    0x06,   //1
    0x5B,   //2
    0x4F,   //3
    0x66,   //4
    0x6D,   //5
    0x7D,   //6
    0x07,   //7
    0x7F,   //8
    0x6F,   //9
    0x80    //.
};

uchar Disp_Buff[] = "012345";                      // 待显示的字符串

/****************************************************************
** 函数名：74HC595_Data_Write                                                     
** 功能：遵循SPI总线写入一字节数据,通过串行数据输入引脚（SI）移位输入，
**       每一位在串行时钟（SCLK）上升沿被锁存
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void HC595_Data_Write( const uchar W_data )
{
    uchar Temp_Data = W_data; 
    
    for( uchar i = 0;i < 8;i++ )
    {
        HC595_SH_L ;                                     //为上升沿做准备
        if( Temp_Data & MSB ) 
        {
            HC595_DS_H ;                                 //最高位是否为1,是则给从设备输入1
        }
        else 
        {
            HC595_DS_L;                                  //否则从设备输入0
        }
        HC595_Delay();
        HC595_SH_H;                                      //串行时钟（SCLK）上升沿
        Temp_Data <<= 1;                                 //下一个数据位 
    }
}
/****************************************************************
** 函数名：SPI_Write_Byte                                                     
** 功能：遵循SPI总线写入一字节数据,通过串行数据输入引脚（SI）移位输入，
**       每一位在串行时钟（SCLK）上升沿被锁存
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void Send_Data_Disp( const uchar Data,const uchar Addr ) 
{
    HC595_MR_L;                                         
    HC595_Delay();
    HC595_MR_H;                                         // 给74HC595一个有效的RST信号
    HC595_ST_L;                                         // 为ST_CP的上升沿做准备
    HC595_Data_Write( Addr );                           // 写待显示位的地址
    HC595_Data_Write( NumCode[ Data ] );                // 在被选择位上送显示字码
    HC595_ST_H;                                         // 给ST_CP一个有效的上升沿
    HC595_Delay();                                      // 在必要时适量延时
    HC595_ST_L;                                         // 为一下次上升沿做准备
}
/*****************************************************************
** 函数名：Init_MCU                                                    
** 功能：初始化MSP430的相关参数
** 作者: 
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void Init_MCU( void )
{
    WDTCTL = WDTPW + WDTHOLD;                           // 关看门狗  
    BCSCTL3 |= XT2S_2;                                  // XT2频率范围设置
    BCSCTL1 &= ~XT2OFF;                                 // 打开XT2振荡器

    do
    {
        IFG1 &= ~OFIFG;                                 // 清振荡器失效标志
        BCSCTL3 &= ~XT2OF;                              // 清XT2失效标志
        for( uint i = 0x47FF; i > 0; i-- );             // 等待XT2频率稳定
    }while (IFG1 & OFIFG);                              // 外部时钟源正常起动了吗？
    BCSCTL2 |= SELM_2 + SELS ;                          // 设置MCLK、SMCLK为XT2 
    WDTCTL =  WDT_ADLY_1_9;                              // 设置WDT为定时器模式
    P3DIR |= BIT0;
    P4OUT &= ~BIT3;                                     // 开LED电源
    P4DIR |= BIT3;
    P7OUT |=  BIT1 + BIT2 + BIT3 + BIT4;                //打开I2C总线
    P7DIR |=  BIT1 + BIT2 + BIT3+ BIT4;
    P8DIR |= BIT4;
    IE1 |= WDTIE;                                       // 开WDT中断
}
/****************************************************************
** 函数名：Buzzer_Start                                                    
** 功能：产生一个开机声音提示信号,持续时间由形参决定
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void Buzzer_Start( const uchar Time )
{
    Buzz_ON;
    for( uchar i = 0;i < Time;i++ )
    {
        for( uint j= 0;j<50000;j++ );
    }
    Buzz_OFF;
}
/****************************************************************
** 函数名：Buzzer_Start                                                    
** 功能：打开(使能)HC595
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void Open_HC595( void )
{ 
    HC595_OE_L ;      
    HC595_MR_L;
    for( uchar i = 0;i<100;i++ );
    HC595_MR_H;   
}
/*****************************************************************
** 函数名：main                                                   
** 功能：系统入口主函数
** 作者: 
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
******************************************************************/
void main( void )
{                                                                                                                                   
    Init_MCU();
    Open_HC595();
    Buzzer_Start( 20 );
    _BIS_SR( CPUOFF + GIE );
}
/******************************************************************
函数名：WDT_Timer
入口参数：无
出口参数：无
功能：设置WDT为定时器模式，每隔2ms进入一次中断调用，更新一次LED显示
代码编制：黄龙松
日期：2008年9月16日
******************************************************************/
#pragma vector = WDT_VECTOR
__interrupt void WDT_Timer(void)
{
    static uchar led_disp_bit = BIT0;
    static uchar *Disp_Ptr = Disp_Buff;
    
    if( Disp_Ptr >  Disp_Buff + 6 )
    {
        Disp_Ptr = Disp_Buff;
        led_disp_bit = BIT0;
    }
    Send_Data_Disp( *Disp_Ptr++ - 0x30 ,~led_disp_bit );
    led_disp_bit <<= 1;
}