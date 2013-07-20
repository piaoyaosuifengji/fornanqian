//**********************************************************
//** 版权:  杭州利尔达科技有限公司
//** 文件名: visualUART.c
//** 版本:1.0
//** 工作环境: IAR 4.11B 
//** 作者: LSD
//** 生成日期: 2008.12.05
//** 功能: TimerA模拟UART 偶校验出错就像串口助手发送0xAA,0xBB,0xCC
//** 修改日志：无
//*********************************************************


#include <msp430x26x.h>

//********************变量声明*****************************
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
//*******************函数声明******************************
void init_TimerA(void);
void init_TXD(unsigned int);
void init_RXD(void);

//************************************************************
//---------------------主函数-------------------------------
//************************************************************
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    BCSCTL1 = CALBC1_8MHZ;               
    DCOCTL  = CALDCO_8MHZ;
    
    P4OUT &= ~BIT1;
    P4DIR |= BIT1;                  //开启所有传输模块的电源
    
    init_TimerA();
    _EINT();
    while(1)
    {
        init_RXD();                   //初始化，等待串口助手发送数据
        LPM0;
        if(verify_fault)              //判断校验是否出错
        {
            init_TXD(0xAA);
            init_TXD(0xBB);
            init_TXD(0xCC);             //偶校验出错连续发送0xAA,0xBB,0xCC给串口助手
            verify_fault=0;
        }
        else
        {
            RxDate = (RxDate & 0xff);   //只取8位数据位
            init_TXD(RxDate);
        }
    }
}


/*************************************************
函数(模块)名称:init_TimerA
功能:	初始化TA
输入参数: 无     	          		
输出参数: 无  	
函数返回值说明:无 		
使用的资源：   		
它说明: 无
*************************************************/
void init_TimerA(void)
{
    TACTL |= TASSEL1+MC1+TACLR;       //SMCLK ,连续记数，清TAR
    CCTL0 |= OUT;                     //默认比较输出为1
    P2DIR |= TXD;
    P2SEL |= TXD+RXD;
}
/*************************************************
函数(模块)名称:init_TXD
功能:	初始化发送
输入参数: 无     	          		
输出参数: 无  	
函数返回值说明:无 		
使用的资源：   		
它说明: 无
*************************************************/
void init_TXD(unsigned int Byte)
{
    Count_T = 11;                   //起始位，8位数据位，奇偶校验位，停止位
    CCR0 = TAR + OneBit;
    //---------------------使发送的数据符合偶校验的规则--------------------
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
    
    TxDate=TxDate<<1;               //往左移1位，产生起始位
    
    CCTL0 = OUTMOD0 + CCIE;         //置位，比较中断允许                
    LPM0;
    
}
/*************************************************
函数(模块)名称:init_RXD
功能:	初始化接收
输入参数: 无     	          		
输出参数: 无  	
函数返回值说明:无 		
使用的资源：   		
它说明: 无
*************************************************/
void init_RXD(void)
{
    Count_R=10;                     //8位数据+1位偶校验+停止位
    /*置位，中断允许，下降沿捕获，同步捕获，选择CCI0B作输入源*/
    CCTL0 = OUTMOD0+CCIE+CM1+CAP+SCS+CCIS0;
    
}

/*************************************************
函数(模块)名称:TA中断函数
功能:	捕获，定时
输入参数: 无     	          		
输出参数: 无  	
函数返回值说明:无 		
使用的资源：   		
它说明: 无
*************************************************/

#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA(void)
{
    CCR0 += OneBit;
    if(CCTL0 & CCIS0)            //接收状态
    {
        if(CCTL0 & CAP)            //处于捕获状态，开始接收数据
        {
            CCTL0 &= ~CAP;           //转变比较方式
            CCR0 += HalfBit;         //再加半位的时间
        }
        else                       //正在接收数据
        {
            if(Count_R!=0)           //8个数据，1个偶校验，1个停止位 没有接收完毕
            {   
                RxDate=RxDate>>1; 
                if(CCTL0 & SCCI)       //接收到的是1
                {
                    RxDate |= 0x0200;    //共接收10位
                    ParityBit ++;     
                }
                if(Count_R==1)         //接收到的最后一位是停止位
                {
                    if( !(RxDate & 0x0200))//最后一位不是1(停止位出错)
                    {
                        verify_fault = 1;
                    }
                }
                Count_R--;            
            }
            else                      //数据接收完毕
            {
                CCTL0 &= ~CCIE;
                if( !(ParityBit&0x01) ) //9位数据里 1的个数是偶数（因为包括了停止位）
                {
                    verify_fault = 1;    //偶校验失败
                }
                ParityBit = 0;
                LPM0_EXIT;             //退出LPM0，CPU开始处理要发送的数据
            } //数据接收完毕
            
        }     //正在接收数据
    }         //接收状态
    else                        //处于发送状态
    {
        if(Count_T!=0)            //8位数据+1位偶校验+停止位  没有发送完毕
        {   
            if(TxDate & 0x0001)     //发送的是1
            {
                CCTL0 &= ~OUTMOD2;    //OUTMOD0是置位
            }               
            else                    //发送的是0
            {
                CCTL0 |= OUTMOD2;     //OUTMOD0+OUTMOD2是复位
            }
            
            TxDate=TxDate>>1;
            Count_T--;
        }
        else                      //8位数据+1位偶校验+停止位 发送完毕
        {
            CCTL0 &= ~CCIE;
            LPM0_EXIT;  
        }
        
    }  
}

