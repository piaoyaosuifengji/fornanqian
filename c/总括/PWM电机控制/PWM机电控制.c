#include<reg51.h>                                           
#define uint unsigned int      //自定义变量
#define uchar unsigned char    
uchar f=5; //计数的次数
sbit  P10=P1^0;    //PWM输出波形1
sbit  P11=P1^1;    //PWM输出波形2
sbit  P12=P1^2;	   //正反转
sbit  P13=P1^3;	   //加速
sbit  P14=P1^4;	  //减速
sbit  P15=P1^5;	  //停止
sbit  P16=P1^6;   //启动
sbit  P20=P2^0;	  //正灯
sbit  P21=P2^1;	  //反灯
sbit  P22=P2^2;	  //停灯
sbit  P23=P2^3;	  //启灯
uchar k=0;	//正反转标志
uchar t=0;   //脉冲加减

void display()	//LED显示
{
  if(k==0) {P20=0;P21=1;}
  else {P20=1;P21=0;}
  if(IE==0x8a){P22=1;P23=0;}
  else {P22=0;P23=1;}
}

void key()		  //按键扫描
{
if(P12==0)	        //如果按下，
	{
	 while(P12==0)    //去抖动
	 display(); 
	 k=~k;
	}	  
if(P16==0)     //启动
	{
	while(P16==0)
	display();
	IE=0x8a;
	} 	  
if(P13==0)         //加速
	{
	while (P13==0)
	display();
	t++;
	}
	if(t>=5)
	t=5;
if(P14==0)  //减速
	{
	while(P14==0)
	display();
	t--;
	}
	if(t<1)
	t=1;
if(P15==0)             //停止
	{
	while(P15==0)
	display();
	EA=0;
	P10=0;
	P11=0;
	} 	
}

void main ()
{ 	
	TMOD=0x01;   	//T0方式1定时	
	TH0=0xb1;	   //装入初值	  20MS
	TL0=0xe0;
	TR0=1;       //启动t0
	while(1)        
	 {
		display(); //数码管显示	
	 	key();
	 }
}	
void t0() interrupt 1 using 1	  //t0定时中断函数
{
	TH0=0xec;  //重装t0
	TL0=0xe0;
	f--;
	if(k==0)
	{
	  if(f<t)
	  {P10=1;
	  P11=0;}
	  else  
	  P10=0;	
	  P11=0;
	 }
	else  
	{
		if(f<t)
		{P10=0;
		P11=1;}
		else 
		P11=0;
		P10=0;
	}
	if(f==0)
		{
		f=5;
		}  
}
