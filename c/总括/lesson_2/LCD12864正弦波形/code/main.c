/*****http:hi.baidu.com/echoas********
程序：MCS-51驱动LCD12864液晶
内容：LCD12864显示正弦曲线
编译器：keil uVision3
开发板：Proteus仿真图
******http:hi.baidu.com/echoas*********/ 
#include <AT89X52.h>
#include <intrins.h>
#include<math.h>
#include"ZB.h"
#define uchar unsigned char
#define uint  unsigned int

#define LCD_databus P0 //LCD12864的8位数据口

sbit RS=P2^2; 	//RS为0---命令；1----数据 
sbit RW=P2^1; 	//RW为1---写；0---读 
sbit EN=P2^0; 	//使能端 
sbit CS1=P2^4; 	//片选1低电平有效，控制左半屏 
sbit CS2=P2^3; 	//片选1低电平有效，控制右半屏 


void Read_busy()//读“忙”函数-----数据线的最高位DB71则busy		
{
	P0=0x00;
   	RS=0;  
   	RW=1; 
	EN=1;
	while(P0 & 0x80);
	EN=0;
}

void write_LCD_command(uchar value)	//写命令函数 
{
   Read_busy();						//对LCD的每次读写都要读忙 
   RS=0;	 						//选择命令 
   RW=0; 							//读操作 
   LCD_databus=value;
   EN=1;							//EN由1----0锁存有效数据 
   _nop_();
   _nop_(); 
   EN=0;
}

void write_LCD_data(uchar value)//写数据函数	
{
   Read_busy();
   RS=1; 						//选择数据 
   RW=0;	
   LCD_databus=value;
   EN=1;						//EN由1----0锁存有效数据 
   _nop_();		
   _nop_();
   EN=0;
}

uchar Read_LCD(void)			//读数据函数 
{
	uchar value;
	Read_busy();
	LCD_databus=0xFF;			//先进行一次空读操作 
	RS=1;
	RW=1;
	EN=1;
	_nop_();
	_nop_();
	EN=0;
	
	LCD_databus=0xFF;			//读取真正的数据 
	RS=1;
	RW=1;
	EN=1;
	value=LCD_databus;
	_nop_();
	_nop_();
	EN=0;
	
	return value; 
}

void Set_page(uchar page)		//设置“页”LCD12864共8页，一页是8行点阵点	
{
  page=0xb8|page; 				//页的首地址为0xB8
  write_LCD_command(page);
}

void Set_line(uchar startline)  //设置显示的起始行	   
{
  startline=0xC0|startline;     //起始行地址为0xC0 
  write_LCD_command(startline); //设置从哪行开始：共0--63;一般从0 行开始显示
}

void Set_column(uchar column)	//设置显示的列	
{
  column=column &0x3f; 			//列的最大值为64
  column= 0x40|column; 			//列的首地址为0x40 
  write_LCD_command(column);	//规定显示的列的位置 
}

void SetOnOff(uchar onoff)		//显示开关函数：0x3E是关显示，0x3F是开显示   
{
	onoff=0x3e|onoff; 			//onoff:1---开显示；0---关显示
	write_LCD_command(onoff);
}

void SelectScreen(uchar screen)	//选择屏幕 
{ 						 
   switch(screen)
   { 
   	 case 0: CS1=0;CS2=0;break; //全屏     
     case 1: CS1=0;CS2=1;break;//左半屏        
     case 2: CS1=1;CS2=0;break;//右半屏       
     default:break; 
   }
}

void ClearScreen(uchar screen)	//清屏函数  
{ 	
     uchar i,j;
     SelectScreen(screen);		//0--全屏；1---左半屏；2---右半屏 
	 for(i=0;i<8;i++)	 		//控制页数0-7，共8页
	 {
        Set_page(i);
		Set_column(0);
      	for(j=0;j<64;j++)	   //控制列数0-63，共64列
	  	{
          write_LCD_data(0x00); //写入0，地址指针自加1 
      	}
     }					 
}

void InitLCD() 					//LCD的初始化  
{ 
	SetOnOff(1); 				//开显示
	SelectScreen(0);
	ClearScreen(0);				//清屏
	Set_line(0); 				//开始行:0
}
/************************************************
LCD12864任意坐标位置打点函数：坐标
左上方(0,0)--------------------------(127,0)-
-											-
-			坐标系							-
-											-
-											-	
(0,63)------------------------右下方(127,63)-	
原理：确定要打点的具体位置(坐标)，然后读取出LCD中
的数据，经过数据处理后再送入LCD中进行显示。 
************************************************/ 
void Draw_dots(uchar x,uchar y,uchar color)
{
	uchar x_byte;
	uchar x_bit; 
	uchar Temp_data;		//暂时存放从LCD读出的数据 
	x_byte=(y>>3);  		//计算出该点属于哪个字节 
	x_bit=y-(x_byte<<3);	//属于字节的哪一位 

	
	if(x>63)				//x>63则显示在右半屏 
	{
		SelectScreen(2);
		x=x-64;
	}
	else					//显示在左半屏 
	{
		SelectScreen(1); 
	}
	Set_page(x_byte);		//设置行地址 
	Set_column(x);			//设置列地址
 	Temp_data=Read_LCD();	//先读出没打点前LCD中的数据 
	switch(color)
	{
		case 0x01 : Temp_data &= ~(1<<x_bit);break;	//擦除
		case 0x02 : Temp_data ^= (1<<x_bit); break;	//反白 
		case 0x03 : Temp_data |= (1<<x_bit); break;	//画点 
		default : break;
	} 
	
	Set_page(x_byte);
	Set_column(x);
	write_LCD_data(Temp_data);	//将处理后的数据送到LCD中显示 
	
}
/*********************************************************
函数：显示汉字----点阵大小为16*16 
要显示一个完整的汉字，要分两页写入数据来完成，一页是8行点阵
的大小，先写上半部分，再写下半部分！可通过Proteus联调看出！ 
*********************************************************/ 
void Display_HZ(uchar ss,uchar page,uchar column,uchar *p)
{
	uchar i;
	SelectScreen(ss);
	Set_page(page);					//写上半页
	Set_column(column*16); 			//控制列
	for(i=0;i<16;i++)  				//控制16列的数据输出
	{
		write_LCD_data(p[i]); 		//汉字的上半部分 
	}
	Set_page(page+1);				//写下半页
	Set_column(column*16);			//控制列
    for(i=0;i<16;i++)				//控制16列的数据输出
	{
		write_LCD_data(p[i+16]);	//汉字的下半部分 
	}
} 

void Display_picture(uchar a[][64])//显示图形--坐标 
{
	uchar i,j;
	
	for(j=0;j<6;j++)
	{	
		SelectScreen(1);
		Set_page(j+2);				//从第三页开始显示 
		Set_column(0);
		for(i=0;i<64;i++)
		{
			write_LCD_data(a[2*j][i]);//每隔一行取一次数组中的数据
		}
		SelectScreen(2);
		Set_page(j+2);
		Set_column(0);
		for(i=0;i<64;i++)
		{
			write_LCD_data(a[2*j+1][i]);//每隔一行取一次数组中的数据
		}	
	}	
}
void main()
{  
	uchar x,y;
   	InitLCD();			//初始12864  
   	ClearScreen(0);		//清屏
   	Set_line(0);		//显示开始行

	Display_HZ(1,0,0,da);		//大
	Display_HZ(1,0,1,hai);		//海
	Display_HZ(1,0,2,xiang);	//橡
	Display_HZ(1,0,3,shu);		//树
	Display_HZ(2,0,0,bai);		//百
	Display_HZ(2,0,1,du);		//度
	Display_HZ(2,0,2,kong);		//空
	Display_HZ(2,0,3,jian);		//间
	Display_picture(BMP);  		//坐标图
	for(x=12;x<128;x++)			//正弦曲线
	{
		y=35-18*sin((x-12)*3.14/16);
		Draw_dots(x,y,2);
	}
		
	while(1);
}

