#ifndef _RTC_Head_File_
#define _RTC_Head_File_

#include <msp430x26x.h>
#include "General_File.h"

#define     Feb            0x01
#define     TIME_FACE      0xA0
#define     DATE_FACE      0xA1

/*************************************
--时间格式
*************************************/
typedef struct
{
    uchar  Sec;
    uchar  Min;
    uchar  Hou;
}Str_Time;
/*************************************
--日期格式
*************************************/
typedef struct
{
    uchar  Dat;
    uchar  Mon;
    uchar  Yea;
}Str_Date;      
/*************************************
--日历格式
*************************************/
typedef struct
{
    Str_Time Time_Type;
    Str_Date Date_Type;
}Str_Calen;

Str_Calen RTC = { 0,0,0,1,1,9 };        //RTC
Str_Time  A_Clock = { 0,0,12 };          //闹钟

extern void Segment_Bit_Display( const uchar Addr,const uchar Data );
extern void Segment_Bit_NULL( const uchar Addr );
extern void Init_BU9796FS( void );
extern void System_Init( void );
extern void InitLCD( void );
extern void ClearRAM( void );
extern void Display_String( uchar x,uchar y, const uchar *String );
extern void Draw_Line( const uchar start_x,const uchar start_y,const uchar End_x );
extern void LCD_OFF( void );
extern void LCD_ON( void );

#endif
