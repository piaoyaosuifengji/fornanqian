/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:I2C_Serve.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：遵循I2C总线协议定义的相关函数
** 生成日期:2008年11月
*****************************************************************/
#include <msp430x26x.h>
#include "I2C_Define.h"
/****************************************************************
** 函数名：I2C_Start                                                       
** 功能：遵循I2C总线协议定义的起动
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void I2C_Start(void)
{
    DIR_OUT;
    SDA_1;
    I2C_Delay();
    SCL_1;
    I2C_Delay();
    SDA_0;
    I2C_Delay();
    SCL_0;
}//End I2C_Start
/*****************************************************************
** 函数名：I2C_Stop                                                      
** 功能：遵循I2C总线协议定义的停止
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void I2C_Stop(void)
{
    DIR_OUT;
    SDA_0;
    I2C_Delay();
    SCL_1;
    I2C_Delay();
    SDA_1;
}//End I2C_Stop
/*****************************************************************
*** 函数名：I2C_ReceiveACK                                                      
** 功能：待接受ACK 信号,完成一次操作
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
******************************************************************/
void I2C_Write_ACK( void )
{
    SDA_1;
    DIR_IN;
    SCL_1;
    I2C_Delay();
    while(SDA_IN );
    SCL_0;
    I2C_Delay();
    DIR_OUT;
    return;
}//End I2C_ReceiveACK
/****************************************************************
** 函数名：2C_Read_Ack                                                      
** 功能：接受数据后发送一个ACK信号
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void I2C_Read_Ack(void)
{
    DIR_OUT;
    SCL_0;
    SDA_0;
    I2C_Delay();
    SCL_1;
    I2C_Delay();
    SCL_0;
    SDA_1;
}//End I2C_Read_Ack
/*****************************************************************
*** 函数名：I2C_Read_NoAck                                                     
** 功能：最后接受数据后发送NoACK信号
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志:
******************************************************************/
void I2C_Read_NoAck( void )
{
    DIR_OUT;
    SCL_0;
    SDA_1;
    I2C_Delay();
    SCL_1;
    I2C_Delay();
    SCL_0;
}//End I2C_Read_Ack
/***************************************************************** 
** 函数名：I2C_Receiveuchar                                                   
** 功能：接受一个字节的数据
** 作者:huanglongsong 
** 生成日期: 2008.11
** 相关文件:
** 修改日志:
*****************************************************************/
uchar I2C_Receiveuchar(void)
{
    uchar Read_Data = 0x00;                            //返回值
    uchar DataBit = 0x00;                              //每一个clk 接受到的数据
    
    SCL_0;
    I2C_Delay();
    SDA_1;
    DIR_IN;
    for( uchar i = 0;i < 8;i++ )
    {
        SCL_1;
        I2C_Delay();
        DataBit = SDA_IN;
        SCL_0;
        I2C_Delay();
        I2C_Delay();
        Read_Data = ( ( Read_Data << 1 ) | DataBit ); //将数据依次存入Read_Data
    }
    return( Read_Data );
}//End I2C_Receiveuchar
/*******************************************************************
** 函数名：I2C_Senduchar                                                    
** 功能：遵循I2C总线协议定义发送一字节数据
** 作者: huanglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志:
*******************************************************************/
void I2C_Senduchar( uchar Wr_Data )
{
    DIR_OUT;
    SCL_0;
    SDA_1;
    for( uchar i = 0;i < 8;i++ )
    {
        if( Wr_Data & 0x80 ) 
        {
            SDA_1;                            //最高位是否为1,为1则SDA= 1
        }
        else 
        {
            SDA_0;                            //否则 SDA=0
        }
        I2C_Delay();
        SCL_1;
        I2C_Delay();
        SCL_0;
        I2C_Delay();
        Wr_Data <<= 1;                        //数据左移一位,进入下一轮送数  
    }
    SDA_1;
    return;
}//End I2C_Senduchar


