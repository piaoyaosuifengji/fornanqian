/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:Sound_Head_File.h
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：
** 生成日期:2008年11月
*****************************************************************/
#ifndef _Audio_Head_File_
#define _Audio_Head_File_

/**************        外部函数调用    **************************/
extern void Init_MCU( void );
extern void OFF_CPU( void );
extern void Record( void );
extern void Play( void );
extern void Display_String( uchar x,uchar y, const uchar *String );
extern void ClearRAM( void );
extern void ShowBMP( uchar x,uchar y,uchar width,uchar high,const uchar *bmp );
extern void InitLCD(void );
extern void Draw_Line( const uchar start_x,const uchar start_y,const uchar End_x );
extern void Record_Switch( uchar Com_Flag );
extern void Play_Switch( uchar Com_Flag );
extern void Exit_Record( void );
extern void Exit_Play( void );
extern uchar Scan_Key( void );

/*****************        函数声明    **************************/
void Audio_Exit( void );
void Record_Pause_Resume( void );
void Play_Pause_Resume( void );
void Record_Mode( void );
void Play_Mode( void );
void CAT5110_INC( void );
void CAT5110_DEC( void );
void Nothing( void );

/***************************************************************/
typedef void (*Key_Action)();
typedef void (*Application_Fun)();

typedef struct
{
    const Key_Action Up;
    const Key_Action Down;
    const Key_Action Left;
    const Key_Action Right;
}Audio_Menu_Type;

const Audio_Menu_Type Audio_Menu[] = 
{
  /************ 录音模式 ************/
  {
    Nothing,
    Nothing,
    Record_Pause_Resume,
    Audio_Exit
  },
  /************ 主菜单模式 ************/
  {
      Nothing,
      Nothing,
      Record_Mode,
      Play_Mode
  },
 /************** 播放模式 *************/
 {
     CAT5110_INC,
     CAT5110_DEC,
     Audio_Exit,
     Play_Pause_Resume
 }
};

const Audio_Menu_Type *Audio_Menu_Ptr = &Audio_Menu[1];

#endif