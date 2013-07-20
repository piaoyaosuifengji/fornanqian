/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:Diop_CH.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 生成日期:2008年12月
*****************************************************************/
#include <string.h>
#include "ZiKu.h"
#include "General_File.h"

/*******************      外部函数调用    ********************************/
extern void PrintGB(const uchar x, const uchar y,const uchar *chinesecode);
extern void Print_ASCII( const uchar x, const uchar y,const uchar *ASCII_Code );
extern void ClearRAM(void);
extern uchar *Commonly_Read_Data( const uchar *Inner_Code );
/*************************************************************************
函数名：Get_GB_12_CHCode
入口参数：const uchar *index
出口参数：无
功能：查找汉字内码
代码编制：黄龙松
日期：2008年12月
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
函数名：Search_chinese_offset_12
入口参数：const uchar *ch_index
出口参数：无
功能：搜索字库中将要显示的汉字
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
uchar Search_chinese_offset_12( const uchar *ch_index )
{
    uchar k = 0;
    uchar j = Chinese_Number - 1;                                     //一共有Chinese_Number个汉字
    uint this_ch_code;
    
    uint in_ch_code = Get_GB_12_CHCode( ch_index );                  //处理需要显示的字符
    while( k <= j )
    {
        this_ch_code = Get_GB_12_CHCode( GB_12[k].Index );          //查找字库中对应的字符
        if( this_ch_code == in_ch_code )   
            return( k ); 
        else  k++;      
    }
    return 0;
}
 
/***********************************************************************
函数名：Display_String
入口参数： uchar x,uchar y, const uchar *String
出口参数：无
功能：在LCD上打印混合式的字符串组
代码编制：黄龙松
日期：2008年12月
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
        if( Buffer[ i ] & 0x80 )                                 //半角还是全角？
        {
#ifndef    CHINESE_SOURCE     
            uchar Offset = Search_chinese_offset_12( Buffer+i ); //内部字库调用
            PrintGB( x,y,GB_12[Offset].Msk );        
#else  
            PrintGB( x,y,Commonly_Read_Data( Buffer + i ));          //外部字库调用   
#endif
            i += 2;        
            x += 6;  
        }
        else
        {
#ifndef    ASCII_SOURCE
            uchar Offset = Buffer[i] - 0x20;
            Print_ASCII( x,y, ASCIICODE[Offset].Msk );           //内部字库调用
#else
            Print_ASCII( x,y,Commonly_Read_Data( Buffer + i ));      //外部字库调用
#endif
            i += 1;
            x += 3;
        }     
    }
    return;
}
