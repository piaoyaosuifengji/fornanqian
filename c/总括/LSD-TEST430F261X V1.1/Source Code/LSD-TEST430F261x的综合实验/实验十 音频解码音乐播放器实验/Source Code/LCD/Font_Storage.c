/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���:Font_Storage.c
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** �������ֿ�оƬ�Ķ�д���򣬸��ݺ��ֻ��ַ������������ֿ��е���Ե�ַ��
**    ͨ��SPI���߷��ʣ���д��ַ���ٶ������ݷ��뻺�������ⲿ����������ʾ���ݣ�
** ��������:2008��11��
*****************************************************************/
#include "GT23L32S4W.h"
#include "General_File.h"
/****************************************************************
** ��������SPI_Write_Byte                                                     
** ���ܣ���ѭSPI����д��һ�ֽ�����,ͨ�����������������ţ�SI����λ���룬
**       ÿһλ�ڴ���ʱ�ӣ�SCLK�������ر�����
** ����: huanglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void SPI_Write_Byte( const uchar W_data )
{
    uchar Temp_Data = W_data; 
    
    SPI_SO_L;
    for( uchar i = 0;i < 8;i++ )
    {
        SPI_CLK_L;                                              //Ϊ��������׼��
        if( Temp_Data & 0x80 ) 
        {
            SPI_SO_H;                                           //���λ�Ƿ�Ϊ1,��������豸����1
        }
        else 
        {
            SPI_SO_L;                                           //������豸����0
        }
        SPI_Delay();
        SPI_CLK_H;                                              //����ʱ�ӣ�SCLK��������
        SPI_Delay();
        Temp_Data <<= 1;                                        //��һ������λ 
    }
}
/****************************************************************
** ��������SPI_Read_Byte                                                     
** ���ܣ���ѭSPI���߶���һ�ֽ�����,ͨ����������������ţ�SO ����
**       λ�����ÿһλ�ڴ���ʱ�ӣ�SCLK���½��ر��Ƴ�
** ����: huanglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
uchar SPI_Read_Byte( void )
{
    uchar Read_Data = 0;
    uchar Bit_Data = 0;
    
    SI_DIR_IN;
    SPI_CLK_H;                                                  //Ϊ�½�����׼��
    for( uchar i = 0;i < 8;i++ )
    {
        SPI_CLK_L;                                              //����ʱ�ӣ�SCLK���½���
        SPI_Delay();
        if( SPI_Read_SI )                                       //�����豸��SO״̬
        {
            Bit_Data = 1;
        }
        else
        {
            Bit_Data = 0;
        }
        Read_Data = (( Read_Data << 1 )|Bit_Data );             //���������δ���Read_Data
        SPI_CLK_H;
    }
    SI_DIR_OUT;
    return( Read_Data );
}
/****************************************************************
** ��������Calculate_Add                                                   
** ���ܣ������ַ������������ֿ�оƬ�е���Ե�ַ
** ����: huanglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־�� 
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
** ��������Commonly_Read_Data                                                   
** ���ܣ���GT23L324W�ֿ�оƬ�ж���һ���ַ���
** ����: huanglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
uchar *Commonly_Read_Data( const uchar *Inner_Code )
{
    uchar Temp_Addr;
    ulong Real_Addr,SPI_Addr;
    static uchar Ch_Code[32];                                   //���ݻ�����,���16X16����
    uchar *Code_Ptr = Ch_Code;
    
    Real_Addr = Calculate_Add( Inner_Code );                    //��Ե�ַ����
    SPI_Addr = Real_Addr << 8;
    SPI_CS_L;                                                   //CS = 0,��GT23L32S4W
    SPI_Write_Byte( Read_Com );                                 //д��"һ���"ָ��
    for( signed char i = 3;i >= 1;i-- )                         //д��Ҫ��ʾ���ַ���ַ
    {
        Temp_Addr = (uchar)( SPI_Addr >> ( i*8 ));              //��λ��ǰ����λ�ں�
        SPI_Write_Byte( Temp_Addr );
    }
    
    if( Real_Addr > CH_BaseAdd && Real_Addr < ASCII_BaseAdd )   //���ֺ�������ASSCI��
    {
        for( uchar i = 0;i < 32;i++ ) 
        {
            *Code_Ptr++ = SPI_Read_Byte();                      //��������
        }
    }
    else
    {
        for( uchar i = 0;i < 16;i++ ) 
        {
            *Code_Ptr++ = SPI_Read_Byte();                     //��ASCII����
        }
    }
    SPI_CS_H;                                                   //CS=1,��ֹGT23L32S4W
    return( Ch_Code );                                          //�ⲿ�������׵�ַ����
}
