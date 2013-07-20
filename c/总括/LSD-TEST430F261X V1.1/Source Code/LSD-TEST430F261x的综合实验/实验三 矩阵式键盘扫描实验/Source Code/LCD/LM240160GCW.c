/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:LM240160GCW.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：关于LM240160GCW液晶模块的相关函数的定义，纯底层液晶驱动，供外部函数调用
** 生成日期:2008年12月
**************************************************************************/
#include "General_File.h"
//*************************************************************************
#define     Fgcolor     0xF8 
#define     Bgcolor     0x00
#define     EndAddr_x   82
#define     EndAddr_y   160

void Delays(uint m );

/*****************************************************************************
函数名：SdCmd
入口参数：uchar Command
出口参数：无
功能:写指令到LCD控制器
代码编制：黄龙松
日期：2008年12月
*****************************************************************************/
void SdCmd(uchar Command)   //send command
{
    LCDCOM &= ~RS;
    LCDBUS = Command;
    LCDCOM &= ~CS;
    LCDCOM &= ~WR;
    LCDCOM |= WR;
    LCDCOM |= CS;
}
/*****************************************************************************
函数名：void SdData(uchar DData)
入口参数：uchar DData
出口参数：无
功能:写数据到LCD的RAM
代码编制：黄龙松
日期：2008年12月
*****************************************************************************/
void SdData(uchar DData)    //send display data
{
    LCDCOM |= RS;
    LCDBUS = DData;
    LCDCOM &= ~CS;
    LCDCOM &= ~WR;
    LCDCOM |= WR;
    LCDCOM |= CS;
}
/*****************************************************************************
函数名：void SdData(uchar DData)
入口参数：uchar DData
出口参数：无
功能:写数据到LCD的RAM
代码编制：黄龙松
日期：2008年12月
*****************************************************************************/
void Reset_LCD( void )
{
    LCDCOM |= RST;   
    LCDCOM |= CS;
    LCDBUS = 0xff;  
    LCDCOM |= WR;
    LCDCOM |= RD;
    LCDCOM &= ~RST; 
    Delays(2);                                 //硬件复位
    LCDCOM |= RST;  
    Delays(800);                               //复位后延迟一段时间
}
    
/*****************************************************************************
函数名：InitLCDM
入口参数：无
出口参数：无
功能:初始化MSP430F449系统
代码编制：黄龙松
日期：2008年12月
*****************************************************************************/
void InitLCD(void )
{  
    Reset_LCD();
    SdCmd(0x30);                                // 选择ext0指令集
    SdCmd(0x94);                                // 退出休眠模式
    SdCmd(0xd1);                                // 内部OSC运行
    SdCmd(0x20); SdData(0x08);                  // 启动内部升压电路，VB=1
    Delays(1);
    SdCmd(0x20); SdData(0x0b);                  // 启动内部LCD驱动电压电路VB=VF=VR=1                                          //灰模式，V0=15.2V 参数设置：25,2V.  蓝模式，V0=15V  参数设置：1D 4
    SdCmd(0x81); SdData(0x1d); SdData(0x04);    // 设置对比度VO值
    SdCmd(0xca); SdData(0x00); SdData(0x27); SdData(0x00);// 设置显示扫描模式：CLD=0; DT=39=160/4-1;FI=0;LF=0;
    SdCmd(0xa7);                                // 正性显示
    SdCmd(0xbb); SdData(0x01);                  // 设置行扫描方向== 0-79,159-80
    SdCmd(0xbc); SdData(0x02); SdData(0x01); SdData(0x02);//设置数据传输形式LI=1;CI=0 ;C/L=0 ;CLR=0 ;GS=2 3b3p
    SdCmd(0x31);                               // 选择ext1指令集 
    SdCmd(0x32); SdData(0x00); SdData(0x03); SdData(0x01);//设置驱动电路参数 Fosc=12.7khz;Fbooster=6hz;bias=1/13 
    SdCmd(0x30);                               // 选择ext0指令集
    SdCmd(0xaf);                               // 开显示 
}
/*****************************************************************************
函数名：ClearRAM
入口参数：
出口参数：无
功能:清屏程序
代码编制：黄龙松
日期：2008年12月
*****************************************************************************/
void ClearRAM( void )
{ 
    // 以背景色为清屏底色
    SdCmd(0x30);                                  // 选择ext0指令集
    SdCmd(0x15); SdData(0x05); SdData(0x54);      // 列地址区域设置
    SdCmd(0x75); SdData(0x00); SdData(0x9f);      // 行地址区域设置
    SdCmd(0x5c);                                  // 数据写入模式
    for (uint i=0;i<160*80;i++)                   // 循环总的数据量（列块*行数）
    {
        SdData(Bgcolor);                          // 写入数据
        SdData(Bgcolor);                          // 写入数据
        SdData(Bgcolor);                          // 写入数据
    }
}
/*****************************************************************************
函数名：ShowBMP
入口参数：uchar x,uchar y,uchar width,uchar high,const uchar bmp[]
出口参数：无
功能:刷新一幅完整的图片到LCD
代码编制：黄龙松
日期：2008年12月
*****************************************************************************/
void ShowBMP( uchar x,uchar y,uchar width,uchar high,const uchar *bmp )
{  
    uint i,p; 
    uchar j,k,Ddata;
    uchar Addr_x = x + 5;
    
    SdCmd(0x30);                                             // 选择ext0指令集
    SdCmd(0x15); 
    SdData(Addr_x); 
    SdData(Addr_x+width/3-1);                                // 列地址区域设置
    SdCmd(0x75); 
    SdData(y); 
    SdData(y+high-1);                                        // 行地址区域设置
    SdCmd(0x5c);                                             // 数据写入模式
    p=0;                                                     // 数组指针初始化
    width=width/8;                                           // 计算水平字节数==图形宽度/8
    for(i=0;i<high*width;i++)                                // 循环总的数据量（字节数）
    {
        k=bmp[p];                                            // 取图形数据
        for (j=0;j<8;j++)                                    // 循环8点列数据写入（按字节方式）
        {
            if( k & 0x80 )                                   // 根据数据位值设置显示色数据
                Ddata = Fgcolor;                             // 数据为1，选择前景色数据
	    else
                Ddata = Bgcolor;                             // 数据为0，选择背景色数据
            SdData( Ddata );                                 // 数据写入
  	    k=k<<1;                                          // 字模数据移位
        }
        p++;                                                 // 数组指针加1
    }
}
/*************************************************************************
函数名：PrintGB
入口参数：const uchar x, const uchar y,const uchar *chinesecode
出口参数：无
功能：在LCD上打印一个完整的汉字
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void PrintGB(const uchar x, const uchar y,const uchar *chinesecode) 
{  
    uchar i,j,k,Ddata;
    uchar Addr_x = x + 5;
    uchar Addr_y = y;
    
    SdCmd(0x30);                                    // 选择ext0指令集
    SdCmd(0x15); SdData(Addr_x); SdData(Addr_x+5);  // 列地址区域设置（18点列）
    SdCmd(0x75); SdData(Addr_y); SdData(Addr_y+15); // 行地址区域设置（16点行）
    SdCmd(0x5c);                                    // 数据写入模式

    for(i=0;i<32;i++)                               // 循环16行像素写入
    {
        k = *(chinesecode+i);                       // 取汉字左半部分字模数据
        for (j=0;j<8;j++)                           // 循环汉字8列像素写入
        {
            if( k & 0x80 )                          // 根据数据位值设置显示色数据
                Ddata=Fgcolor;                      // 数据为1，选择前景色数据数
            else
                Ddata=Bgcolor;                      // 数据为0，选择背景色数据
            SdData(Ddata);                          // 数据写入
            k <<= 1;                                // 字模数据移位
        }
        if( i % 2 )
        {
            SdData( Bgcolor );                       // 空点写入
            SdData( Bgcolor );                       // 空点写入
        }
    }	
}
/*************************************************************************
函数名：Print_ASCII
入口参数：const uchar x, const uchar y,const uchar *ASCII_Code
出口参数：无
功能：18X16ASCII码驱动程序（能够在LCD上打印一个完整的字符）
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Print_ASCII( const uchar x, const uchar y,const uchar *ASCII_Code )
{
   uchar i,j,k,Ddata;
   uchar Addr_x = x + 5;
   uchar Addr_y = y;
   
   SdCmd(0x30);                                         // 选择ext0指令集
   SdCmd(0x15); SdData( Addr_x ); SdData( Addr_x +2 );  // 列地址区域设置（12点列）
   SdCmd(0x75); SdData(Addr_y); SdData(Addr_y +15);     // 行地址区域设置（16点行）
   SdCmd(0x5c); 
                                                        // 数据写入模式
   for(i=0;i<16;i++)                                    // 循环24行像素写入
   {
       k = *(ASCII_Code + i);                           // 取汉字左半部分字模数据
       for (j=0;j<8;j++)                                // 循环汉字8列像素写入
       {
           if( k & 0x80 )                               // 根据数据位值设置显示色数据
               Ddata = Fgcolor;                         // 数据为1，选择前景色数据
           else
               Ddata = Bgcolor;                         // 据为0，选择背景色数据
           SdData( Ddata );                             // 数据写入
           k <<= 1;                                     // 字模数据移位
       }
       SdData( Bgcolor ); 
   }
}
/*************************************************************************
函数名：Draw_Line
入口参数：const uchar x, const uchar y
出口参数：无
功能：在LCD上画一条线，从起点(x)到终点(y)
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void Draw_Line( const uchar start_x,const uchar start_y,const uchar End_x )
{
    uchar Addr_x = start_x + 5;
    uchar Addr_y = start_y;
    uchar Line_Length = End_x - start_x ;
    
    SdCmd(0x30);                                    // 选择ext0指令集
    SdCmd(0x15); SdData( Addr_x ); SdData( Addr_x+ Line_Length/3-1 );  // 列地址区域设置（18点列）
    SdCmd(0x75); SdData( Addr_y ); SdData( Addr_y ); // 行地址区域设置（16点行）
    SdCmd(0x5c);  
    
    for( uchar i = 0;i < Line_Length;i++ )
    {
        SdData( Fgcolor );
    }
}
//********************************************************************
//---------------------延时函数----------------------------------------
//*********************************************************************
void Delays(uint m )  
{ 
  uint j; uint i;
  for(i=0; i<m; i++)
    for(j=0;j<2000; j++) 
      _NOP();
}