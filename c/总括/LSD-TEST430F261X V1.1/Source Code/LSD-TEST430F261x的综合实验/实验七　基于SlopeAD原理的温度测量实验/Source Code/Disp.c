
/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:Segment_Lcd.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：遵循I2C总线协议定义的相关函数
** 生成日期:2008年11月
*****************************************************************/
#include <msp430x26x.h>
#include "General_File.h"
#include "I2C_Serve.c"
/************            BU9796FS相关指令定义          **********/

#define     Write_Com            0x80
#define     Write_Data           0x00
#define     Display_ON           0x48
#define     Half_Bias            0x44
#define     Set_Reset            0x6A
#define     Ext_Clock            0x69
#define     Blink_Mode0          0x70
#define     Blink_Mode1          0x71
#define     Blink_Mode2          0x72
#define     Blink_Mode3          0x73
#define     Pixel_ON             0x7E
#define     Pixel_OFF            0x7D
#define     BU9796_Addr          0x7C
#define     Base_Add             0x00

/**************         引用的外部函数      *********************/
extern void I2C_Start(void);
extern void I2C_Stop(void);
extern void I2C_Write_ACK(void);
extern void I2C_Senduchar( uchar Wr_Data );

/**************  定义段式LCD的阿拉伯数字码  *********************/
const uchar Num_Code[] = 
{
    0xAF,          // 0
    0x06,          // 1
    0x6D,          // 2
    0x4F,          // 3
    0xC6,          // 4
    0xCB,          // 5
    0xEB,          // 6
    0x0E,          // 7
    0xEF,          // 8
    0xCF,          // 9
    0x10,          //. 如果要显示小数点，必须要将此值与当前位值相加
    0x88           //: ,包括LCD上的两个":"
};
/*****************************************************************
** 函数名：Segment_Display                                                  
** 功能：初
** 作者: 
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
****************************************************************/ 
void Segment_Display( uchar Addr,const uchar *P_Data,unsigned char cnt_point )
{
    uchar User_Addr = Addr;
    
    I2C_Start();                                 //启动BU9796 
    I2C_Senduchar( BU9796_Addr );                //写BU9796的物理地址
    I2C_Write_ACK();
    I2C_Senduchar( Base_Add + User_Addr * 2 );   //发送起始地址，下一个紧跟的是数据
    I2C_Write_ACK();
    if(Addr==cnt_point)          
        I2C_Senduchar( *P_Data+0x10 );
    else
        I2C_Senduchar( *P_Data);
    I2C_Write_ACK();      
    
    I2C_Stop();                                  //访问结束
} 
/*****************************************************************
** 函数名：Init_BU9796FS                                                     
** 功能：初始化驱动芯片BU9796的相关参数
** 作者: 
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void Init_BU9796FS( void )
{
    I2C_Start();                             //启动BU9796   
    I2C_Senduchar( BU9796_Addr );            //写BU9796的物理地址
    I2C_Write_ACK();                         //等待ack 
    I2C_Senduchar( Write_Com + Set_Reset);   //启动软复位
    I2C_Write_ACK();                         //等待ack
    I2C_Senduchar( Write_Com + Display_ON ); //开显示
    I2C_Write_ACK();
    I2C_Senduchar( Write_Data + Base_Add );  //发送起始地址，下一个紧跟的是数据
    I2C_Write_ACK();
    for( uchar i = 0;i<20;i++ )              //清LCD显示屏
    {
       I2C_Senduchar( 0x00 );
       I2C_Write_ACK();
    }
    I2C_Stop();                              //访问结束
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
    WDTCTL = WDTPW + WDTHOLD;                 // 关看门狗
    BCSCTL3 |= XT2S_2;                        // XT2频率范围设置
    BCSCTL1 &= ~XT2OFF;                       // 打开XT2振荡器
    do
    {
        IFG1 &= ~OFIFG;                       // 清振荡器失效标志
        BCSCTL3 &= ~XT2OF;                    // 清XT2失效标志
        for( uint i = 0x47FF; i > 0; i-- );   // 等待XT2频率稳定
    }while (IFG1 & OFIFG);                    // 外部时钟源正常起动了吗？
    BCSCTL2 |= SELM_2 + SELS ;                // 设置MCLK、SMCLK为XT2 
    P4OUT &= ~BIT4;
    P4DIR |=  BIT4;
    P8DIR |= BIT3 + BIT4;
    P8OUT |= BIT3 + BIT4;
    P5OUT &= ~BIT7;
    P5DIR |= BIT7;
    
}


