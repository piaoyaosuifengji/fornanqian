#include<reg51.h>                                           
#define uint unsigned int      //�Զ������
#define uchar unsigned char    
uchar f=5; //�����Ĵ���
sbit  P10=P1^0;    //PWM�������1
sbit  P11=P1^1;    //PWM�������2
sbit  P12=P1^2;	   //����ת
sbit  P13=P1^3;	   //����
sbit  P14=P1^4;	  //����
sbit  P15=P1^5;	  //ֹͣ
sbit  P16=P1^6;   //����
sbit  P20=P2^0;	  //����
sbit  P21=P2^1;	  //����
sbit  P22=P2^2;	  //ͣ��
sbit  P23=P2^3;	  //����
uchar k=0;	//����ת��־
uchar t=0;   //����Ӽ�

void display()	//LED��ʾ
{
  if(k==0) {P20=0;P21=1;}
  else {P20=1;P21=0;}
  if(IE==0x8a){P22=1;P23=0;}
  else {P22=0;P23=1;}
}

void key()		  //����ɨ��
{
if(P12==0)	        //������£�
	{
	 while(P12==0)    //ȥ����
	 display(); 
	 k=~k;
	}	  
if(P16==0)     //����
	{
	while(P16==0)
	display();
	IE=0x8a;
	} 	  
if(P13==0)         //����
	{
	while (P13==0)
	display();
	t++;
	}
	if(t>=5)
	t=5;
if(P14==0)  //����
	{
	while(P14==0)
	display();
	t--;
	}
	if(t<1)
	t=1;
if(P15==0)             //ֹͣ
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
	TMOD=0x01;   	//T0��ʽ1��ʱ	
	TH0=0xb1;	   //װ���ֵ	  20MS
	TL0=0xe0;
	TR0=1;       //����t0
	while(1)        
	 {
		display(); //�������ʾ	
	 	key();
	 }
}	
void t0() interrupt 1 using 1	  //t0��ʱ�жϺ���
{
	TH0=0xec;  //��װt0
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
