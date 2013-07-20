
#include "slop.h"

/***************************************************************
函数名称：void delay(int i)
函数功能：延时
入口参数：无
返回值：  无
****************************************************************/

void delay(int i)
{   
    int j;
    for(;i>0;i--)
      for(j=0;j<500;j++);
}
/***************************************************************
函数名称：unsigned char *led_data( int result)
函数功能：显示数位分解存储
入口参数：int result
返回值：  unsigned char *
****************************************************************/
unsigned char *led_data( int result)
{  
    unsigned char p[3];
    unsigned char *head_adr;
    head_adr=p;
    char a,k,n;
    int j;
    a=3;
    for(n=0;n<3;n++)
    {       
        j=1;
        for(k=1;k<a;k++)
        {
            j=j*10;
        }
        a=a-1;
        p[n]=result/j;
        result=result%j;
    }
    return head_adr;
}
/***************************************************************
函数名称：void  Time_r_cc(void)
函数功能：测温度
入口参数：无
返回值：  无
****************************************************************/

void Measure_tempreture(void)
{  
    float Rx;
    int i;
    
    Cut_Rx;                                              //断开P2.6
    P2DIR=BIT7;       
    SET_High_Rref;                                       //P2.7置高通过参考电阻充电

    delay(120);                                          //延时等待充电完成
    
    SET_Low_Rref;                                        //P2.7置低通过参考电阻放电
    
    TACTL=TASSEL_2+TACLR+MC_2;     
    TACCTL1=CAP+CM_2+CCIE+CCIS_1;                        //打开TimerA中断测量放电时间
    LPM0;
    //delay(100);                                          //延时等待放电完成
    
    SET_High_Rref;                                       //P2.7置高通过参考电阻充电

    delay(100);                                          //延时等待充电完成
    
    Cut_Rref;                                            //断开P2.7
    P2DIR|=BIT6; 
    SET_Low_Rx;                                          //P2.6置低通过热敏电阻放电
    
    TACTL=TASSEL_2+TACLR+MC_2;
    TACCTL1=CAP+CM_2+CCIE+CCIS_1;                        //打开TimerA中断测量放电时间
    
    LPM0;
    //delay(100);
    
    Rx=((float)time_Rx/time_Rref)*10000;                 //计算热敏电阻
    
    for( i=0;i<35;i++)                                   //查找r所在范围
    {
        if(Rx>=TmpTable[i])break;
    }
    line_k=5.0/(TmpTable[i-1]-TmpTable[i]);             
    tempreture=((i-1)*5-40)+(TmpTable[i-1]-Rx)*line_k;   //计算温度
}
