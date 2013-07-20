/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:Sound_Set.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：语音部分设置，由主函数调用相关函数实现录音与播放功能，在播放模式下手动设置音量
** 生成日期:2008年12月
*****************************************************************/
#include <msp430x26x.h>
#include "General_File.h"

uchar Sampleresult = 0;
uint Flash_Data = 0;

void Record( void );
void Play( void );
void Audio_Exit( void );

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
/****************************************************************
** 函数名：Init_ADC12                                                   
** 功能：初始化ADC12模块
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void Init_ADC12( void )
{   
    ADC12IFG = 0;
    ADC12MCTL0 = SREF_0 + INCH_0;                   // 参考电AVcc,通道A0
    ADC12CTL1 |= SHS_1 + CONSEQ1;                   // 扩展采样触发源自TA1
    //ADC12CTL0 |= ADC12ON + MSC + ENC;             // 开ADC12,单通道单次采样模式
    ADC12CTL0 |= ADC12ON + REFON + REF2_5V + ENC;
    for( uint j= 0;j<1000;j++ );                    //等待参考电压稳定
}
/****************************************************************
** 函数名：Init_DAC12                                                   
** 功能：初始化DAC12模块
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void Init_DAC12( void )
{
    DAC12_0CTL = DAC12RES + DAC12IR + DAC12CALON + DAC12AMP_7;       // 输出电压不放大,高速输入输出
}
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
    BCSCTL1 |= DIVA_2;                         // ACLK/4
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
    /***************  音频端口初始化 *******************************/   
    P3DIR |= BIT1;                              
    P3OUT &= ~BIT1;                          // 打开音频电源                         
    P6OUT |= BIT1 + BIT2 + BIT3;
    P6DIR |= BIT1 + BIT2 + BIT3;             // 数字电位器端口     
    /***************  液晶接口初始化 *******************************/    
    P4OUT &= ~( BIT4 + BIT6);
    P4DIR |= BIT4 + BIT6;
    P7DIR |= 0xFF;
    P7OUT =  0xFF;
    P8DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5;
    P8OUT = 0xFF;
    /***************  按键端口初始化 *****************************/
    P5OUT &= ~BIT7;                         // 用于按键指示
    P5DIR |= BIT7;    
    //P2REN = 0xff;       
}
/****************************************************************
** 函数名：Init_Timer_A                                               
** 功能：初始化系统时钟
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void Init_Timer_A( void )
{
    /***********************  语音采样定时  ************************/   
    TACTL = TASSEL_2 + MC_1 + TACLR;         //SMCLK = 3M,增计数模式,清空TAR
    //TACCTL1 = OUTMOD_3;                    //TA1为输出模式3,TAR = CCR1置位,TAR = CCR0复位,用于ADC12/DAC12的触发信号
    TACCR0 = 2000;
    TACCR1 = 1000;                           //CCR0 = 16M/8K = 2000,CCR1 = CCR0/2,TA1输出频率为8K,点空比为0.5
    /***********************  LCD背光  ******************************/
    TBCTL = TBSSEL_2 + MC_1 + TACLR;        // SMCLK,增计数模式 
    TBCCR0 = 8000;                          // PWM频率设置
    TBCCR3  = 2000;                         // PWM占空比设置
    TBCCTL3 = OUTMOD_6;                     // LCD背光控制
}
/*************************************************************************
** 程序名 : EraseSegment擦除子程                                            
** 输入参数 : ulong FarPtr        
** 输出参数 : 无                                                          
** 作用 : 擦除FLASH中的多个段,为下一次写入做好准备
*************************************************************************/
void EraseSegment( ulong FarPtr)               
{
    while (FCTL3 & BUSY );                  // 当前FLASH无操作
    FCTL3 = FWKEY;                          // FLASH解锁                                      // BANK擦设置
    for( uchar i = 0; i< 128;i++ )          // 执行对象为128个段
    {
        while (FCTL3 & BUSY );              // 当前FLASH无操作
        FCTL1 = FWKEY + ERASE;              // 使能段擦除
        *(uchar *)FarPtr = 0;               // 空写    
        FarPtr += 512;                      // 下一个段做准备
    } 
    while (FCTL3 & BUSY );                  // 当前FLASH无操作
    FCTL3 = FWKEY +  LOCK;                  // FLASH锁住
}
/*************************************************************************
函数名：Record_Switch
入口参数：Com_Flag
出口参数：无
功能：停止/暂停ADC12采样，从而达到停止/暂停录音功能
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Record_Switch( uchar Com_Flag ) 
{
    if( Com_Flag )
    {
        ADC12IE |= BIT0;                                     // 启动DMA传输
    }
    else
    {
        ADC12IE &= ~BIT0;                                     // 关闭DMA传输
    }
}
/*************************************************************************
函数名：Record_Mode
入口参数：无
出口参数：无
功能：采集外部的语音量,暂定采样率为8Kb/S,由Timer_A定时触发ADC12
代码编制：黄龙松
日期：2008年12月10日
*************************************************************************/
void Record( void )
{
    TACCTL1 = OUTMOD_3;                                     // TA1为输出模式3,TAR = CCR1置位,TAR = CCR0复位,用于ADC12的触发信号   
    DMACTL0 = DMA0TSEL_0;                                   // 手动方式起动DMA传输
    DMACTL1 |= DMAONFETCH;
    DMA0CTL = DMADT_4 + DMADSTINCR_3 + DMADSTBYTE;          // 单通道多次传输,目的地址增量,字节传输
    DMA0SA = (ulong*)&Sampleresult;                         // 源地址 &sampleresult
    DMA0DA = (ulong*)0x10000; 
    DMA0SZ = 0xFFFF;                                        // 开ADC12MCTL0转换中断
    FCTL2 = FWKEY + FSSEL_2 + FN4 + FN2;                    // 设置时钟为SMCLK/32
    EraseSegment(0x10000);
    FCTL3 = FWKEY;                                          // 解锁
    FCTL1 = FWKEY + WRT;                                    // 启动字节写
    DMA0CTL |= DMAEN;                                       // 启动DMA传输
    ADC12IE |= BIT0;
}
/*************************************************************************
函数名：Stop_Record
入口参数：无
出口参数：恢复/暂停播放，从而达到恢复/暂停播放功能
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Play_Switch( uchar Com_Flag ) 
{
    if( Com_Flag )
    {
        TACCTL1 = CCIE;                                    // 开Timer_A中断,定时启动DMA传输  
        Enabled_TPA301;
    }
    else
    {
        TACCTL1 &= ~CCIE;
        Disabled_TPA301;
    }
}
/*************************************************************************
函数名：Exit_Record
入口参数：无
出口参数：无
功能：退出录音模式
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Exit_Record( void )
{
    ADC12IE &= ~BIT0;
    FCTL1 = FWKEY;                                                
    FCTL3 = FWKEY +  LOCK;
    P5OUT |= BIT7;
}
/*************************************************************************
函数名：Exit_Play
入口参数：无
出口参数：无
功能：退出播放模式
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Exit_Play( void )
{
    DMA0CTL &= ~DMAEN;                                  // 播放数据结束,停止DMA
    TACCTL1 &= ~CCIE;                                   // 关Time_A中断触发
    DAC12_1CTL &= ~DAC12ENC;
    Disabled_TPA301;
}
/*************************************************************************
函数名：Play_Mode
入口参数：无
出口参数：无
功能：将存储器的数据通过DMA传送到DAC12中,暂定播放率为8Kb/S,由Timer_A定时触发DMA
代码编制：黄龙松
日期：2008年12月10日
*************************************************************************/
void Play( void )
{
    //TACTL = TASSEL_2 + MC_1 + TACLR;                    // SMCLK = 16M,增计数模式,清空TAR
    TACCTL1 = CCIE;                                       // 开Timer_A中断,定时启动DMA传输
    DMA0CTL = DMADT_4 + DMASRCINCR_3 + DMASRCBYTE;        // 单通道多次传输,源地址增量,字节传输
    DMA0SA = (ulong*)0x10000;                             // 源地址0x1900;
    DMA0DA = (ulong*)&Flash_Data; 
    DMA0SZ = 0xFFFF;
    DAC12_1CTL |= DAC12ENC;
    DMA0CTL |= DMAEN;
    Enabled_TPA301;
}
/****************************************************************
** 函数名：CAT5110_INC                                               
** 功能：软件调节数字电位器(CAT5110),每执行一次电位器增量
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void CAT5110_INC( void )
{
    //CAT5110_CS_H;
    CAT5110_UD_H;
    CAT5110_CS_L;
    CAT5110_UD_L;
    for( uchar i=0;i<4;i++ )
    {
        CAT5110_UD_H;
        _NOP();
        CAT5110_UD_L;
        Delays( 100 );
    }
    CAT5110_CS_H;
    CAT5110_UD_H;
}
/****************************************************************
** 函数名：CAT5110_DEC                                               
** 功能：软件调节字电位器(CAT5110),每执行一次电位器减量
** 作者: huanglongsong
** 生成日期: 2008.12
** 相关文件:
** 修改日志：
*****************************************************************/
void CAT5110_DEC( void )
{
    //CAT5110_CS_H;
    CAT5110_UD_L;
    CAT5110_CS_L;
    for( uchar i=0;i<4;i++ )
    {
        CAT5110_UD_H;
        _NOP();
        CAT5110_UD_L;
        Delays( 100 );
        
    }
    CAT5110_CS_H;
    CAT5110_UD_H; 
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
    _NOP();
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
    WDTCTL = WDT_ADLY_1000;                       // 定时器模式，ACLK/2,延时2S
    IE1 |= WDTIE;
    Init_CLK();
    Init_ADC12();
    Init_Port();
    Init_Timer_A();
    Init_DAC12();
}
/*******************************************************************
函数名：Key
入口参数：无
出口参数：无
功能：按键中断，响应UP、Down、Left、Right功能
代码编制：黄龙松
日期：2008年12月
********************************************************************/
uchar Scan_Key( void )
{
    uchar Key_Code = 0;
    uchar Key_Flag = ~P2IN & 0x0F;
    
    if(  Key_Flag )
    {
        switch( Key_Flag )
        {
        case 1:                                // UP_KEY
            Delays(20); 
            if( ~P2IN & BIT0 )
            {
                Key_Code = UP_KEY;
            }
            break;  
        case 2:                                // DOWN_KEY
            Delays(20); 
            if( ~P2IN & BIT1 )
            {
                Key_Code = DOWN_KEY;
            }
            break;
        case 4:                               // LEFT_KEY
            Delays(20); 
            if( ~P2IN & BIT2 )
            {
                Key_Code = LEFT_KEY;
            }
            break;  
        case 8:                                //RIGHT_KEY
            Delays(20); 
            if( ~P2IN & BIT3 )
            {
                Key_Code = RIGHT_KEY;
            }
            break;
        default:
            Key_Code = 0;
            break;
        }
        P5OUT |= BIT7;
        while( ~P2IN & 0x0F );                //等待按键释放
        P5OUT &= ~BIT7;
    }
    return ( Key_Code );
}
/*************************************************************************
函数名：ADC12_ISR
入口参数：无
出口参数：无
功能：中断入口函数，采集片外电压
代码编制：黄龙松
日期：2008年9月16日
*************************************************************************/
#define     Sampl_Length            0XFFFF
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{   
    static ulong Count = 0;
    
    if( Count++ > Sampl_Length )                //如果内存已满,应该马上退出录音模式
    { 
        Count = 0;
        Audio_Exit();
    }
    else
    {
        Sampleresult = (uchar)ADC12MEM0;
        DMA0CTL |= DMAREQ;
    } 
}
/**********************************************************************
函数名：WDT_ISR
入口参数：无
出口参数：无
功能：定时唤醒CPU
代码编制：黄龙松
日期：2008年12月
***********************************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    IE1 &= ~WDTIE;
    _BIC_SR_IRQ( CPUOFF );
}
/*************************************************************************
函数名：Timer_A 
入口参数：无
出口参数：无
功能：定时启动DA转换
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A1 (void)
{
    static ulong Length = 0;
    
    switch( TAIV )
    {
    case 2:
        if( Length++ > Sampl_Length )
        {  
            Length = 0;
            Audio_Exit();
        }
        else
        {
            DAC12_0DAT = Flash_Data & 0x00FF;
            DMA0CTL |= DMAREQ;                             // 启动DMA传输
        }
        break;
    default:
        break;
    }
}
