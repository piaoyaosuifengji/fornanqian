/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���:Diop_CH.c
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ��������:2008��12��
*****************************************************************/
#include <string.h>
#include "ZiKu.h"
#include "General_File.h"

/*******************      �ⲿ��������    ********************************/
extern void PrintGB(const uchar x, const uchar y,const uchar *chinesecode);
extern void Print_ASCII( const uchar x, const uchar y,const uchar *ASCII_Code );
extern void ClearRAM(void);
extern uchar *Commonly_Read_Data( const uchar *Inner_Code );
/*************************************************************************
��������Get_GB_12_CHCode
��ڲ�����const uchar *index
���ڲ�������
���ܣ����Һ�������
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
uint Get_GB_12_CHCode( const uchar *index ) 
{
    uint this_ch_code;
    
    this_ch_code =  index[ 1 ] ;
    this_ch_code =  ( this_ch_code << 8 ) + index[ 0 ];   
    return this_ch_code;
}
//End Get_GB_12_CHCod
/*************************************************************************
��������Search_chinese_offset_12
��ڲ�����const uchar *ch_index
���ڲ�������
���ܣ������ֿ��н�Ҫ��ʾ�ĺ���
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
uchar Search_chinese_offset_12( const uchar *ch_index )
{
    uchar k = 0;
    uchar j = Chinese_Number - 1;                                     //һ����Chinese_Number������
    uint this_ch_code;
    
    uint in_ch_code = Get_GB_12_CHCode( ch_index );                  //������Ҫ��ʾ���ַ�
    while( k <= j )
    {
        this_ch_code = Get_GB_12_CHCode( GB_12[k].Index );          //�����ֿ��ж�Ӧ���ַ�
        if( this_ch_code == in_ch_code )   
            return( k ); 
        else  k++;      
    }
    return 0;
}
 
/***********************************************************************
��������Display_String
��ڲ����� uchar x,uchar y, const uchar *String
���ڲ�������
���ܣ���LCD�ϴ�ӡ���ʽ���ַ�����
������ƣ�������
���ڣ�2008��12��
************************************************************************/
#define     CHINESE_SOURCE
//#define     ASCII_SOURCE   

void Display_String( uchar x,uchar y, const uchar *String )
{
    uchar i= 0;
    const uchar *Buffer = String;
    uchar String_CNT = strlen( (char*)Buffer );

    while( i < String_CNT )
    {  
        if( Buffer[ i ] & 0x80 )                                 //��ǻ���ȫ�ǣ�
        {
#ifndef    CHINESE_SOURCE     
            uchar Offset = Search_chinese_offset_12( Buffer+i ); //�ڲ��ֿ����
            PrintGB( x,y,GB_12[Offset].Msk );        
#else  
            PrintGB( x,y,Commonly_Read_Data( Buffer + i ));          //�ⲿ�ֿ����   
#endif
            i += 2;        
            x += 6;  
        }
        else
        {
#ifndef    ASCII_SOURCE
            uchar Offset = Buffer[i] - 0x20;
            Print_ASCII( x,y, ASCIICODE[Offset].Msk );           //�ڲ��ֿ����
#else
            Print_ASCII( x,y,Commonly_Read_Data( Buffer + i ));      //�ⲿ�ֿ����
#endif
            i += 1;
            x += 3;
        }     
    }
    return;
}
