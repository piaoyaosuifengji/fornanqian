			   #include<reg52.h>
#include <intrins.h>
#define uchar unsigned char	 //定义unsighed 为unsigned char
#define uint unsigned int	  //定义uint 为 int
uchar code table2[]={"Tian  Gui      "};
uchar code table1[]={'0','1','2','3','4','5','6','7','8','9'};
uchar code table3[]={"key-js is :   "};
uchar qian,bai,shi,ge;	//千，百，十，个位
signed int num=0;
sbit en=P2^2;
sbit rs=P2^0;			
sbit rw=P2^1;

void Delay(uint xx)
{
uint yy;
while(xx--)
for(yy=0;yy<11;yy++);
}


void write_com(uchar com)	 //读状态用了
{
  rw=0;
  rs=0;
  P0=com;
  Delay(5);
  en=1;
  Delay(5);
  en=0;
}

void init()	   //.............初始化
{
  en=0;
  rs=0;
  write_com(0x38);
  write_com(0x0c);
  write_com(0x06);
  write_com(0x01);
  write_com(0x80);
}



void write_date(uchar date)	//向p1口送数据	     用了
{
  rw=0;
  rs=1;
  P0=date;
  Delay(5);
  en=1;
  Delay(5);
  en=0; 
}



void num_cl()
{
if(num==100)
{num=0;}
shi=num%100/10;
ge=num%10;
}



void display()
{
write_com(0xce);	
write_date(table1[shi]);
write_com(0xcf);	
write_date(table1[ge]);
}


void Disp_tg(uchar com,const uchar * pt,uchar num)
{
    uchar i;		
   	write_com(com); 
      for(i=0;i<num;i++) 
      write_date(*(pt++)); 
}


void main()
{
init();
TMOD=0x06;
IT1=1;
TH0=256-1;
TL0=256-1;
IP=0x08;
IE=0X86;
TR0=1;
Disp_tg(0x80,table2,16);
Disp_tg(0xc0,table3,16);
num_cl();
display();
while(1) {
if(num==100)
 num=0;
num_cl();
display();
}

}

 void time0() interrupt 1 using 2{
 TH0=256-1;
 TL0=256-1;
 num++;

}

 void service_int1() interrupt  2 using 2{
 num=0;
 }

