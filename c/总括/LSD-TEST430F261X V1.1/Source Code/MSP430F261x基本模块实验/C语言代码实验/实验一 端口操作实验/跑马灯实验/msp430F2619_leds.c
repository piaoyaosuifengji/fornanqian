//*******************************************************************
//** 版权:     利尔达科技有限公司
//** 文件名:   msp430F2619_leds.c
//** 版本：    V1.0
//** 工作环境: IAR Embedded Workbench IDE 4.11A
//** 作者:     谷雨
//** 生成日期: 2008.11.12
//** 功能:     通过PA（P7和P8）口外界的发光二极管实现跑马灯功能
//** 相关文件:  
//** 修改日志: 无
//*******************************************************************

#include  <msp430x26x.h>

void Delay(unsigned int time);

unsigned int FLASH_led[]=              //跑马灯花样
                         {0x0001,0x8000,0x0003,0xc000,0x0007,0xe000,
                          0x000f,0xf000,0x001f,0xf800,0x003f,0xfc00,
                          0x007f,0xfe00,0x00ff,0xff00,0x01ff,0xff80,
                          0x03ff,0xffc0,0x07ff,0xffe0,0x0fff,0xfff0,
                          0x1fff,0xfff8,0x3fff,0xfffc,0x7fff,0xfffe,
                          0xffff,0xffff,0x0005,0xa000,0x0015,0xa800,
                          0x0055,0xaa00,0x0155,0xaa80,0x0555,0xaaa0,
                          0x1555,0xaaa8,0x5555,0xaaaa};
unsigned int Time_led[]=             //跑马灯速度
                         {0x0fff,0x1fff,0x2fff,0x3fff,0x4fff,0x5fff,
                          0x6fff,0x7fff,0x8fff,0x9fff,0xafff,0xbfff,
                          0xcfff,0xdfff,0xefff,0xffff};

/************************************************************************
函数名：   main (void)
入口参数：无
出口参数：无
功能：    主程序入口，初始化相关端口
************************************************************************/
int main(void)
{
    unsigned char i;
    unsigned char count;                   //跑马灯花样计数
    unsigned char count_time=0;            //跑马灯时间循环计数
    unsigned char speed_up=0;             //加速标志位
    unsigned int temp;
    
    WDTCTL = WDTPW + WDTHOLD;              //关开门狗
    P3DIR |= BIT2;                         //使能跑马灯的电源信号
    P3OUT &= ~BIT2;
    P8SEL = 0X00;                         //配置P8.6，P8.7
    PADIR = 0xffff; 
    PAOUT = 0x0000;                      //点亮全部LED，用于测试  
    for(i=0;i<10;i++)
    {
        Delay(0xffff);
    }
    
    while(1)
    {
        for(count=0;count<46;count++)         
        {
            temp=FLASH_led[count];
            if(count%2==0)                //正向旋转
            {
                for(i=0;i<16;i++)
                {
                    PAOUT = 0xffff-temp;
                    temp = temp << 1;
                    Delay(Time_led[count_time]);
                }
            }
            else                          //反向旋转
            {
                for(i=0;i<16;i++)
                {
                    PAOUT = 0xffff-temp;
                    temp = temp >> 1;
                    Delay(Time_led[count_time]);
                }
            }
        }
            
        for(count=0;count<50;count++)     //间隔旋转
        {
            if(count%2==0)
            {
                PAOUT = 0x5555;
            }
            else
            {
                PAOUT = 0xaaaa;   
            }
            Delay(Time_led[count_time]);
        }
        
        if(speed_up==0)
        {
            count_time++; 
            if(count_time==16)            //计数到16则为一个周期    
            {
                speed_up = 1;
            }
        }
        else
        {
            count_time--;
            if(count_time==0)                  
            {
                speed_up = 0;
            }
        }
    }
}
    
/************************************************************************
函数名：   void Delay(unsigned int time)
入口参数：unsigned int time  延时时间
出口参数：无
功能：    延时子程序
************************************************************************/
void Delay(unsigned int time)
{
    while(time--)
    {
        _NOP();
    }
}

