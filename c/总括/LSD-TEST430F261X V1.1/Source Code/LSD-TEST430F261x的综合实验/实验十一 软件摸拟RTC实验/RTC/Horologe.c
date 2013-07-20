/*************************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���:Horologe.c
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ��������:2008��11��
** ����:���ģ��RTC���ܣ�ʵ���롢�֡�ʱ���ա��¡���(����)���Զ����㣡���ڶ�ʽLCD����ʾ��
**������ÿ��5����������ʱ��֮���л�������˵���ڵ���ʽLCD��ʾ
** ����ļ�:
** �޸���־
*************************************************************************/
#include "RTC_Head_File.h"

uchar Enter_Key = 0;                // ���ڲ���У���ж�
uchar Face_Flag = TIME_FACE;        // ���ڽ����л���־
uchar Key_Flag = 0;
uchar Display_Page = 0;
uchar LCD_State = 1;
uchar Month[12]= { 31,0,31,30,31,30,31,31,30,31,30,31 };
/*****************************************************************
��������Operation_Help
��ڲ�������
���ڲ�������
����:��LCD����ʾRTC����˵��
������ƣ�������
���ڣ�2008��12��
******************************************************************/
void Operation_Help( void )
{
    ClearRAM();
    Display_String( 18,0, "RTC��������˵��" );
    Draw_Line( 0,18,240);
    Display_String( 0,24,"Up:�л�ʱ����������ʾ");
    Display_String( 0,42, "Down:�л�ʱ����������ʾ");
    Display_String( 0,60, "Enter:����У׼,��<Up/Down>");
    Display_String( 0,78, "������������ֵ,����<Enter>");
    Display_String( 0,96, "ʱ,��ǰҳ�汻��ֵ��˸,����");
    Display_String( 0,116,"�����л�����,ȫ�������Զ�");
    Display_String( 0,134, "�˳�����У׼!");
}

/*****************************************************************
��������void Delays(uint m)
��ڲ�������
���ڲ�������
����:��ʱ�ӳ���
������ƣ�������
���ڣ�2008��12��
******************************************************************/
void Delays(uint m )  
{ 
    uint j; uint i;
    
    for(i=0; i<m; i++)
        for(j=0;j<2000; j++) 
            _NOP();
}
/*************************************************************************
��������Display_LCD
��ڲ�����const uchar Data,const uchar Add
���ڲ�������
���ܣ��ڶ�Ӧλ����ʾ��������
������ƣ�������
���ڣ�2008��11��
*************************************************************************/
void Display_LCD( const uchar Data,const uchar Add )
{  
    Segment_Bit_Display( Add,Data % 10 );
    Segment_Bit_Display( Add - 1, Data / 10 );
}//End Display_LCD
/*************************************************************************
��������Display_NULL
��ڲ�����const uchar Add
���ڲ�������
���ܣ��ڶ�Ӧλ����ʾ�հ�
������ƣ�������
���ڣ�2008��11��
*************************************************************************/
void Display_NULL( const uchar Add )
{  
    Segment_Bit_NULL( Add);
    Segment_Bit_NULL( Add - 1 );
}//End Display_LCD
/*************************************************************************
��������Find_Leap_year
��ڲ�����const uchar Year
���ڲ�������
���ܣ��������������껹��ƽ��
������ƣ�������
���ڣ�2008��11��
*************************************************************************/
uchar Find_Leap_year( const uchar Year )
{
    uint Real_year = 2000 + Year;
    
    return (( Real_year%4 == 0 && Real_year%100 != 0 ) || Real_year%400 == 0 )?1:0;
}
/*************************************************************************
��������Real_Time
��ڲ�����
���ڲ�������
���ܣ�RTC��ʱ�Ӽ��㣬ÿ��һ����¸�ʱ��������WDT_Timer�жϵ���
������ƣ�������
���ڣ�2008��11��
*************************************************************************/
void Real_Time( void )
{
    Str_Calen *P_RTC = &RTC;
    if(  ++P_RTC -> Time_Type.Sec > 59 )
    {
        P_RTC -> Time_Type.Sec = 0;
        if( ++P_RTC -> Time_Type.Min > 59 )
        {
            P_RTC -> Time_Type.Min = 0;
            if( ++P_RTC -> Time_Type.Hou > 23 )
            {
                P_RTC -> Time_Type.Hou = 0; 
                if( Find_Leap_year( P_RTC -> Date_Type.Yea ))
                    Month[ Feb ] = 29;
                else
                    Month[ Feb ] = 28; 
                if( ++P_RTC -> Date_Type.Dat > Month[ P_RTC -> Date_Type.Mon - 1 ] )
                {
                    P_RTC -> Date_Type.Dat = 1;
                    if( ++P_RTC -> Date_Type.Mon > 12 )
                    {
                        P_RTC -> Date_Type.Mon = 1;
                        P_RTC -> Date_Type.Yea++;
                    }
                }
            }
        }
    }
}
/*************************************************************************
��������Display_Time
��ڲ�����
���ڲ�������
���ܣ���LCD����ʾʱ���ʽ
������ƣ�������
���ڣ�2008��11��
*************************************************************************/
void Display_Time( void )
{
    Str_Calen *P_RTC = &RTC;
    Segment_Bit_Display( 6,0x0B );               //ʱ���ʽ��ʼ��
    Display_LCD( P_RTC -> Time_Type.Sec,5);
    Display_LCD( P_RTC -> Time_Type.Min,3);
    Display_LCD( P_RTC -> Time_Type.Hou,1);
}
/*************************************************************************
��������Display_Date
��ڲ�����
���ڲ�������
���ܣ���LCD����ʾ���ڸ�ʽ
������ƣ�������
���ڣ�2008��11��
*************************************************************************/
void Display_Date( void )
{
    Str_Calen *P_RTC = &RTC;
    Segment_Bit_NULL( 6 );
    Display_LCD( P_RTC -> Date_Type.Dat,5);
    Display_LCD( P_RTC -> Date_Type.Mon,3);
    Display_LCD( P_RTC -> Date_Type.Yea,1);
}
/*************************************************************************
��������Display_Object
��ڲ�����
���ڲ�������
���ܣ��ڸ�����ʱ���������Ե���ʾʱ�䡢���ڡ�����
������ƣ�������
���ڣ�2008��11��
*************************************************************************/
void Display_Object( void )
{ 
    if( Face_Flag == DATE_FACE )
        Display_Date();
    else
        Display_Time();
}
/*************************************************************************
��������main
��ڲ�������
���ڲ�������
���ܣ����������
������ƣ�������
���ڣ�2008��11��
*************************************************************************/  
void main( void )
{
    System_Init(); 
    Init_BU9796FS();
    InitLCD();
    Operation_Help();
    _BIS_SR( LPM3_bits + GIE );
    _NOP(); 
}
/*************************************************************************
��������Basic_Timer
��ڲ�������
���ڲ�������
���ܣ��ж���ں�����500ms���
������ƣ�������
���ڣ�2008��11��
*************************************************************************/
#pragma vector = WDT_VECTOR
__interrupt void WDT_Timer(void)
{
    static uchar WDT_Face_Flag = 0;
    
    if( ++WDT_Face_Flag > 1 )                 //1Sʱ�䵽���ͼ���һ��RTC
    {
        Real_Time();
        WDT_Face_Flag = 0;
    }
    if( Key_Flag )
    {
        if( Face_Flag == DATE_FACE )
        {
            switch( Enter_Key )               // ��̬��˸Ҫ���õĶ���
            {
            case 1 :                          // ��˸��
                if( WDT_Face_Flag )
                    Display_Date();
                else
                    Display_NULL( 5 ); 
                break;
            case 2 :                          // ��˸��
                if( WDT_Face_Flag )
                    Display_Date();
                else
                    Display_NULL( 3 ); 
                break; 
            case 3 :                          // ��˸��
                if( WDT_Face_Flag )
                    Display_Date();
                else
                    Display_NULL( 1 ); 
                break; 
                
            default:
                Face_Flag = DATE_FACE;         // �ظ���ENTER_KEY�ͻָ������ڽ��沢ͣ��˸
                Key_Flag = 0;
                Enter_Key = 0;
                break;
            }
        }
        else
        {
            switch( Enter_Key )
            {
            case 1 :                            // ��˸��
                if( WDT_Face_Flag )
                    Display_Time();
                else
                    Display_NULL( 5 ); 
                break;
            case 2 :                            // ��˸��
                if( WDT_Face_Flag )
                    Display_Time();
                else
                    Display_NULL( 3 ); 
                break; 
            case 3 :                            // ��˸ʱ
                if( WDT_Face_Flag )
                    Display_Time();
                else
                    Display_NULL( 1 );
                break; 
            default:
                Face_Flag = TIME_FACE;          // �ظ���ENTER_KEY�ͻָ���ʱ����沢ͣ��˸
                Enter_Key = 0;
                Key_Flag = 0;
                break;
            }
        }
        Display_Page = 0;
    }
    else if( WDT_Face_Flag == 0 )           
    { 
        if( ++Display_Page >= 10 )               // ÿ��10�����һ��ҳ��
        {
            if( Face_Flag == TIME_FACE )
                Face_Flag = DATE_FACE;
            else
                Face_Flag = TIME_FACE;
            Display_Page = 0;
        }
       Display_Object();                     // ���һ�����һ��ʱ��ֵ
    }
}
/*************************************************************************
��������Key
��ڲ�������
���ڲ�������
���ܣ������жϣ���ӦEnter��Up��Down�����ܣ�ֱ�����������������ʱ�����У׼
������ƣ�������
���ڣ�2008��8��28��
*************************************************************************/
#pragma vector = PORT2_VECTOR
__interrupt void Key(void)
{
    Str_Calen *P_RTC = &RTC;
    
    KEY_IE = 0;
    _BIS_SR( GIE );
    switch( KEY_IFG & 0x13 )
    {
    case ENTER:                                   //ENTER��
        Delays(20);
        if( ~KEY_IN & ENTER )
        {
            Enter_Key++;
            Key_Flag = 1;
        }
        break;
    case DOWN:                                    //DOWN��
        Delays(20);
        if( ~KEY_IN & DOWN )
        {
            if( Key_Flag )
            {
                if( Face_Flag == DATE_FACE )
                {
                    switch( Enter_Key )
                    {
                    case 1 :
                        if( --P_RTC -> Date_Type.Dat < 1 )
                        {
                            if( --P_RTC -> Date_Type.Mon < 1 )
                            {
                                P_RTC -> Date_Type.Mon = 12;
                                P_RTC -> Date_Type.Yea--;
                            }
                            P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon - 1 ]; 
                        }
                        break; 
                    case 2:
                        P_RTC -> Date_Type.Mon--; 
                        if( P_RTC -> Date_Type.Mon < 1 )
                        {
                            P_RTC -> Date_Type.Mon = 12;
                            P_RTC -> Date_Type.Yea --;   
                        }
                        if( P_RTC -> Date_Type.Dat > Month[ P_RTC -> Date_Type.Mon -1 ])
                            P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon -1 ];
                        break; 
                    case 3:
                        P_RTC -> Date_Type.Yea--;
                        if( Find_Leap_year( P_RTC -> Date_Type.Yea ))
                        {
                            Month[ Feb ] = 29;
                        }
                        else
                        {
                            Month[ Feb ] = 28;
                        }
                        if( P_RTC -> Date_Type.Dat > Month[ P_RTC -> Date_Type.Mon -1 ])
                            P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon -1 ];
                        if( P_RTC -> Date_Type.Yea > 99 )
                            P_RTC -> Date_Type.Yea = 99;
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    switch( Enter_Key )
                    {
                    case 1 :
                        if( --P_RTC -> Time_Type.Sec > 59 )
                        {
                            P_RTC -> Time_Type.Sec = 59;
                            if( --P_RTC -> Time_Type.Min > 59 )
                            {
                                P_RTC -> Time_Type.Min  = 59;
                                if( --P_RTC -> Time_Type.Hou > 23 )
                                {
                                    P_RTC -> Time_Type.Hou = 23;
                                    P_RTC -> Date_Type.Dat--;
                                }
                            }
                        } 
                        break; 
                    case 2:
                        if( --P_RTC -> Time_Type.Min > 59 )
                        {
                            P_RTC -> Time_Type.Min = 59;
                            if( --P_RTC -> Time_Type.Hou > 23 )
                            {
                                P_RTC -> Time_Type.Hou  = 23;
                                if( --P_RTC -> Date_Type.Dat < 1 )
                                {
                                    P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon-1 ];
                                    if( --P_RTC -> Date_Type.Mon < 1 )
                                    {
                                        P_RTC -> Date_Type.Mon = 12;
                                        P_RTC -> Date_Type.Yea--;
                                    }
                                }
                            }
                        }
                        break; 
                    case 3:
                        if( --P_RTC -> Time_Type.Hou > 23 )
                        {
                            P_RTC -> Time_Type.Hou = 23;
                            if( --P_RTC -> Date_Type.Dat < 1 )
                            {
                                P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon-1 ];
                                if( --P_RTC -> Date_Type.Mon < 1 )
                                {
                                    P_RTC -> Date_Type.Mon = 12;
                                    P_RTC -> Date_Type.Yea--;
                                }
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                if( Find_Leap_year( P_RTC -> Date_Type.Yea ))         // �������
                {
                    Month[ Feb ] = 29;
                }
                else
                {
                    Month[ Feb ] = 28;
                }
            }
            else                                            // �ֶ��л���ʾ����
            {
                if( Face_Flag == TIME_FACE )
                {
                    Face_Flag = DATE_FACE;
                }
                else
                {
                    Face_Flag = TIME_FACE;
                }
                Display_Page = 0;
            }
            break;
        }
    case UP:                                     //INC
        Delays(20);
        if( ~KEY_IN & UP )
        {
            if( Key_Flag )
            {
                if( Face_Flag == DATE_FACE )
                {
                    switch( Enter_Key )
                    {
                    case 1 :
                        if( ++P_RTC -> Date_Type.Dat > Month[ P_RTC -> Date_Type.Mon-1 ] )
                        {
                            P_RTC -> Date_Type.Dat = 1;
                            if( ++P_RTC -> Date_Type.Mon > 12 )
                            {
                                P_RTC -> Date_Type.Mon = 1;
                                P_RTC -> Date_Type.Yea++;
                            }
                        }
                        break; 
                    case 2:
                        P_RTC -> Date_Type.Mon++; 
                        if( P_RTC -> Date_Type.Mon > 12 )
                        {
                            P_RTC -> Date_Type.Mon = 1;
                            P_RTC -> Date_Type.Yea++;
                        }
                        if( P_RTC -> Date_Type.Dat > Month[ P_RTC -> Date_Type.Mon -1 ])
                            P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon -1 ];
                        break; 
                    case 3:
                        P_RTC -> Date_Type.Yea++;
                        if( Find_Leap_year( P_RTC -> Date_Type.Yea ))
                            Month[ Feb ] = 29;
                        else
                            Month[ Feb ] = 28;
                        if( P_RTC -> Date_Type.Dat > Month[ P_RTC -> Date_Type.Mon -1 ])
                            P_RTC -> Date_Type.Dat = Month[ P_RTC -> Date_Type.Mon -1 ];
                        if( P_RTC -> Date_Type.Yea > 99 )
                            P_RTC -> Date_Type.Yea = 0;
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    switch( Enter_Key )
                    {
                    case 1 :
                        if( ++P_RTC -> Time_Type.Sec > 59 )
                        {
                            P_RTC -> Time_Type.Sec = 0;
                            if( ++P_RTC -> Time_Type.Min > 59 )
                            {
                                P_RTC -> Time_Type.Min = 0;
                                if( ++P_RTC -> Time_Type.Hou > 23 )
                                {
                                    P_RTC -> Time_Type.Hou = 0;
                                    P_RTC -> Date_Type.Dat++;
                                }
                            }
                        }
                        break; 
                    case 2:
                        if( ++P_RTC -> Time_Type.Min > 59 )
                        {
                            P_RTC -> Time_Type.Min = 0;
                            if( ++P_RTC -> Time_Type.Hou > 23 )
                            {
                                P_RTC -> Time_Type.Hou = 0;
                                P_RTC -> Date_Type.Dat++; 
                            }
                        }
                        break; 
                    case 3:
                        if( ++P_RTC -> Time_Type.Hou > 23 )
                        {
                            P_RTC -> Time_Type.Hou = 0;
                            P_RTC -> Date_Type.Dat++;
                        }
                        break;
                    default:
                        break;
                    }
                } 
            }
            else                                            // �ֶ��л���ʾ����
            {
                if( Face_Flag == TIME_FACE )
                {
                    Face_Flag = DATE_FACE;
                }
                else
                {
                    Face_Flag = TIME_FACE;
                }
                Display_Page = 0;
            }
            break;   
        default:
            break;
        }
    }
    LED_OUT |= LED_BIT;                          //����ָʾ
    while( ~KEY_IN & 0x13 );                     //�ȴ������ͷ�
    LED_OUT &= ~LED_BIT;
    KEY_IFG = 0;
    KEY_IE |= UP + DOWN + ENTER;
}
