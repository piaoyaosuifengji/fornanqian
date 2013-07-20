/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:Init_MSP430.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：MSP430相关函数的定义，配置相关模块的工作模式
** 生成日期:2008年11月
*****************************************************************/
#include <msp430x26x.h>
#include "General_File.h"
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
    BCSCTL3 |= XT2S_2;                         // XT2频率范围设置
    BCSCTL1 &= ~XT2OFF;                        // 打开XT2振荡器
    BCSCTL1 |= DIVA_1;                         // ACLK/2
    do
    {
        IFG1 &= ~OFIFG;                        // 清振荡器失效标志
        BCSCTL3 &= ~XT2OF;                     // 清XT2失效标志
        for( uint i = 0x47FF; i > 0; i-- );    // 等待XT2频率稳定
    }while (IFG1 & OFIFG);                     // 外部时钟源正常起动了吗？
    BCSCTL2 |= SELM_2 + SELS + DIVS_2 ;        // 设置MCLK、SMCLK为XT2 
}
/****************************************************************
** 函数名：Init_Port                                               
** 功能：初始化系统时钟
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void Init_Port( void )
{
    P4OUT &= ~( BIT4+BIT3 + BIT6);
    P4SEL |= BIT6;
    P4DIR |= BIT4 + BIT6;
    P7DIR |= 0xFF;
    P7OUT =  0xFF;
    P8DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5;
    P8OUT = 0xFF;
    P5DIR |= BIT7;
    P5OUT |= BIT7;
//    P1SEL |= BIT2;
//    P1DIR |= BIT2;
    P2REN = 0xff;
    P2IFG = 0;
    P2IES |= BIT1 + BIT0;                   //开Key1、Key3
    P2IE |= BIT0 + BIT1;
}
/****************************************************************
** 函数名：Init_Timer_A                                               
** 功能：初始化系统时钟
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void Init_Timer( void )
{
    TACTL = TASSEL_1 + MC_1 + TACLR;        // ACLK,增计数模式
    TBCTL = TBSSEL_2 + MC_1 + TACLR;        // SMCLK,增计数模式 
    TACCTL0 = CCIE;                         // 开TimerA的比较输出0中断
    TACCR0 = 32767;                         // TimerA定时(ACLK=32.768Hz)
    TBCCR0 = 2000;                          // PWM频率设置
    TBCCR6  = 1000;                         // PWM占空比设置
    TBCCTL6 = OUTMOD_6;                     // LCD背光控制
    _BIS_SR( GIE );                         // 开总中断
}
/*****************************************************************
** 函数名：Init_MCU                                             
** 功能：初始化系统时钟
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void Init_MCU( void )
{
    WDTCTL = WDTPW + WDTHOLD;                 // 关看门狗
    Init_CLK();                               // 初始化时钟                  
    Init_Port();                              // 初始化端口
    Init_Timer();                             // 初始化相关定时器
}
/*****************************************************************
** 函数名：OFF_CPU                                           
** 功能：调用本征函数实现关CPU
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void OFF_CPU( void )
{
    _BIS_SR( CPUOFF );
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
/*******************************************************************
函数名：Key
入口参数：无
出口参数：无
功能：按键中断，响应Down、Up功能
代码编制：黄龙松
日期：2008年12月
********************************************************************/
#pragma vector = PORT2_VECTOR
__interrupt void Key(void)
{
    uint  CCR1_PWM = TBCCR6;
    TACCTL0 &= ~CCIE;
    switch( P2IFG & 0x03 )
    {
    case 2:                                   //Down
        Delays(20); 
        if( !( P2IN & BIT1 ) )
        {
            P5OUT |= BIT7;
            if( CCR1_PWM >= 1900 )
            {
                TBCCTL6 = OUTMOD_1; 
                CCR1_PWM = 2000;
            }
            else
            { 
                CCR1_PWM += 100; 
                TBCCTL6 = OUTMOD_6;
            }
        }
        break;
    case 1:                                  //Up
        Delays(20);
        if( !( P2IN & BIT0 ) )
        {
            P5OUT |= BIT7;
            if( CCR1_PWM <= 100 )
            {
                TBCCTL6 = OUTMOD_5; 
                CCR1_PWM = 0;
            }
            else
            { 
                CCR1_PWM -= 100; 
                TBCCTL6 = OUTMOD_6;
            }
        }
        break; 
    default:
        break;
    }
    TBCCR6 = CCR1_PWM;
    while( !( P2IN & BIT0 ) || !( P2IN & BIT1 ));   //等待按键释放
    P2IFG = 0;
    TACCTL0 = CCIE;
    P5OUT &= ~BIT7;
}
/**********************************************************************
函数名：Timer_A 
入口参数：无
出口参数：无
功能：定时唤醒CPU
代码编制：黄龙松
日期：2008年12月
***********************************************************************/
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
    _BIC_SR_IRQ( CPUOFF );
}
