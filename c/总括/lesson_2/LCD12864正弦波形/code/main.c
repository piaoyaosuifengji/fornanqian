/*****http:hi.baidu.com/echoas********
����MCS-51����LCD12864Һ��
���ݣ�LCD12864��ʾ��������
��������keil uVision3
�����壺Proteus����ͼ
******http:hi.baidu.com/echoas*********/ 
#include <AT89X52.h>
#include <intrins.h>
#include<math.h>
#include"ZB.h"
#define uchar unsigned char
#define uint  unsigned int

#define LCD_databus P0 //LCD12864��8λ���ݿ�

sbit RS=P2^2; 	//RSΪ0---���1----���� 
sbit RW=P2^1; 	//RWΪ1---д��0---�� 
sbit EN=P2^0; 	//ʹ�ܶ� 
sbit CS1=P2^4; 	//Ƭѡ1�͵�ƽ��Ч����������� 
sbit CS2=P2^3; 	//Ƭѡ1�͵�ƽ��Ч�������Ұ��� 


void Read_busy()//����æ������-----�����ߵ����λDB71��busy		
{
	P0=0x00;
   	RS=0;  
   	RW=1; 
	EN=1;
	while(P0 & 0x80);
	EN=0;
}

void write_LCD_command(uchar value)	//д����� 
{
   Read_busy();						//��LCD��ÿ�ζ�д��Ҫ��æ 
   RS=0;	 						//ѡ������ 
   RW=0; 							//������ 
   LCD_databus=value;
   EN=1;							//EN��1----0������Ч���� 
   _nop_();
   _nop_(); 
   EN=0;
}

void write_LCD_data(uchar value)//д���ݺ���	
{
   Read_busy();
   RS=1; 						//ѡ������ 
   RW=0;	
   LCD_databus=value;
   EN=1;						//EN��1----0������Ч���� 
   _nop_();		
   _nop_();
   EN=0;
}

uchar Read_LCD(void)			//�����ݺ��� 
{
	uchar value;
	Read_busy();
	LCD_databus=0xFF;			//�Ƚ���һ�οն����� 
	RS=1;
	RW=1;
	EN=1;
	_nop_();
	_nop_();
	EN=0;
	
	LCD_databus=0xFF;			//��ȡ���������� 
	RS=1;
	RW=1;
	EN=1;
	value=LCD_databus;
	_nop_();
	_nop_();
	EN=0;
	
	return value; 
}

void Set_page(uchar page)		//���á�ҳ��LCD12864��8ҳ��һҳ��8�е����	
{
  page=0xb8|page; 				//ҳ���׵�ַΪ0xB8
  write_LCD_command(page);
}

void Set_line(uchar startline)  //������ʾ����ʼ��	   
{
  startline=0xC0|startline;     //��ʼ�е�ַΪ0xC0 
  write_LCD_command(startline); //���ô����п�ʼ����0--63;һ���0 �п�ʼ��ʾ
}

void Set_column(uchar column)	//������ʾ����	
{
  column=column &0x3f; 			//�е����ֵΪ64
  column= 0x40|column; 			//�е��׵�ַΪ0x40 
  write_LCD_command(column);	//�涨��ʾ���е�λ�� 
}

void SetOnOff(uchar onoff)		//��ʾ���غ�����0x3E�ǹ���ʾ��0x3F�ǿ���ʾ   
{
	onoff=0x3e|onoff; 			//onoff:1---����ʾ��0---����ʾ
	write_LCD_command(onoff);
}

void SelectScreen(uchar screen)	//ѡ����Ļ 
{ 						 
   switch(screen)
   { 
   	 case 0: CS1=0;CS2=0;break; //ȫ��     
     case 1: CS1=0;CS2=1;break;//�����        
     case 2: CS1=1;CS2=0;break;//�Ұ���       
     default:break; 
   }
}

void ClearScreen(uchar screen)	//��������  
{ 	
     uchar i,j;
     SelectScreen(screen);		//0--ȫ����1---�������2---�Ұ��� 
	 for(i=0;i<8;i++)	 		//����ҳ��0-7����8ҳ
	 {
        Set_page(i);
		Set_column(0);
      	for(j=0;j<64;j++)	   //��������0-63����64��
	  	{
          write_LCD_data(0x00); //д��0����ַָ���Լ�1 
      	}
     }					 
}

void InitLCD() 					//LCD�ĳ�ʼ��  
{ 
	SetOnOff(1); 				//����ʾ
	SelectScreen(0);
	ClearScreen(0);				//����
	Set_line(0); 				//��ʼ��:0
}
/************************************************
LCD12864��������λ�ô�㺯��������
���Ϸ�(0,0)--------------------------(127,0)-
-											-
-			����ϵ							-
-											-
-											-	
(0,63)------------------------���·�(127,63)-	
ԭ��ȷ��Ҫ���ľ���λ��(����)��Ȼ���ȡ��LCD��
�����ݣ��������ݴ����������LCD�н�����ʾ�� 
************************************************/ 
void Draw_dots(uchar x,uchar y,uchar color)
{
	uchar x_byte;
	uchar x_bit; 
	uchar Temp_data;		//��ʱ��Ŵ�LCD���������� 
	x_byte=(y>>3);  		//������õ������ĸ��ֽ� 
	x_bit=y-(x_byte<<3);	//�����ֽڵ���һλ 

	
	if(x>63)				//x>63����ʾ���Ұ��� 
	{
		SelectScreen(2);
		x=x-64;
	}
	else					//��ʾ������� 
	{
		SelectScreen(1); 
	}
	Set_page(x_byte);		//�����е�ַ 
	Set_column(x);			//�����е�ַ
 	Temp_data=Read_LCD();	//�ȶ���û���ǰLCD�е����� 
	switch(color)
	{
		case 0x01 : Temp_data &= ~(1<<x_bit);break;	//����
		case 0x02 : Temp_data ^= (1<<x_bit); break;	//���� 
		case 0x03 : Temp_data |= (1<<x_bit); break;	//���� 
		default : break;
	} 
	
	Set_page(x_byte);
	Set_column(x);
	write_LCD_data(Temp_data);	//�������������͵�LCD����ʾ 
	
}
/*********************************************************
��������ʾ����----�����СΪ16*16 
Ҫ��ʾһ�������ĺ��֣�Ҫ����ҳд����������ɣ�һҳ��8�е���
�Ĵ�С����д�ϰ벿�֣���д�°벿�֣���ͨ��Proteus���������� 
*********************************************************/ 
void Display_HZ(uchar ss,uchar page,uchar column,uchar *p)
{
	uchar i;
	SelectScreen(ss);
	Set_page(page);					//д�ϰ�ҳ
	Set_column(column*16); 			//������
	for(i=0;i<16;i++)  				//����16�е��������
	{
		write_LCD_data(p[i]); 		//���ֵ��ϰ벿�� 
	}
	Set_page(page+1);				//д�°�ҳ
	Set_column(column*16);			//������
    for(i=0;i<16;i++)				//����16�е��������
	{
		write_LCD_data(p[i+16]);	//���ֵ��°벿�� 
	}
} 

void Display_picture(uchar a[][64])//��ʾͼ��--���� 
{
	uchar i,j;
	
	for(j=0;j<6;j++)
	{	
		SelectScreen(1);
		Set_page(j+2);				//�ӵ���ҳ��ʼ��ʾ 
		Set_column(0);
		for(i=0;i<64;i++)
		{
			write_LCD_data(a[2*j][i]);//ÿ��һ��ȡһ�������е�����
		}
		SelectScreen(2);
		Set_page(j+2);
		Set_column(0);
		for(i=0;i<64;i++)
		{
			write_LCD_data(a[2*j+1][i]);//ÿ��һ��ȡһ�������е�����
		}	
	}	
}
void main()
{  
	uchar x,y;
   	InitLCD();			//��ʼ12864  
   	ClearScreen(0);		//����
   	Set_line(0);		//��ʾ��ʼ��

	Display_HZ(1,0,0,da);		//��
	Display_HZ(1,0,1,hai);		//��
	Display_HZ(1,0,2,xiang);	//��
	Display_HZ(1,0,3,shu);		//��
	Display_HZ(2,0,0,bai);		//��
	Display_HZ(2,0,1,du);		//��
	Display_HZ(2,0,2,kong);		//��
	Display_HZ(2,0,3,jian);		//��
	Display_picture(BMP);  		//����ͼ
	for(x=12;x<128;x++)			//��������
	{
		y=35-18*sin((x-12)*3.14/16);
		Draw_dots(x,y,2);
	}
		
	while(1);
}

