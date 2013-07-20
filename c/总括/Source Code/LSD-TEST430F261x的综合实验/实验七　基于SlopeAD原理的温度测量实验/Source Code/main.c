
//**********************************************************
//** 版权:  杭州利尔达科技有限公司
//** 文件名: slop.c
//** 版本:1.0
//** 工作环境: IAR 4.11 
//** 作者: ZHAO YUAN GUO
//** 生成日期: 2008.11
//** 功能: slop AD 温度测量
//** 利用比较器实现温度测量
//** 修改日志：无
/*************************************************************
      接口连接： P2.7<-------------->Rref
                 P2.6<-------------->Rx
                 P2.4<-------------->CAP_Vx
*/
//*********************************************************** 

#include "msp430x26x.h"
#include "Disp.c"
#include "slop.c"

/***************************************************************
函数名称：main( void )
函数功能：主函数
入口参数：无
返回值：  无
****************************************************************/

int main( void )
{                                          
    unsigned  char cnt_point=0;
    int temp;
    unsigned char *data_pr;
    P2SEL |= BIT4; 
    CACTL1=CARSEL+CAREF_1+CAON;                           //比较器设置
    CACTL2=P2CA4+CAF;
    Init_MCU();                                           //初始化MCU 
    WDTCTL=WDT_ADLY_1000;                                 //看门狗设置，定时一秒
    Init_BU9796FS();                                      //初始化LCD驱动
    P5OUT |= BIT7;                                        //点亮指示灯
    IE1=WDTIE;                                            //使能看门狗中断
    _EINT();                                              //开中断                                                                                           
    
    while(1)
    {           
        if( Nable_measure_flg==1)
        {   
            P5OUT|=BIT7;                                  //点亮指示灯
            Nable_measure_flg=0;
            Measure_tempreture();                         //测温度
            temp=(int)tempreture;
            while(temp=temp/10)                           //计算整数位
            {
                cnt_point++;
            }
            cnt_point++;
            temp=(int)(tempreture*10);
            data_pr=led_data(temp);                       //显示数位分解，返回首地址
            for(unsigned char k=0;k<3;k++)                //显示数据
            {   
                Segment_Display( k,Num_Code+(*data_pr),cnt_point );
                data_pr++;                                
            }
            cnt_point=0;
            P5OUT&=~BIT7;                                //熄灭指示灯
        }                                              
    }
}
/***************************************************************
函数名称：void T_A1(void)
函数功能：Timer中断服务程序
入口参数：无
返回值：  无
****************************************************************/

#pragma  vector=TIMERA1_VECTOR
__interrupt void T_A1(void)
{ 
    switch(TAIV)
    { 
      case 4: break;
      case 2: 
                if(choice_flg==0)
                {
                    time_Rref=TACCR1;
                    choice_flg=1;break;         
                }
                else if(choice_flg==1)
                {
                    time_Rx=TACCR1;
                    choice_flg=0; break;        
                }  
      case 10:  break;
      default:break;
    }
    LPM0_EXIT;
}
/***************************************************************
函数名称：void WDT_timer(void)
函数功能：看门狗定时器中断服务程序
入口参数：无
返回值：  无
****************************************************************/
#pragma  vector=WDT_VECTOR
__interrupt  void WDT_timer(void)
{
    IFG1=0;
    cnt_time++;
    if(cnt_time%2==0)                                     //每隔两秒进行一次测量
        Nable_measure_flg=1;                              //置位测量使能标志   
}
