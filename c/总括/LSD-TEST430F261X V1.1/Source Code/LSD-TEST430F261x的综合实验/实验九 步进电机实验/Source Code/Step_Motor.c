//**********************************************************
//** 版权:  杭州利尔达科技有限公司
//** 文件名:   Step_Motor.c
//** 版本:     1.0
//** 工作环境: IAR  4.11B
//** 作者:     LSD
//** 生成日期: 2008.11.13
//** 功能:    驱动步进电机转动
//** 相关说明：5.4-->MCLK ; 5.5-->SMCLK ; 5.6-->ACLK
//**          该步进电机步距角为7.5度
//**          K1开启暂停； K2整步； K3半步； K4 1/4步； K5 方向；
//**          K6加速； K7减速； 
//**注意点：  在电机运行过程中不得转换运行模式      
//*********************************************************** 
#include <msp430x26x.h>
#include "LCD_Graphic.h"

#define  NN            100              //加速减速分NN挡（有NN个不同速度） 

#define  Pluse_OUT          P5OUT
#define  Direc_flag         0x01
#define  Speed_UP_flag      0x02
#define  Speed_DOWN_flag    0x04

#define  FULL_flag          0x10
#define  HALF_flag          0x20
#define  QUARTER_flag       0x40


//***************************************************************************
//-------------------------变量声明-----------------------------------------
//***************************************************************************

const uchar FullStep[16]={0x61,0x41,0x01,0x21, 0x61,0x41,0x01,0x21,
0x61,0x41,0x01,0x21, 0x61,0x41,0x01,0x21}; 

const uchar HalfStep[16]={0x6B,0x67,0x4B,0x59, 0x0B,0x07,0x2B,0x39,
0x6B,0x67,0x4B,0x59, 0x0B,0x07,0x2B,0x39};

const uchar QuarterStep[16]={0x71,0x6B,0x65,0x67,  0x45,0x4B,0x51,0x59,
0x11,0x0B,0x05,0x07,  0x25,0x2B,0x31,0x39};

//------------------------------------------------------------------------------
uchar state = FULL_flag|Direc_flag; 
uchar next;
uchar temp; 
uint FreData;
uchar DispASCII[4];

//**************************************************************************
//-------------------------------函数声明---------------------------------
//**************************************************************************
void init_CLK(void);
void init_Timer(void);
void init_PORT(void);
void KeyScan(void);
void KeyFunction(void);
extern void InitLCD(void);
extern void ClearRAM(void);  
extern void ShowBMP( uchar x,uchar y,uchar width,uchar high,const uchar *bmp );
extern void Delays(uint m);
extern void Display_String( uchar x,uchar y, const uchar *String );
extern void Draw_Line( const uchar start_x,const uchar start_y,const uchar End_x );
//*************************************************************************
//--------------------------------主函数-------------------------------
//**************************************************************************
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                //关看门狗 
  init_CLK();  
  init_PORT();
  init_Timer();
  InitLCD();
  ClearRAM();                                    // 清屏
  ShowBMP(0,0,240,160 ,Lierda_Logo);             //公司产品 LOGO                 
  Delays(900);
  
  ClearRAM(); 
  Draw_Line(0,28,240);
  Display_String(16,8,"步进电机综合实验");
  Display_String(0,34,"模式:    整步");
  Display_String(0,58,"方向:    顺时针");
  Display_String(0,82,"频率:    0  Hz");
  Display_String(0,106,"状态:    暂停  ");
  while(1)
  {
    _BIS_SR(GIE+CPUOFF); 
  }
  
}



/*************************************************
函数(模块)名称:端口1中断服务程序
功能:     判断按键中断。有就退出低功耗
输入参数: 无     	          		
输出参数: 无  	
函数返回值说明:无 		
使用的资源：   		
它说明: 无
*************************************************/
#pragma vector = PORT1_VECTOR
__interrupt void PORT_ISR(void)
{   
  KeyScan();
  P1OUT = 0xf0; 
  P1IFG = 0x00;
}

/*************************************************
函数(模块)名称:TA中断程序
功能:     给步进电机输出特定相序驱动其工作
输入参数: 无     	          		
输出参数: 无  	
函数返回值说明:无 		
使用的资源：   		
它说明: 无
*************************************************/
#pragma vector = TIMERA0_VECTOR
__interrupt void TA_ISR(void)
{
  static char length = 0x0F;
  _EINT();                        //允许中断嵌套，在TA_ISR里能响应按键ISR
  
  CCR0 = 1000000/(24+3*next);     //每按一次加速，增加3个plus
  
  if(state & FULL_flag )          //整步运行？
  {
    Pluse_OUT = FullStep[length];
  }
  else if(state & HALF_flag )     //半步运行？
  {
    Pluse_OUT = HalfStep[length];
  }
  else if(state & QUARTER_flag )  //1/4步运行？
  {
    Pluse_OUT = QuarterStep[length];
  }
  
  
  if(state & Direc_flag ) //顺时针转     
  {
    length--;
    if(length==0xFF)
    {
      length=15;
    }
  }
  else                    //逆时针转
  {
    length++;
    if(length==16)
    {
      length=0;
    }
  }
  
}

/*************************************************
函数(模块)名称:KeyScan
功能:     扫描得到按键的键码值
输入参数: 无     	          		
输出参数: 无  	
函数返回值说明:无 		
使用的资源：   		
它说明: 无
*************************************************/
void KeyScan(void)
{
  char scancode,tempcode;    
  
  P1IFG = 0x00;
  P1OUT = 0xF0;
  
  if( (P1IN&0xF0)!=0xF0 )              //P1.4--P1.7口不全为“1”则与键按下
  {
    Delays(2);    
    if((P1IN&0xF0)!=0xF0)              //再判断一次
    {
      scancode=0xFE;             
      while( (scancode&0x10)!=0 )      //判断4次循环扫描结束没？
      { 
        P1OUT = scancode;
        if  ((P1IN&0xF0)!=0xF0)        //是该行有键按下
        {
          tempcode = (P1IN&0xF0)|0x0F;
          temp = (~scancode)|(~tempcode);
          break;
        }
        else
        {
          scancode=(scancode<<1)|0x01;   
        }
      }     
    }                  
  }
  
  KeyFunction();
  temp=0;
}

/*************************************************
函数(模块)名称:KeyFuction
功能:     根据键值，执行何种操作
输入参数: 无     	          		
输出参数: 无  	
函数返回值说明:无 		
使用的资源：   		
它说明: 无
*************************************************/
void KeyFunction(void)
{
  if(temp == 0x00)
  {
    return;
  }
  else if( temp == 0x11)        //k1  暂停/启动,停止发送脉冲
  {
    TACTL ^= 0x10; 
    if (TACTL & 0x10)
    {
      P5OUT |= BIT0;
      next=0;
      Display_String(0,82,"频率:    024Hz");
      Display_String(0,106,"状态：   运行中…");
    }
    else
    {
      P5OUT &= ~BIT0;
      Display_String(0,82,"频率:    0  Hz");
      Display_String(0,106,"状态：   暂停    ");
    }
  }
  /* 当电机运行后就不能切换运行模式，只有在关闭情况下可切换*/
  else if( (temp == 0x21)&&(!(P5OUT & BIT0))  ) //k2  整部
  {  
    state |= FULL_flag;   
    state &= ~(HALF_flag+QUARTER_flag); //切换到整部模式后，旧运行模式就删除掉 
    Display_String(0,34,"模式：   整步  ");
  }
  
  else if( (temp == 0x41)&&(!(P5OUT & BIT0))  )//k3  半步
  {
    state |= HALF_flag;    
    state &= ~(FULL_flag+QUARTER_flag);
    Display_String(0,34,"模式：   半步  ");
  }
  
  else if( (temp == 0x81)&&(!(P5OUT & BIT0))  )//k4  1/4步
  {
    state |= QUARTER_flag;
    state &= ~(FULL_flag + HALF_flag);
    Display_String(0,34,"模式：   1/4步");
  }
  
  else if(temp == 0x12)      //k5  方向
  {
    state ^=  Direc_flag;
    if(state&Direc_flag)
    {
      Display_String(0,58,"方向：   顺时针");
    }
    else
    {
      Display_String(0,58,"方向：   逆时针");
    }
  }
  
  else if( (temp ==0x22)&&( TACTL & 0x10 )) //k6 加速（电机在运行的情况下加速才有意义）
  {
    next++;
    if(next>NN-1)        //加速到最高速度就恒定
    {
      next = NN-1;
    }
    FreData = 24+3*next;
    DispASCII[0]=FreData/100+48;
    DispASCII[1]=(FreData/10)%10+48;
    DispASCII[2]= FreData%10+48;
    DispASCII[3]= '\0';
    Display_String(27,82,DispASCII);
    
  }
  
  else if( (temp == 0x42)&&( TACTL & 0x10) )//k7 减速（电机在运行的情况下减速才有意义）
  {
    next--;
    if( next==0xFF )          //减速到最低速度就恒定
    {
      next = 0;
    }
    FreData = 24+3*next;
    DispASCII[0]=FreData/100+48;
    DispASCII[1]=(FreData/10)%10+48;
    DispASCII[2]= FreData%10+48;          //得出频率的ASCII值（显示是用ASCII显示的）
    Display_String(27,82,DispASCII);
  } 
  
}

/*************************************************
函数(模块)名称:init_CLK
功能:     初始化时钟
输入参数: 无     	          		
输出参数: 无  	
函数返回值说明:无 		
使用的资源：   		
它说明: 无
*************************************************/
void init_CLK(void)
{
  BCSCTL3 |= XT2S1;                      // 16MHz crystal or resonator
  BCSCTL1 &= ~XT2OFF;                    // Activate XT2 high freq xtal
  do
  {
    IFG1 &= ~OFIFG;                      // Clear OSCFault flag
    BCSCTL3 &= ~XT2OF;
    for (unsigned int i = 0x47FF; i > 0; i--);   // Time for flag to set
  }while (IFG1 & OFIFG);                   // OSCFault flag still set?
  
  BCSCTL2 |= SELM_2 + SELS + DIVS_1;     //MCLK = XT2 HF XTAL (safe) SMCLK=16MHz/2=8MHz
}

/*************************************************
函数(模块)名称:init_Timer
功能:     初始化定时器
输入参数: 无     	          		
输出参数: 无  	
函数返回值说明:无 		
使用的资源：   		
它说明: 无
*************************************************/
void init_Timer(void)
{
  TACTL = TASSEL_2 + TACLR + ID_3;        //TACLK=8MHz/8=1MHz                    
  CCR0  =  41666;                         //24pps
  CCTL0 =  CCIE;
  
  TBCTL = TBSSEL_2 + MC0 + TACLR;        // SMCLK,增计数模式 
  TBCCR0 = 2000;                          // PWM频率设置
  TBCCR3  = 500;                          // PWM占空比设置
  TBCCTL3 = OUTMOD_6;                     // LCD背光控制
}


/*************************************************
函数(模块)名称:init_PORT
功能:     初始化各个端口
输入参数: 无     	          		
输出参数: 无  	
函数返回值说明:无 		
使用的资源：   		
它说明: 无
*************************************************/
void init_PORT(void)
{
  P5DIR = 0xef;   //p5口输出   p5.7为输入 它未用作电机控制 （跑马灯）
  Pluse_OUT = 0x00;
  P1DIR = 0x0f;   //P1.7--p1.4为输入状态  P1.3--P1.0为输出
  P1OUT = 0xf0;   
  P1IES = 0xf0;   //下降沿中断
  P1IE  = 0xf0;   //P1.7--p1.4中断使能
  P1IFG = 0x00;   //标志位清0  
  
  P4OUT &= ~( BIT4+BIT3 + BIT6);
  P4SEL |= BIT3;
  P4DIR |= BIT4 + BIT3 + BIT6;
  P7DIR |= 0xFF;
  P7OUT =  0xFF;
  P8DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5;
  P8OUT = 0xFF;
  P5DIR |= BIT7;
  P5OUT |= BIT7;
}





