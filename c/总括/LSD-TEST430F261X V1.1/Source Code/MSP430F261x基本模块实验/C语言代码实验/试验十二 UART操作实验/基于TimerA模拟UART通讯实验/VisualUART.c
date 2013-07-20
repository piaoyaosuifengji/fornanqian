//**********************************************************
//** ��Ȩ:  ����������Ƽ����޹�˾
//** �ļ���: visualUART.c
//** �汾:1.0
//** ��������: IAR 4.11B 
//** ����: LSD
//** ��������: 2008.12.05
//** ����: TimerAģ��UART żУ�������񴮿����ַ���0xAA,0xBB,0xCC
//** �޸���־����
//*********************************************************


#include <msp430x26x.h>

//********************��������*****************************
#define  TXD      BIT7          //P2.7=TXD  
#define  RXD      BIT2          //P2.2=RXD  

#define  OneBit   0x341         //8M/9600=0x341
#define  HalfBit  0x1A0
unsigned int  TxDate=0;
unsigned int  RxDate=0;
unsigned char Count_R;
unsigned char Count_T;
unsigned char ParityBit=0;
unsigned char Movebit=0x01;
unsigned char verify_fault;
//*******************��������******************************
void init_TimerA(void);
void init_TXD(unsigned int);
void init_RXD(void);

//************************************************************
//---------------------������-------------------------------
//************************************************************
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    BCSCTL1 = CALBC1_8MHZ;               
    DCOCTL  = CALDCO_8MHZ;
    
    P4OUT &= ~BIT1;
    P4DIR |= BIT1;                  //�������д���ģ��ĵ�Դ
    
    init_TimerA();
    _EINT();
    while(1)
    {
        init_RXD();                   //��ʼ�����ȴ��������ַ�������
        LPM0;
        if(verify_fault)              //�ж�У���Ƿ����
        {
            init_TXD(0xAA);
            init_TXD(0xBB);
            init_TXD(0xCC);             //żУ�������������0xAA,0xBB,0xCC����������
            verify_fault=0;
        }
        else
        {
            RxDate = (RxDate & 0xff);   //ֻȡ8λ����λ
            init_TXD(RxDate);
        }
    }
}


/*************************************************
����(ģ��)����:init_TimerA
����:	��ʼ��TA
�������: ��     	          		
�������: ��  	
��������ֵ˵��:�� 		
ʹ�õ���Դ��   		
��˵��: ��
*************************************************/
void init_TimerA(void)
{
    TACTL |= TASSEL1+MC1+TACLR;       //SMCLK ,������������TAR
    CCTL0 |= OUT;                     //Ĭ�ϱȽ����Ϊ1
    P2DIR |= TXD;
    P2SEL |= TXD+RXD;
}
/*************************************************
����(ģ��)����:init_TXD
����:	��ʼ������
�������: ��     	          		
�������: ��  	
��������ֵ˵��:�� 		
ʹ�õ���Դ��   		
��˵��: ��
*************************************************/
void init_TXD(unsigned int Byte)
{
    Count_T = 11;                   //��ʼλ��8λ����λ����żУ��λ��ֹͣλ
    CCR0 = TAR + OneBit;
    //---------------------ʹ���͵����ݷ���żУ��Ĺ���--------------------
    for(char i=0;i<8;i++)
    {
        if(Movebit & Byte)
        {
            ParityBit++;
        }
        Movebit <<= 1;
    }
    Movebit = 0x01;
    if(ParityBit%2 == 0)         
    {                               
        TxDate |=0x0200+Byte;      //0010 xxxx xxxx                    
    }
    else
    {
        TxDate |=0x0300+Byte;      //0011 xxxx xxxx 
    }                                   
    ParityBit = 0;
    //------------------------------------------------------------------- 
    
    TxDate=TxDate<<1;               //������1λ��������ʼλ
    
    CCTL0 = OUTMOD0 + CCIE;         //��λ���Ƚ��ж�����                
    LPM0;
    
}
/*************************************************
����(ģ��)����:init_RXD
����:	��ʼ������
�������: ��     	          		
�������: ��  	
��������ֵ˵��:�� 		
ʹ�õ���Դ��   		
��˵��: ��
*************************************************/
void init_RXD(void)
{
    Count_R=10;                     //8λ����+1λżУ��+ֹͣλ
    /*��λ���ж������½��ز���ͬ������ѡ��CCI0B������Դ*/
    CCTL0 = OUTMOD0+CCIE+CM1+CAP+SCS+CCIS0;
    
}

/*************************************************
����(ģ��)����:TA�жϺ���
����:	���񣬶�ʱ
�������: ��     	          		
�������: ��  	
��������ֵ˵��:�� 		
ʹ�õ���Դ��   		
��˵��: ��
*************************************************/

#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA(void)
{
    CCR0 += OneBit;
    if(CCTL0 & CCIS0)            //����״̬
    {
        if(CCTL0 & CAP)            //���ڲ���״̬����ʼ��������
        {
            CCTL0 &= ~CAP;           //ת��ȽϷ�ʽ
            CCR0 += HalfBit;         //�ټӰ�λ��ʱ��
        }
        else                       //���ڽ�������
        {
            if(Count_R!=0)           //8�����ݣ�1��żУ�飬1��ֹͣλ û�н������
            {   
                RxDate=RxDate>>1; 
                if(CCTL0 & SCCI)       //���յ�����1
                {
                    RxDate |= 0x0200;    //������10λ
                    ParityBit ++;     
                }
                if(Count_R==1)         //���յ������һλ��ֹͣλ
                {
                    if( !(RxDate & 0x0200))//���һλ����1(ֹͣλ����)
                    {
                        verify_fault = 1;
                    }
                }
                Count_R--;            
            }
            else                      //���ݽ������
            {
                CCTL0 &= ~CCIE;
                if( !(ParityBit&0x01) ) //9λ������ 1�ĸ�����ż������Ϊ������ֹͣλ��
                {
                    verify_fault = 1;    //żУ��ʧ��
                }
                ParityBit = 0;
                LPM0_EXIT;             //�˳�LPM0��CPU��ʼ����Ҫ���͵�����
            } //���ݽ������
            
        }     //���ڽ�������
    }         //����״̬
    else                        //���ڷ���״̬
    {
        if(Count_T!=0)            //8λ����+1λżУ��+ֹͣλ  û�з������
        {   
            if(TxDate & 0x0001)     //���͵���1
            {
                CCTL0 &= ~OUTMOD2;    //OUTMOD0����λ
            }               
            else                    //���͵���0
            {
                CCTL0 |= OUTMOD2;     //OUTMOD0+OUTMOD2�Ǹ�λ
            }
            
            TxDate=TxDate>>1;
            Count_T--;
        }
        else                      //8λ����+1λżУ��+ֹͣλ �������
        {
            CCTL0 &= ~CCIE;
            LPM0_EXIT;  
        }
        
    }  
}

