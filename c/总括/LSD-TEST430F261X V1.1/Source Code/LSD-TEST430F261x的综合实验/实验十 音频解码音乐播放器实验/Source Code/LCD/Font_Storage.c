/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:Font_Storage.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：字库芯片的读写程序，根据汉字或字符的内码计算出字库中的相对地址，
**    通过SPI总线访问，先写地址，再读出数据放入缓存区由外部函数调用显示内容！
** 生成日期:2008年11月
*****************************************************************/
#include "GT23L32S4W.h"
#include "General_File.h"
/****************************************************************
** 函数名：SPI_Write_Byte                                                     
** 功能：遵循SPI总线写入一字节数据,通过串行数据输入引脚（SI）移位输入，
**       每一位在串行时钟（SCLK）上升沿被锁存
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void SPI_Write_Byte( const uchar W_data )
{
    uchar Temp_Data = W_data; 
    
    SPI_SO_L;
    for( uchar i = 0;i < 8;i++ )
    {
        SPI_CLK_L;                                              //为上升沿做准备
        if( Temp_Data & 0x80 ) 
        {
            SPI_SO_H;                                           //最高位是否为1,是则给从设备输入1
        }
        else 
        {
            SPI_SO_L;                                           //否则从设备输入0
        }
        SPI_Delay();
        SPI_CLK_H;                                              //串行时钟（SCLK）上升沿
        SPI_Delay();
        Temp_Data <<= 1;                                        //下一个数据位 
    }
}
/****************************************************************
** 函数名：SPI_Read_Byte                                                     
** 功能：遵循SPI总线读出一字节数据,通过串行数据输出引脚（SO ）移
**       位输出，每一位在串行时钟（SCLK）下降沿被移出
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
uchar SPI_Read_Byte( void )
{
    uchar Read_Data = 0;
    uchar Bit_Data = 0;
    
    SI_DIR_IN;
    SPI_CLK_H;                                                  //为下降沿做准备
    for( uchar i = 0;i < 8;i++ )
    {
        SPI_CLK_L;                                              //串行时钟（SCLK）下降沿
        SPI_Delay();
        if( SPI_Read_SI )                                       //读从设备的SO状态
        {
            Bit_Data = 1;
        }
        else
        {
            Bit_Data = 0;
        }
        Read_Data = (( Read_Data << 1 )|Bit_Data );             //将数据依次存入Read_Data
        SPI_CLK_H;
    }
    SI_DIR_OUT;
    return( Read_Data );
}
/****************************************************************
** 函数名：Calculate_Add                                                   
** 功能：根据字符内码计算出在字库芯片中的相对地址
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志： 
*****************************************************************/
ulong Calculate_Add( const uchar *GBCode )
{
    ulong Address;
    
    if( *GBCode & 0x80 )
    {
        uchar MSB = GBCode[0];
        uchar LSB = GBCode[1];
        
        if( MSB >= 0xA1 && MSB <= 0XA9 && LSB >= 0xA1 ) 
            Address =((ulong)(( MSB - 0xA1) * 94 + (LSB - 0xA1)))*32 + CH_BaseAdd; 
        else if( MSB >= 0xB0 && MSB <= 0xF7 && LSB >= 0xA1 )   
            Address = ((ulong)((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846))*32 + CH_BaseAdd;
    }
    else
    {
        uchar ASCIICode =  *GBCode;
        
        if( ASCIICode >= 0x20 && ASCIICode <= 0x7E )
            Address = ( ASCIICode - 0x20 ) * 16 + ASCII_BaseAdd;
    }
    return( Address );
}
/****************************************************************
** 函数名：Commonly_Read_Data                                                   
** 功能：从GT23L324W字库芯片中读出一组字符码
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
uchar *Commonly_Read_Data( const uchar *Inner_Code )
{
    uchar Temp_Addr;
    ulong Real_Addr,SPI_Addr;
    static uchar Ch_Code[32];                                   //数据缓存区,最大16X16点阵
    uchar *Code_Ptr = Ch_Code;
    
    Real_Addr = Calculate_Add( Inner_Code );                    //相对地址计算
    SPI_Addr = Real_Addr << 8;
    SPI_CS_L;                                                   //CS = 0,打开GT23L32S4W
    SPI_Write_Byte( Read_Com );                                 //写入"一般读"指令
    for( signed char i = 3;i >= 1;i-- )                         //写入要显示的字符地址
    {
        Temp_Addr = (uchar)( SPI_Addr >> ( i*8 ));              //高位在前，低位在后
        SPI_Write_Byte( Temp_Addr );
    }
    
    if( Real_Addr > CH_BaseAdd && Real_Addr < ASCII_BaseAdd )   //区分汉字码与ASSCI码
    {
        for( uchar i = 0;i < 32;i++ ) 
        {
            *Code_Ptr++ = SPI_Read_Byte();                      //读汉字区
        }
    }
    else
    {
        for( uchar i = 0;i < 16;i++ ) 
        {
            *Code_Ptr++ = SPI_Read_Byte();                     //读ASCII码区
        }
    }
    SPI_CS_H;                                                   //CS=1,禁止GT23L32S4W
    return( Ch_Code );                                          //外部函数从首地址访问
}
