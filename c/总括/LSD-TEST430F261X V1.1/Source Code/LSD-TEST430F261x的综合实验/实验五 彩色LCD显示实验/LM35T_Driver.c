/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:LM35T_Driver.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：关于LM35T液晶模块的相关函数的定义，纯底层液晶驱动，供外部函数调用
** 生成日期:2008年11月
*****************************************************************/
#include "LM35T_Head.h"
#include <string.h>
/*********************************************************************************
** 部分参数定义,LCD初始化配置表
** 日期：2008年10月27日
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
** 函数名：delayms                                               
** 功能：延时子程序
** 作者: Hunglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
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
** 函数名：delay50us                                               
** 功能：延时子程序
** 作者: Hunglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void delay50us()     
{
      uchar i;
      
      for(i=0; i<11; i++)
            _NOP();
}
/*****************************************************************
** 函数名：SdCmd                                                
** 功能：给液晶写入一个指令
** 作者: Hunglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
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
** 函数名：CmdEnd                                               
** 功能：据LM35T的协议每写完一组数据发送一个终止符
** 作者: Hunglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void CmdEnd(void)   			//send command
{
      LCDCOM |= A1;                	// 指令
      LCDBUS = 1;
      LCDCOM &= ~CS;                    // 使能
      LCDCOM &= ~WR;
      LCDCOM  |= WR;
      while( P8IN & Wait );
      LCDCOM  |= CS;                    // 禁止
}
/*****************************************************************
** 函数名：WritePKG                                             
** 功能：据LM35T的协议向LM35T写入一个包
** 作者: Hunglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void WritePKG(uchar const *pkg)      
{
      uchar i;
      for(i=*pkg;i;i--)              //pkg数组首地址存放指令包的数据量，包括指令代码
            SdCmd(*(++pkg));
      CmdEnd();
      delayms(10);
}
/*****************************************************************
** 函数名：SetFgColor                                            
** 功能：设置前景色
** 作者: Hunglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
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
** 函数名：SetFontFgColor                                            
** 功能：设置字体色
** 作者: Hunglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
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
** 函数名：SetFontBgColor                                           
** 功能：设置背景色
** 作者: Hunglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
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
** 函数名：PrintASCII                                          
** 功能：在LCD上打印ASCII码
** 作者: Hunglongsong
** 生成日期: 2008.11
** 相关文件:
** 修改日志：
*****************************************************************/
void PrintASCII(uint X, uint Y, uchar *pstr)	// 最大字符串数量限定为64个字节
{  
      uchar TempData[3], Buffer[6], NoOfChar;
      
      TempData[0]=2;		                // 使用内部8X16字体
      TempData[1]=0x10;
      TempData[2]=0x00;
      WritePKG(TempData);
      
      Buffer[0]=5;		                // 位置设定
      Buffer[1]=0x12;
      Buffer[2]=X;
      Buffer[3]=X>>8;
      Buffer[4]=Y;
      Buffer[5]=Y>>8;
      WritePKG(Buffer);
      
      NoOfChar=strlen((char*) pstr);            // 获取字符串长度
      SdCmd(0x17);
      SdCmd(NoOfChar);
      while(*pstr>0)
      {
            SdCmd(*pstr++);
      }
      CmdEnd();
} 
/*************************************************************************
函数名：PrintGB
入口参数：const uchar x, const uchar y,const uchar *pstr
出口参数：无
功能：在LCD上打印一个完整的汉字
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void PrintGB(uint X, uint Y, uchar *pstr)       // 最大字符串数量限定为64个字节
{  
      uchar TempData[3], Buffer[6], NoOfChar;
      
      TempData[0]=2;		                // 使用内部6x16 font字体
      TempData[1]=0x10;
      TempData[2]=0x04;
      WritePKG(TempData);
      
      Buffer[0]=5;		                // 位置设定
      Buffer[1]=0x12;
      Buffer[2]=X;
      Buffer[3]=X>>8;
      Buffer[4]=Y;
      Buffer[5]=Y>>8;
      WritePKG(Buffer);
      
      NoOfChar=strlen((char*)pstr);             // 获取字符串长度
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
函数名：Draw_Dot
入口参数：uint X, uint Y
出口参数：无
功能：在LCD上指定的位置放一个点
代码编制：黄龙松
日期：2008年12月
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
函数名：Draw_Line
入口参数：uint X, uint Y
出口参数：无
功能：在LCD上指定的位置画一条线
代码编制：黄龙松
日期：2008年12月
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
函数名：Draw_Rect
入口参数：uint X, uint Y
出口参数：无
功能：在LCD上指定的位置画一个框
代码编制：黄龙松
日期：2008年12月
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
函数名：Draw_Rect
入口参数：uint X, uint Y
出口参数：无
功能：在LCD上指定的位置画一个实心框
代码编制：黄龙松
日期：2008年12月
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
函数名：Draw_Circle
入口参数：uint X, uint Y
出口参数：无
功能：在LCD上指定的位置画一个圆
代码编制：黄龙松
日期：2008年12月
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
函数名：Draw_Circle
入口参数：uint X, uint Y
出口参数：无
功能：在LCD上指定的位置画一个实心圆
代码编制：黄龙松
日期：2008年12月
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
函数名：initLCDM
入口参数：
出口参数：无
功能：彩色LCD参数初始化
代码编制：黄龙松
日期：2008年12月
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