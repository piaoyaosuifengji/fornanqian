//*************************************************
//** 版权:    杭州利尔达科技有限公司
//** 文件名:  Key Scan.C
//** 版本：V1.0
//** 工作环境: IAR EmbeddedWorkbench 4.11B
//** 作者: ChenBing
//** 生成日期: 2008.11
//** 功能:    
//*****************************************************
#include "msp430x26x.h"
#include "LCD_Graphic.h"

char KeyCodeTemp;

char KeyScan(void);
void Key_Process(uchar);
void Init_Timer(void);

extern void Init_CLK(void);
extern void Init_Port(void);
extern void Delays(uint);
extern void InitLCD(void);
extern void ClearRAM(void); 
extern void Draw_Line( const uchar start_x,const uchar start_y,const uchar End_x );
extern void ShowBMP( uchar x,uchar y,uchar width,uchar high,const uchar *bmp );
extern void Display_String( uchar x,uchar y, const uchar *String );
extern void Delays(uint);
//*****************************************************************************
void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  Init_CLK();
  Init_Port();
  Init_Timer();
  InitLCD();
  ClearRAM();                                    // 清屏
  ShowBMP(0,0,240,160 ,Lierda_Logo);             //公司产品 LOGO
  Delays(2000);
  
  ClearRAM(); 
  Draw_Line(0,28,240);
  Display_String(16,8,"矩阵式按键实验");
  Display_String(10,40,"当前按下的按键:");
  Display_String(10,70,"编码值:");
  while(1)
  {
    P1OUT = 0xF0;
    _BIS_SR(GIE+CPUOFF);
    KeyCodeTemp = KeyScan();
    Key_Process(KeyCodeTemp);
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
  P1IFG = 0x00;
  LPM0_EXIT; 
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
char KeyScan(void)
{
  char scancode,tempcode;    
  
  P1IFG = 0x00;
  
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
          return((~scancode)|(~tempcode));
        }
        else
        {
          scancode=(scancode<<1)|0x01;   
        }
      }     
    }                  
  }
  return 0;
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
void Init_Timer(void)
{
  TACTL = TASSEL_2 + TACLR + ID_3;        //TACLK=8MHz/8=1MHz                    
  CCR0  =  41666;                         //24pps
  CCTL0 =  CCIE;
  
  TBCTL = TBSSEL_2 + MC_1 + TACLR;        // SMCLK,增计数模式 
  TBCCR0 = 2000;                          // PWM频率设置
  TBCCR3  = 500;                          // PWM占空比设置
  TBCCTL3 = OUTMOD_6;                     // LCD背光控制
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
void Key_Process(uchar KeyCode)
{
    switch(KeyCode)
    {
    case 0x11:
        Display_String(58,40,"K1 "); 
        Display_String(34,70,"0x11");
        break;
        
    case 0x21:
        Display_String(58,40,"K2 ");
        Display_String(34,70,"0x21");
        break;
        
    case 0x41:
        Display_String(58,40,"K3 ");
        Display_String(34,70,"0x41");
        break;
        
    case 0x81:
        Display_String(58,40,"K4 ");
        Display_String(34,70,"0x81");
        break;
        
    case 0x12:
        Display_String(58,40,"K5 "); 
        Display_String(34,70,"0x12");
        break;
        
    case 0x22:
        Display_String(58,40,"K6 "); 
        Display_String(34,70,"0x22");
        break;
        
    case 0x42:
        Display_String(58,40,"K7 ");  
        Display_String(34,70,"0x42");
        break;
        
    case 0x82:
        Display_String(58,40,"K8 "); 
        Display_String(34,70,"0x82");
        break;
        
    case 0x14:
        Display_String(58,40,"K9 ");  
        Display_String(34,70,"0x14");
        break;
        
    case 0x24:
        Display_String(58,40,"K10"); 
        Display_String(34,70,"0x24");
        break;
        
    case 0x44:
        Display_String(58,40,"K11"); 
        Display_String(34,70,"0x44");
        break;
        
    case 0x84:
        Display_String(58,40,"K12");
        Display_String(34,70,"0x84");
        break;
        
    case 0x18:
        Display_String(58,40,"K13"); 
        Display_String(34,70,"0x18");
        break;
        
    case 0x28:
        Display_String(58,40,"K14");
        Display_String(34,70,"0x28");
        break;
        
    case 0x48:
        Display_String(58,40,"K15"); 
        Display_String(34,70,"0x48");
        break;
        
    case 0x88:
        Display_String(58,40,"K16");
        Display_String(34,70,"0x88");
        break;
        
    default:
        break; 
    }
}
