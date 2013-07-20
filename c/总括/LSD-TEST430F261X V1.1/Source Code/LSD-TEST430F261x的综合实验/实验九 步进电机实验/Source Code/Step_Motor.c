//**********************************************************
//** ��Ȩ:  ����������Ƽ����޹�˾
//** �ļ���:   Step_Motor.c
//** �汾:     1.0
//** ��������: IAR  4.11B
//** ����:     LSD
//** ��������: 2008.11.13
//** ����:    �����������ת��
//** ���˵����5.4-->MCLK ; 5.5-->SMCLK ; 5.6-->ACLK
//**          �ò�����������Ϊ7.5��
//**          K1������ͣ�� K2������ K3�벽�� K4 1/4���� K5 ����
//**          K6���٣� K7���٣� 
//**ע��㣺  �ڵ�����й����в���ת������ģʽ      
//*********************************************************** 
#include <msp430x26x.h>
#include "LCD_Graphic.h"

#define  NN            100              //���ټ��ٷ�NN������NN����ͬ�ٶȣ� 

#define  Pluse_OUT          P5OUT
#define  Direc_flag         0x01
#define  Speed_UP_flag      0x02
#define  Speed_DOWN_flag    0x04

#define  FULL_flag          0x10
#define  HALF_flag          0x20
#define  QUARTER_flag       0x40


//***************************************************************************
//-------------------------��������-----------------------------------------
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
//-------------------------------��������---------------------------------
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
//--------------------------------������-------------------------------
//**************************************************************************
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                //�ؿ��Ź� 
  init_CLK();  
  init_PORT();
  init_Timer();
  InitLCD();
  ClearRAM();                                    // ����
  ShowBMP(0,0,240,160 ,Lierda_Logo);             //��˾��Ʒ LOGO                 
  Delays(900);
  
  ClearRAM(); 
  Draw_Line(0,28,240);
  Display_String(16,8,"��������ۺ�ʵ��");
  Display_String(0,34,"ģʽ:    ����");
  Display_String(0,58,"����:    ˳ʱ��");
  Display_String(0,82,"Ƶ��:    0  Hz");
  Display_String(0,106,"״̬:    ��ͣ  ");
  while(1)
  {
    _BIS_SR(GIE+CPUOFF); 
  }
  
}



/*************************************************
����(ģ��)����:�˿�1�жϷ������
����:     �жϰ����жϡ��о��˳��͹���
�������: ��     	          		
�������: ��  	
��������ֵ˵��:�� 		
ʹ�õ���Դ��   		
��˵��: ��
*************************************************/
#pragma vector = PORT1_VECTOR
__interrupt void PORT_ISR(void)
{   
  KeyScan();
  P1OUT = 0xf0; 
  P1IFG = 0x00;
}

/*************************************************
����(ģ��)����:TA�жϳ���
����:     �������������ض����������乤��
�������: ��     	          		
�������: ��  	
��������ֵ˵��:�� 		
ʹ�õ���Դ��   		
��˵��: ��
*************************************************/
#pragma vector = TIMERA0_VECTOR
__interrupt void TA_ISR(void)
{
  static char length = 0x0F;
  _EINT();                        //�����ж�Ƕ�ף���TA_ISR������Ӧ����ISR
  
  CCR0 = 1000000/(24+3*next);     //ÿ��һ�μ��٣�����3��plus
  
  if(state & FULL_flag )          //�������У�
  {
    Pluse_OUT = FullStep[length];
  }
  else if(state & HALF_flag )     //�벽���У�
  {
    Pluse_OUT = HalfStep[length];
  }
  else if(state & QUARTER_flag )  //1/4�����У�
  {
    Pluse_OUT = QuarterStep[length];
  }
  
  
  if(state & Direc_flag ) //˳ʱ��ת     
  {
    length--;
    if(length==0xFF)
    {
      length=15;
    }
  }
  else                    //��ʱ��ת
  {
    length++;
    if(length==16)
    {
      length=0;
    }
  }
  
}

/*************************************************
����(ģ��)����:KeyScan
����:     ɨ��õ������ļ���ֵ
�������: ��     	          		
�������: ��  	
��������ֵ˵��:�� 		
ʹ�õ���Դ��   		
��˵��: ��
*************************************************/
void KeyScan(void)
{
  char scancode,tempcode;    
  
  P1IFG = 0x00;
  P1OUT = 0xF0;
  
  if( (P1IN&0xF0)!=0xF0 )              //P1.4--P1.7�ڲ�ȫΪ��1�����������
  {
    Delays(2);    
    if((P1IN&0xF0)!=0xF0)              //���ж�һ��
    {
      scancode=0xFE;             
      while( (scancode&0x10)!=0 )      //�ж�4��ѭ��ɨ�����û��
      { 
        P1OUT = scancode;
        if  ((P1IN&0xF0)!=0xF0)        //�Ǹ����м�����
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
����(ģ��)����:KeyFuction
����:     ���ݼ�ֵ��ִ�к��ֲ���
�������: ��     	          		
�������: ��  	
��������ֵ˵��:�� 		
ʹ�õ���Դ��   		
��˵��: ��
*************************************************/
void KeyFunction(void)
{
  if(temp == 0x00)
  {
    return;
  }
  else if( temp == 0x11)        //k1  ��ͣ/����,ֹͣ��������
  {
    TACTL ^= 0x10; 
    if (TACTL & 0x10)
    {
      P5OUT |= BIT0;
      next=0;
      Display_String(0,82,"Ƶ��:    024Hz");
      Display_String(0,106,"״̬��   �����С�");
    }
    else
    {
      P5OUT &= ~BIT0;
      Display_String(0,82,"Ƶ��:    0  Hz");
      Display_String(0,106,"״̬��   ��ͣ    ");
    }
  }
  /* ��������к�Ͳ����л�����ģʽ��ֻ���ڹر�����¿��л�*/
  else if( (temp == 0x21)&&(!(P5OUT & BIT0))  ) //k2  ����
  {  
    state |= FULL_flag;   
    state &= ~(HALF_flag+QUARTER_flag); //�л�������ģʽ�󣬾�����ģʽ��ɾ���� 
    Display_String(0,34,"ģʽ��   ����  ");
  }
  
  else if( (temp == 0x41)&&(!(P5OUT & BIT0))  )//k3  �벽
  {
    state |= HALF_flag;    
    state &= ~(FULL_flag+QUARTER_flag);
    Display_String(0,34,"ģʽ��   �벽  ");
  }
  
  else if( (temp == 0x81)&&(!(P5OUT & BIT0))  )//k4  1/4��
  {
    state |= QUARTER_flag;
    state &= ~(FULL_flag + HALF_flag);
    Display_String(0,34,"ģʽ��   1/4��");
  }
  
  else if(temp == 0x12)      //k5  ����
  {
    state ^=  Direc_flag;
    if(state&Direc_flag)
    {
      Display_String(0,58,"����   ˳ʱ��");
    }
    else
    {
      Display_String(0,58,"����   ��ʱ��");
    }
  }
  
  else if( (temp ==0x22)&&( TACTL & 0x10 )) //k6 ���٣���������е�����¼��ٲ������壩
  {
    next++;
    if(next>NN-1)        //���ٵ�����ٶȾͺ㶨
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
  
  else if( (temp == 0x42)&&( TACTL & 0x10) )//k7 ���٣���������е�����¼��ٲ������壩
  {
    next--;
    if( next==0xFF )          //���ٵ�����ٶȾͺ㶨
    {
      next = 0;
    }
    FreData = 24+3*next;
    DispASCII[0]=FreData/100+48;
    DispASCII[1]=(FreData/10)%10+48;
    DispASCII[2]= FreData%10+48;          //�ó�Ƶ�ʵ�ASCIIֵ����ʾ����ASCII��ʾ�ģ�
    Display_String(27,82,DispASCII);
  } 
  
}

/*************************************************
����(ģ��)����:init_CLK
����:     ��ʼ��ʱ��
�������: ��     	          		
�������: ��  	
��������ֵ˵��:�� 		
ʹ�õ���Դ��   		
��˵��: ��
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
����(ģ��)����:init_Timer
����:     ��ʼ����ʱ��
�������: ��     	          		
�������: ��  	
��������ֵ˵��:�� 		
ʹ�õ���Դ��   		
��˵��: ��
*************************************************/
void init_Timer(void)
{
  TACTL = TASSEL_2 + TACLR + ID_3;        //TACLK=8MHz/8=1MHz                    
  CCR0  =  41666;                         //24pps
  CCTL0 =  CCIE;
  
  TBCTL = TBSSEL_2 + MC0 + TACLR;        // SMCLK,������ģʽ 
  TBCCR0 = 2000;                          // PWMƵ������
  TBCCR3  = 500;                          // PWMռ�ձ�����
  TBCCTL3 = OUTMOD_6;                     // LCD�������
}


/*************************************************
����(ģ��)����:init_PORT
����:     ��ʼ�������˿�
�������: ��     	          		
�������: ��  	
��������ֵ˵��:�� 		
ʹ�õ���Դ��   		
��˵��: ��
*************************************************/
void init_PORT(void)
{
  P5DIR = 0xef;   //p5�����   p5.7Ϊ���� ��δ����������� ������ƣ�
  Pluse_OUT = 0x00;
  P1DIR = 0x0f;   //P1.7--p1.4Ϊ����״̬  P1.3--P1.0Ϊ���
  P1OUT = 0xf0;   
  P1IES = 0xf0;   //�½����ж�
  P1IE  = 0xf0;   //P1.7--p1.4�ж�ʹ��
  P1IFG = 0x00;   //��־λ��0  
  
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





