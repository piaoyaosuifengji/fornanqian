/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���:BU72435_Control.c
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ���������ֲ��Ż��Ŀ����๦�ܳ�����MSP430����BU73435����ع���״̬
** ��������:2008��12��
*****************************************************************/
#include "General_File.h"
#include "LCD_Graphic.h"

extern void Delays( uint m );
extern void ShowBMP( uchar x,uchar y,uchar width,uchar high,const uchar *bmp );
extern void InitLCD( void );
extern void Draw_Line( const uchar start_x,const uchar start_y,const uchar End_x );
extern void Display_String( uchar x,uchar y, const uchar *String );
extern void ClearRAM( void );

uchar Process_Flag = 0;
uchar Last_Flag = 1;
/****************************************************************
** ��������Init_CLK                                                
** ���ܣ���ʼ��ϵͳʱ��
** ����: huanglongsong
** ��������: 2008.12
** ����ļ�:
** �޸���־��
*****************************************************************/
void Init_CLK( void )
{
    DCOCTL  = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;
}
/****************************************************************
** ��������Init_Port                                               
** ���ܣ�MSP430�˿ڳ�ʼ��
** ����: huanglongsong
** ��������: 2008.12
** ����ļ�:
** �޸���־��
******************************************************************/
void Init_Port( void )
{
/***************  BU72435�˿ڳ�ʼ�� *******************************/                            
    P6OUT &= ~( BIT3 + BIT4 + BIT5 + BIT6 );            // BU72435��A0��A1����Ϊ0
    P6OUT |= BIT2 + BIT7;
    P6DIR |= BIT0 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;   // BU72435�Ŀ��ƶ˿ڶ��� 
    P6REN |= BIT2 + BIT4;    
/***************  Һ���ӿڳ�ʼ�� *******************************/    
    P4OUT &= ~( BIT4 + BIT3 + BIT6);
    P4SEL |= BIT3;
    P4DIR |= BIT4 + BIT3 + BIT6;
    P7DIR |= 0xFF;
    P7OUT =  0xFF;
    P8DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5;
    P8OUT = 0xFF;
/***************  �����˿ڳ�ʼ�� *****************************/
    P1OUT = 0;
    P1DIR |= BIT0 + BIT1 + BIT2 + BIT3;
    P1IFG = 0; 
    P1IES = 0xFF;
    P1IE |= BIT4 + BIT5 + BIT6 + BIT7;
}
/****************************************************************
** ��������Init_MCU                                             
** ���ܣ���ʼ��ϵͳʱ��
** ����: huanglongsong
** ��������: 2008.12
** ����ļ�:
** �޸���־��
*****************************************************************/
void Init_MCU( void )
{
    WDTCTL = WDT_ADLY_250;                 //�ؿ��Ź�
    IE1 |= WDTIE;
    Init_CLK( );
    Init_Port( );
}
/****************************************************************
** ��������main                                            
** ���ܣ�ϵͳ���������
** ����: huanglongsong
** ��������: 2008.12
** ����ļ�:
** �޸���־��
*****************************************************************/
void main( void )
{
    Init_MCU();
    InitLCD();
    ShowBMP( 0,0,240,160 ,Lierda_Logo );             //��˾��Ʒ LOGO
    Delays(2000);
    _EINT();
    LPM3;
}
/****************************************************************
** ��������main                                            
** ���ܣ�WDT�жϷ����ӳ��򣬶��ڼ��ģ���Ƿ���ȷ����
** ����: huanglongsong
** ��������: 2008.12
** ����ļ�:
** �޸���־��
*****************************************************************/
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR( void )
{
    static uchar Sta_Flag = 2;
    uchar Connect_Sta = P3IN & BIT3 ? 1 : 0;
    
    if( Sta_Flag != Connect_Sta )
    {
        if( Connect_Sta )
        {
            ClearRAM();
            Display_String( 25,0, "���ֲ��Ż�" );
            Display_String( 0,142,"�ο�ָ����������ֲ�����" );
            Display_String( 10,50,"�޷��ҵ����ֲ�����ģ��,");
            Display_String( 0,68, "����Ӳ������!"); 
            Draw_Line( 0,18,240);
            Draw_Line( 0,138,240);
            Sta_Flag = Connect_Sta;
        }
        else
        {
            Delays(1000);
            P6OUT &= ~BIT0;                                     // ��λ��Ƶ����ģ��
            ClearRAM();
            P6OUT |= BIT0;
            ShowBMP( 0,0,240,160 ,Music_Sign );
            Display_String( 25,0, "���ֲ��Ż�" );
            Display_String( 0,142,"�ο�ָ����������ֲ�����" );
            Draw_Line( 0,18,240);
            Draw_Line( 0,138,240);
            Sta_Flag = Connect_Sta;
        }
    }
}