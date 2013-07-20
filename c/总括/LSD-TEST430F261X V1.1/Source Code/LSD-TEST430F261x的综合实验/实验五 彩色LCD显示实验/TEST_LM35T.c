/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���:TEST_LM35T.c
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ��������:2008��12��
** ����������LM35TҺ����,��LCD����ʾһЩͼƬ�����ֵ���Ϣ
**  LCD Module:		  Color TFT 320x240 + TCB8000A
*****************************************************************/
#include "LM35T_Head.h"
#include "Picture.h"

//#define     OPEN_BLINK

/***********************      �ⲿ��������    ***********************************/
extern void Init_MCU( void );
extern void InitLCDM( void );
extern void ShowSingleBMP(ulong X, ulong Y,const uchar *Pic);
extern void ShowBMP160(ulong X, ulong Y,const uchar *Pic);
extern void PrintGB(uint X, uint Y, uchar *pstr);
extern void OFF_CPU( void );
extern void OPEN_ISR( void );
extern void SetFontBgColor(uint color);
extern void SetFontFgColor(uint color);
/*******************************************************************************/
const uint Font_FGColor[] = {  RED,GREEN,BLUE,YELLOW,CYAN,MAGENTA,BLACK };
const uint *Font_Color = Font_FGColor;
/*********************************************************************************
��������main
��ڲ�����
���ڲ�������
���ܣ�������
������ƣ�������
���ڣ�2008��12��
**********************************************************************************/
void main( void )
{
    Init_MCU(); 
    InitLCDM();
    ShowSingleBMP( 0,0,Lierda );                    // ����ֱ����ʾһ��ͼƬ
    
#ifdef   OPEN_BLINK                                 // ʾ�����ӹ��ܣ���LCD����ʾ�ı�����������ɫ
    
    SetFontBgColor( WHITE );                        // �ı���ɫ
    do{
        SetFontFgColor( *Font_Color );              // �ı�ǰ��ɫ
        PrintGB( 72,5,"����������Ƽ����޹�˾");    // �ı���Ϣ
        OPEN_ISR();
        OFF_CPU();
        if( ++Font_Color >( Font_FGColor + sizeof( Font_FGColor ) / sizeof( uint ) ))
        {
            Font_Color = Font_FGColor;
        }
    }while(1);
    
#else
    
    OFF_CPU();
    
#endif
    
}


