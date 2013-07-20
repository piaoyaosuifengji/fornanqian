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
    0x10,          //. ���Ҫ��ʾС���㣬����Ҫ����ֵ����һλֵ���
    0x88           //: ,����LCD�ϵ�����":"
};

const uchar Disp_Data[] = { 0,1,2,3,4,5 };
/*****************************************************************
** ��������Segment_Display                                                  
** ���ܣ���ʽLCD���ݰ�д�������򣬸���һ���ַ��͵���ʽLCD��ȥ��ʾ
** ����: Huanglongsong
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
    I2C_Senduchar( Base_Add + User_Addr * 2 );   //������ʼ��ַ����һ��������������
    I2C_Write_ACK();
    for( uchar i = Length ;i > 0;i-- )
    {     
        if( *P_Data != 0x0A )                    // �Դ����Ƿ���С���㣿����У��ͽ�С������ֵ����һλ��ֵ���
        {
            I2C_Senduchar( Num_Code[ *P_Data++ ] );
        }
        else
        {
            uchar Temp_Disp_Data = Num_Code[ *P_Data++ ];
            I2C_Senduchar( Temp_Disp_Data + Num_Code[ *P_Data++ ]);
            i--;
        }
        I2C_Write_ACK();
        
    }
    I2C_Stop();                                  //���ʽ���
} 
/*****************************************************************
** ��������Init_BU9796FS                                                     
** ���ܣ���ʼ������оƬBU9796����ز���
** ����: Huanglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void Init_BU9796FS( void )
{
    I2C_Start();                             //����BU9796   
    I2C_Senduchar( BU9796_Addr );            //дBU9796�������ַ
    I2C_Write_ACK();                         //�ȴ�ack 
    I2C_Senduchar( Write_Com + Set_Reset);   //������λ
    I2C_Write_ACK();                         //�ȴ�ack
    I2C_Senduchar( Write_Com + Blink_Mode2 );
     I2C_Write_ACK();
    I2C_Senduchar( Write_Com + Display_ON ); //����ʾ
    I2C_Write_ACK();
    I2C_Senduchar( Write_Data + Base_Add );  //������ʼ��ַ����һ��������������
    I2C_Write_ACK();
    for( uchar i = 0;i<10;i++ )              //��LCD��ʾ��
    {
       I2C_Senduchar( 0x00 );
       I2C_Write_ACK();
    }
    I2C_Stop();                              //���ʽ���
}
/*****************************************************************
** ��������Init_MCU                                                    
** ���ܣ���ʼ��MSP430����ز���
** ����: Huanglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void Init_MCU( void )
{
    WDTCTL = WDTPW + WDTHOLD;                  // �ؿ��Ź�
    BCSCTL3 |= XT2S_2;                         // XT2Ƶ�ʷ�Χ����
    BCSCTL1 &= ~XT2OFF;                        // ��XT2����
    do
    {
        IFG1 &= ~OFIFG;                       // ������ʧЧ��־
        BCSCTL3 &= ~XT2OF;                    // ��XT2ʧЧ��־
        for( uint i = 0x47FF; i > 0; i-- );   // �ȴ�XT2Ƶ���ȶ�
    }while (IFG1 & OFIFG);                    // �ⲿʱ��Դ����������
    BCSCTL2 |= SELM_2 + SELS ;                // ����MCLK��SMCLKΪXT2 
    P4OUT &= ~BIT4;
    P4DIR |=  BIT4;                           // ��LCD��ʾ���ֵĵ�Դ
    //P8REN |= BIT3 + BIT4;
    P8DIR |= BIT3 + BIT4;                     // ����MSP430��BU9796��������
    P8OUT |= BIT3 + BIT4;
    P5OUT &= ~BIT7;                           // �����ⲿLED
    P5DIR |= BIT7;
    
}
/*****************************************************************
** ��������main                                                   
** ���ܣ�ϵͳ���������
** ����: Huanglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
******************************************************************/
void main( void )
{
    Init_MCU();
    Init_BU9796FS();
    P5OUT |= BIT7;
    Segment_Display( 0,Disp_Data,6 );
    _BIS_SR( CPUOFF ); 
}


