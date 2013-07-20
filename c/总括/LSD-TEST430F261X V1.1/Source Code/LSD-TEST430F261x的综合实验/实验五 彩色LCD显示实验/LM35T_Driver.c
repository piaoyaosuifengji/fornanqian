/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���:LM35T_Driver.c
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ����������LM35TҺ��ģ�����غ����Ķ��壬���ײ�Һ�����������ⲿ��������
** ��������:2008��11��
*****************************************************************/
#include "LM35T_Head.h"
#include <string.h>
/*********************************************************************************
** ���ֲ�������,LCD��ʼ�����ñ�
** ���ڣ�2008��10��27��
**********************************************************************************/
const uchar  Set_F500[]={4,0x83,0x00,0xf5,0x00};	// Reg[f500]=00 (internal MCS0 cycle pulse width)
const uchar  Set_F504[]={4,0x83,0x04,0xf5,0x04};	// Reg[f504]=04 (internal MCS1 pulse width) 
const uchar  Set_F505[]={4,0x83,0x05,0xf5,0x80};	// Reg[f505]=80 (internal MCS1 memory accessing setting)
const uchar  Set_F6C4[]={4,0x83,0xc4,0xf6,0x10};	// Reg[f505]=80 Set Memory Clock Divide
const uchar  Set_F080[]={4,0x83,0x80,0xf0,0xfc};	// Reg[f080]=fc (16bpp TFT)
const uchar  Set_F08E[]={4,0x83,0x8e,0xf0,0x32};	// Reg[f08e]=32 (set pixel clock and LCD_ON)
const uchar  Set_F090[]={4,0x83,0x90,0xf0,0x14};	// set the LCD charactics 
const uchar  Set_F091[]={4,0x83,0x91,0xf0,0x25};	
const uchar  Set_F092[]={4,0x83,0x92,0xf0,0x1e};	
const uchar  Set_F094[]={4,0x83,0x94,0xf0,0x05};	
const uchar  Set_F095[]={4,0x83,0x95,0xf0,0x0e};	
const uchar  Set_F096[]={4,0x83,0x96,0xf0,0x03};	

/*****************************************************************
** ��������delayms                                               
** ���ܣ���ʱ�ӳ���
** ����: Hunglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void delayms(uint m)
{
      uint j;
      uint i;
      
      for(i=0; i<m; i++)
            for(j=0; j<100; j++)
                  _NOP();
}
/*****************************************************************
** ��������delay50us                                               
** ���ܣ���ʱ�ӳ���
** ����: Hunglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void delay50us()     
{
      uchar i;
      
      for(i=0; i<11; i++)
            _NOP();
}
/*****************************************************************
** ��������SdCmd                                                
** ���ܣ���Һ��д��һ��ָ��
** ����: Hunglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void SdCmd(uchar Command)  	//send command
{
    LCDCOM  &= ~A1;
    LCDBUS = Command;
    LCDCOM  &= ~CS;
    LCDCOM  &= ~WR;
    LCDCOM  |= WR;
    while( P8IN & Wait );
    LCDCOM  |= CS;
}
/*****************************************************************
** ��������CmdEnd                                               
** ���ܣ���LM35T��Э��ÿд��һ�����ݷ���һ����ֹ��
** ����: Hunglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void CmdEnd(void)   			//send command
{
      LCDCOM |= A1;                	// ָ��
      LCDBUS = 1;
      LCDCOM &= ~CS;                    // ʹ��
      LCDCOM &= ~WR;
      LCDCOM  |= WR;
      while( P8IN & Wait );
      LCDCOM  |= CS;                    // ��ֹ
}
/*****************************************************************
** ��������WritePKG                                             
** ���ܣ���LM35T��Э����LM35Tд��һ����
** ����: Hunglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void WritePKG(uchar const *pkg)      
{
      uchar i;
      for(i=*pkg;i;i--)              //pkg�����׵�ַ���ָ�����������������ָ�����
            SdCmd(*(++pkg));
      CmdEnd();
      delayms(10);
}
/*****************************************************************
** ��������SetFgColor                                            
** ���ܣ�����ǰ��ɫ
** ����: Hunglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void SetFgColor(uint color)
{
      uchar Buffer[4];
      
      Buffer[0]=3;
      Buffer[1]=0x20;
      Buffer[2]=color;
      Buffer[3]=color>>8;
      WritePKG(Buffer);
}
/*****************************************************************
** ��������SetFontFgColor                                            
** ���ܣ���������ɫ
** ����: Hunglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void SetFontFgColor(uint color)
{
      uchar Buffer[4];
      
      Buffer[0]=3;
      Buffer[1]=0x14;
      Buffer[2]=color;
      Buffer[3]=color>>8;
      WritePKG(Buffer);
      
}
/*****************************************************************
** ��������SetFontBgColor                                           
** ���ܣ����ñ���ɫ
** ����: Hunglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void SetFontBgColor(uint color)
{
      uchar Buffer[4];
      
      Buffer[0]=3;
      Buffer[1]=0x15;
      Buffer[2]=color;
      Buffer[3]=color>>8;
      WritePKG(Buffer);
}
/*****************************************************************
** ��������PrintASCII                                          
** ���ܣ���LCD�ϴ�ӡASCII��
** ����: Hunglongsong
** ��������: 2008.11
** ����ļ�:
** �޸���־��
*****************************************************************/
void PrintASCII(uint X, uint Y, uchar *pstr)	// ����ַ��������޶�Ϊ64���ֽ�
{  
      uchar TempData[3], Buffer[6], NoOfChar;
      
      TempData[0]=2;		                // ʹ���ڲ�8X16����
      TempData[1]=0x10;
      TempData[2]=0x00;
      WritePKG(TempData);
      
      Buffer[0]=5;		                // λ���趨
      Buffer[1]=0x12;
      Buffer[2]=X;
      Buffer[3]=X>>8;
      Buffer[4]=Y;
      Buffer[5]=Y>>8;
      WritePKG(Buffer);
      
      NoOfChar=strlen((char*) pstr);            // ��ȡ�ַ�������
      SdCmd(0x17);
      SdCmd(NoOfChar);
      while(*pstr>0)
      {
            SdCmd(*pstr++);
      }
      CmdEnd();
} 
/*************************************************************************
��������PrintGB
��ڲ�����const uchar x, const uchar y,const uchar *pstr
���ڲ�������
���ܣ���LCD�ϴ�ӡһ�������ĺ���
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
void PrintGB(uint X, uint Y, uchar *pstr)       // ����ַ��������޶�Ϊ64���ֽ�
{  
      uchar TempData[3], Buffer[6], NoOfChar;
      
      TempData[0]=2;		                // ʹ���ڲ�6x16 font����
      TempData[1]=0x10;
      TempData[2]=0x04;
      WritePKG(TempData);
      
      Buffer[0]=5;		                // λ���趨
      Buffer[1]=0x12;
      Buffer[2]=X;
      Buffer[3]=X>>8;
      Buffer[4]=Y;
      Buffer[5]=Y>>8;
      WritePKG(Buffer);
      
      NoOfChar=strlen((char*)pstr);             // ��ȡ�ַ�������
      SdCmd(0x17);
      SdCmd(NoOfChar);
      while(*pstr>0)
      {
            SdCmd(*pstr++);
            delay50us();
      }
      CmdEnd();
} 
/*************************************************************************
��������Draw_Dot
��ڲ�����uint X, uint Y
���ڲ�������
���ܣ���LCD��ָ����λ�÷�һ����
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
void Draw_Dot(uint X, uint Y)
{
      uchar Buffer[6];
      
      Buffer[0]=5;
      Buffer[1]=0x23;
      Buffer[2]=X;
      Buffer[3]=X>>8;
      Buffer[4]=Y;
      Buffer[5]=Y>>8;
      WritePKG(Buffer);
      delayms(20);
      
}
/*************************************************************************
��������Draw_Line
��ڲ�����uint X, uint Y
���ڲ�������
���ܣ���LCD��ָ����λ�û�һ����
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
void Draw_Line(uint x1, uint y1, uint x2, uint y2)
{
      uchar Buffer[10];
      
      Buffer[0]=9;
      Buffer[1]=0x24;
      Buffer[2]=x1;
      Buffer[3]=x1>>8;
      Buffer[4]=y1;
      Buffer[5]=y1>>8;
      Buffer[6]=x2;
      Buffer[7]=x2>>8;
      Buffer[8]=y2;
      Buffer[9]=y2>>8;
      WritePKG(Buffer);
      delayms(20);
}
/*************************************************************************
��������Draw_Rect
��ڲ�����uint X, uint Y
���ڲ�������
���ܣ���LCD��ָ����λ�û�һ����
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
void Draw_Rect(uint x1, uint y1, uint x2, uint y2)
{
      uchar Buffer[10];
      
      Buffer[0]=9;
      Buffer[1]=0x26;
      Buffer[2]=x1;
      Buffer[3]=x1>>8;
      Buffer[4]=y1;
      Buffer[5]=y1>>8;
      Buffer[6]=x2;
      Buffer[7]=x2>>8;
      Buffer[8]=y2;
      Buffer[9]=y2>>8;
      WritePKG(Buffer);
      delayms(50);
}
/*************************************************************************
��������Draw_Rect
��ڲ�����uint X, uint Y
���ڲ�������
���ܣ���LCD��ָ����λ�û�һ��ʵ�Ŀ�
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
void Fill_Rect(uint x1, uint y1, uint x2,uint y2)
{
      uchar Buffer[10];
      
      Buffer[0]=9;
      Buffer[1]=0x27;
      Buffer[2]=x1;
      Buffer[3]=x1>>8;
      Buffer[4]=y1;
      Buffer[5]=y1>>8;
      Buffer[6]=x2;
      Buffer[7]=x2>>8;
      Buffer[8]=y2;
      Buffer[9]=y2>>8;
      WritePKG(Buffer);
      //delayms(50);
}
/*************************************************************************
��������Draw_Circle
��ڲ�����uint X, uint Y
���ڲ�������
���ܣ���LCD��ָ����λ�û�һ��Բ
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
void Draw_Circle(uint X, uint Y, uchar R)
{
      uchar Buffer[7];
      
      Buffer[0]=6;
      Buffer[1]=0x28;
      Buffer[2]=X;
      Buffer[3]=X>>8;
      Buffer[4]=Y;
      Buffer[5]=Y>>8;
      Buffer[6]=R;
      WritePKG(Buffer);
      delayms(50);
}
/*************************************************************************
��������Draw_Circle
��ڲ�����uint X, uint Y
���ڲ�������
���ܣ���LCD��ָ����λ�û�һ��ʵ��Բ
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
void Fill_Circle(uint X, uint Y, uchar R)
{
      uchar Buffer[7];
      
      Buffer[0]=6;
      Buffer[1]=0x29;
      Buffer[2]=X;
      Buffer[3]=X>>8;
      Buffer[4]=Y;
      Buffer[5]=Y>>8;
      Buffer[6]=R;
      WritePKG(Buffer);
      delayms(50);
}
/*********************************************************************************
��������initLCDM
��ڲ�����
���ڲ�������
���ܣ���ɫLCD������ʼ��
������ƣ�������
���ڣ�2008��12��
**********************************************************************************/
void InitLCDM(void)
{ 
    LCDCOM  &= ~REST;	
    delayms(1000);
    LCDCOM  |= REST;	
    delayms(4000);
    
    WritePKG(Set_F500);
    WritePKG(Set_F504);	WritePKG(Set_F505);
    WritePKG(Set_F6C4);
    WritePKG(Set_F080);	WritePKG(Set_F08E);
    WritePKG(Set_F090);	WritePKG(Set_F091);
    WritePKG(Set_F092);	WritePKG(Set_F094);
    WritePKG(Set_F095);	WritePKG(Set_F096);
    
    SetFgColor(BLACK);	
    Fill_Rect(0,0,319,239);
}