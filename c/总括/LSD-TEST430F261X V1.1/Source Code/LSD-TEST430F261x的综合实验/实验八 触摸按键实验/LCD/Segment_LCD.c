/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���:Segment_Lcd.c
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ��������ѭI2C����Э�鶨�����غ���
** ��������:2008��11��
*****************************************************************/
#include <msp430x26x.h>
#include "General_File.h"

/************            BU9796FS���ָ���          **********/

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

/**************         ���õ��ⲿ����      *********************/
extern void I2C_Start(void);
extern void I2C_Stop(void);
extern void I2C_Write_ACK(void);
extern void I2C_Senduchar( uchar Wr_Data );

/**************  �����ʽLCD�İ�����������  *********************/
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
    0x10,          //. ���Ҫ��ʾС���㣬����Ҫ����ֵ�뵱ǰλֵ���
    0x88,           //: ,����LCD�ϵ�����":"
    0x00
};
/*****************************************************************
** ��������Segment_Display                                                  
** ���ܣ���
** ����: 
** ��������: 2008.11
** ����ļ�:
** �޸���־��
****************************************************************/ 
void Segment_Display( const uchar Addr,const uchar *P_Data, uchar Length )
{
    uchar User_Addr = Addr;
    
    I2C_Start();                                 //����BU9796 
    I2C_Senduchar( BU9796_Addr );                //дBU9796�������ַ
    I2C_Write_ACK();
    I2C_Senduchar( Base_Add + User_Addr * 2 ); //������ʼ��ַ����һ��������������
    I2C_Write_ACK();
    for( uchar i = Length ;i > 0;i-- )
    {
        I2C_Senduchar( *P_Data++ );
        I2C_Write_ACK();
    }
    I2C_Stop();                                  //���ʽ���
} 
/*****************************************************************
** ��������Init_BU9796FS                                                     
** ���ܣ���ʼ������оƬBU9796����ز���
** ����: 
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void init_BU9796FS( void )
{
    I2C_Start();                             //����BU9796   
    I2C_Senduchar( BU9796_Addr );            //дBU9796�������ַ
    I2C_Write_ACK();                         //�ȴ�ack 
    I2C_Senduchar( Write_Com + Set_Reset);   //������λ
    I2C_Write_ACK();                         //�ȴ�ack
    I2C_Senduchar( Write_Com + Display_ON ); //����ʾ
    I2C_Write_ACK();
    I2C_Senduchar( Write_Data + Base_Add ); //������ʼ��ַ����һ��������������
    I2C_Write_ACK();
    for( uchar i = 0;i<10;i++ )               //��LCD��ʾ��
    {
       I2C_Senduchar( 0x00 );
       I2C_Write_ACK();
    }
    I2C_Stop();                              //���ʽ���
}
/*****************************************************************
** ��������Init_MCU                                                    
** ���ܣ���ʼ��MSP430����ز���
** ����: 
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void Init_MCU_LCD( void )
{  
    P4OUT &= ~BIT4;
    P4DIR |=  BIT4;
    P8DIR |= BIT3 + BIT4;
    P8OUT |= BIT3 + BIT4;
    P5OUT &= ~BIT7;
    P5DIR |= BIT7;
}


