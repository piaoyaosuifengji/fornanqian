/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���:Sound_Head_File.h
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ������
** ��������:2008��11��
*****************************************************************/
#ifndef _Audio_Head_File_
#define _Audio_Head_File_

/**************        �ⲿ��������    **************************/
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

/*****************        ��������    **************************/
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
  /************ ¼��ģʽ ************/
  {
    Nothing,
    Nothing,
    Record_Pause_Resume,
    Audio_Exit
  },
  /************ ���˵�ģʽ ************/
  {
      Nothing,
      Nothing,
      Record_Mode,
      Play_Mode
  },
 /************** ����ģʽ *************/
 {
     CAT5110_INC,
     CAT5110_DEC,
     Audio_Exit,
     Play_Pause_Resume
 }
};

const Audio_Menu_Type *Audio_Menu_Ptr = &Audio_Menu[1];

#endif