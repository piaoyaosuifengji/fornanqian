/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���:LED_Display.c
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ����������CD74HC595�����ԣ���MSP430F2619ģ��SPIʱ�򣬷��ʹ���ʾ���ݵ�
        LED��ʾ���ϣ���WDT��ʱ����ʱ������ʾ��ַ����ʾ���ݣ�
** ��������:2008��12��
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

uchar Disp_Buff[] = "012345";                      // ����ʾ���ַ���

/****************************************************************
** ��������74HC595_Data_Write                                                     
** ���ܣ���ѭSPI����д��һ�ֽ�����,ͨ�����������������ţ�SI����λ���룬
**       ÿһλ�ڴ���ʱ�ӣ�SCLK�������ر�����
** ����: huanglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void HC595_Data_Write( const uchar W_data )
{
    uchar Temp_Data = W_data; 
    
    for( uchar i = 0;i < 8;i++ )
    {
        HC595_SH_L ;                                     //Ϊ��������׼��
        if( Temp_Data & MSB ) 
        {
            HC595_DS_H ;                                 //���λ�Ƿ�Ϊ1,��������豸����1
        }
        else 
        {
            HC595_DS_L;                                  //������豸����0
        }
        HC595_Delay();
        HC595_SH_H;                                      //����ʱ�ӣ�SCLK��������
        Temp_Data <<= 1;                                 //��һ������λ 
    }
}
/****************************************************************
** ��������SPI_Write_Byte                                                     
** ���ܣ���ѭSPI����д��һ�ֽ�����,ͨ�����������������ţ�SI����λ���룬
**       ÿһλ�ڴ���ʱ�ӣ�SCLK�������ر�����
** ����: huanglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void Send_Data_Disp( const uchar Data,const uchar Addr ) 
{
    HC595_MR_L;                                         
    HC595_Delay();
    HC595_MR_H;                                         // ��74HC595һ����Ч��RST�ź�
    HC595_ST_L;                                         // ΪST_CP����������׼��
    HC595_Data_Write( Addr );                           // д����ʾλ�ĵ�ַ
    HC595_Data_Write( NumCode[ Data ] );                // �ڱ�ѡ��λ������ʾ����
    HC595_ST_H;                                         // ��ST_CPһ����Ч��������
    HC595_Delay();                                      // �ڱ�Ҫʱ������ʱ
    HC595_ST_L;                                         // Ϊһ�´���������׼��
}
/*****************************************************************
** ��������Init_MCU                                                    
** ���ܣ���ʼ��MSP430����ز���
** ����: 
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void Init_MCU( void )
{
    WDTCTL = WDTPW + WDTHOLD;                           // �ؿ��Ź�  
    BCSCTL3 |= XT2S_2;                                  // XT2Ƶ�ʷ�Χ����
    BCSCTL1 &= ~XT2OFF;                                 // ��XT2����

    do
    {
        IFG1 &= ~OFIFG;                                 // ������ʧЧ��־
        BCSCTL3 &= ~XT2OF;                              // ��XT2ʧЧ��־
        for( uint i = 0x47FF; i > 0; i-- );             // �ȴ�XT2Ƶ���ȶ�
    }while (IFG1 & OFIFG);                              // �ⲿʱ��Դ����������
    BCSCTL2 |= SELM_2 + SELS ;                          // ����MCLK��SMCLKΪXT2 
    WDTCTL =  WDT_ADLY_1_9;                              // ����WDTΪ��ʱ��ģʽ
    P3DIR |= BIT0;
    P4OUT &= ~BIT3;                                     // ��LED��Դ
    P4DIR |= BIT3;
    P7OUT |=  BIT1 + BIT2 + BIT3 + BIT4;                //��I2C����
    P7DIR |=  BIT1 + BIT2 + BIT3+ BIT4;
    P8DIR |= BIT4;
    IE1 |= WDTIE;                                       // ��WDT�ж�
}
/****************************************************************
** ��������Buzzer_Start                                                    
** ���ܣ�����һ������������ʾ�ź�,����ʱ�����βξ���
** ����: huanglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
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
** ��������Buzzer_Start                                                    
** ���ܣ���(ʹ��)HC595
** ����: huanglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void Open_HC595( void )
{ 
    HC595_OE_L ;      
    HC595_MR_L;
    for( uchar i = 0;i<100;i++ );
    HC595_MR_H;   
}
/*****************************************************************
** ��������main                                                   
** ���ܣ�ϵͳ���������
** ����: 
** ��������: 2008.11
** ����ļ�:
** �޸���־��
******************************************************************/
void main( void )
{                                                                                                                                   
    Init_MCU();
    Open_HC595();
    Buzzer_Start( 20 );
    _BIS_SR( CPUOFF + GIE );
}
/******************************************************************
��������WDT_Timer
��ڲ�������
���ڲ�������
���ܣ�����WDTΪ��ʱ��ģʽ��ÿ��2ms����һ���жϵ��ã�����һ��LED��ʾ
������ƣ�������
���ڣ�2008��9��16��
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