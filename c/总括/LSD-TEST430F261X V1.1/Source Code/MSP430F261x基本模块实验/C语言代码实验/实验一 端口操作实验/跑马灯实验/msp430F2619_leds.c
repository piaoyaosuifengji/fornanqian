//*******************************************************************
//** ��Ȩ:     ������Ƽ����޹�˾
//** �ļ���:   msp430F2619_leds.c
//** �汾��    V1.0
//** ��������: IAR Embedded Workbench IDE 4.11A
//** ����:     ����
//** ��������: 2008.11.12
//** ����:     ͨ��PA��P7��P8�������ķ��������ʵ������ƹ���
//** ����ļ�:  
//** �޸���־: ��
//*******************************************************************

#include  <msp430x26x.h>

void Delay(unsigned int time);

unsigned int FLASH_led[]=              //����ƻ���
                         {0x0001,0x8000,0x0003,0xc000,0x0007,0xe000,
                          0x000f,0xf000,0x001f,0xf800,0x003f,0xfc00,
                          0x007f,0xfe00,0x00ff,0xff00,0x01ff,0xff80,
                          0x03ff,0xffc0,0x07ff,0xffe0,0x0fff,0xfff0,
                          0x1fff,0xfff8,0x3fff,0xfffc,0x7fff,0xfffe,
                          0xffff,0xffff,0x0005,0xa000,0x0015,0xa800,
                          0x0055,0xaa00,0x0155,0xaa80,0x0555,0xaaa0,
                          0x1555,0xaaa8,0x5555,0xaaaa};
unsigned int Time_led[]=             //������ٶ�
                         {0x0fff,0x1fff,0x2fff,0x3fff,0x4fff,0x5fff,
                          0x6fff,0x7fff,0x8fff,0x9fff,0xafff,0xbfff,
                          0xcfff,0xdfff,0xefff,0xffff};

/************************************************************************
��������   main (void)
��ڲ�������
���ڲ�������
���ܣ�    ��������ڣ���ʼ����ض˿�
************************************************************************/
int main(void)
{
    unsigned char i;
    unsigned char count;                   //����ƻ�������
    unsigned char count_time=0;            //�����ʱ��ѭ������
    unsigned char speed_up=0;             //���ٱ�־λ
    unsigned int temp;
    
    WDTCTL = WDTPW + WDTHOLD;              //�ؿ��Ź�
    P3DIR |= BIT2;                         //ʹ������Ƶĵ�Դ�ź�
    P3OUT &= ~BIT2;
    P8SEL = 0X00;                         //����P8.6��P8.7
    PADIR = 0xffff; 
    PAOUT = 0x0000;                      //����ȫ��LED�����ڲ���  
    for(i=0;i<10;i++)
    {
        Delay(0xffff);
    }
    
    while(1)
    {
        for(count=0;count<46;count++)         
        {
            temp=FLASH_led[count];
            if(count%2==0)                //������ת
            {
                for(i=0;i<16;i++)
                {
                    PAOUT = 0xffff-temp;
                    temp = temp << 1;
                    Delay(Time_led[count_time]);
                }
            }
            else                          //������ת
            {
                for(i=0;i<16;i++)
                {
                    PAOUT = 0xffff-temp;
                    temp = temp >> 1;
                    Delay(Time_led[count_time]);
                }
            }
        }
            
        for(count=0;count<50;count++)     //�����ת
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
            if(count_time==16)            //������16��Ϊһ������    
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
��������   void Delay(unsigned int time)
��ڲ�����unsigned int time  ��ʱʱ��
���ڲ�������
���ܣ�    ��ʱ�ӳ���
************************************************************************/
void Delay(unsigned int time)
{
    while(time--)
    {
        _NOP();
    }
}

