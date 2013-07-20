
#include "slop.h"

/***************************************************************
�������ƣ�void delay(int i)
�������ܣ���ʱ
��ڲ�������
����ֵ��  ��
****************************************************************/

void delay(int i)
{   
    int j;
    for(;i>0;i--)
      for(j=0;j<500;j++);
}
/***************************************************************
�������ƣ�unsigned char *led_data( int result)
�������ܣ���ʾ��λ�ֽ�洢
��ڲ�����int result
����ֵ��  unsigned char *
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
�������ƣ�void  Time_r_cc(void)
�������ܣ����¶�
��ڲ�������
����ֵ��  ��
****************************************************************/

void Measure_tempreture(void)
{  
    float Rx;
    int i;
    
    Cut_Rx;                                              //�Ͽ�P2.6
    P2DIR=BIT7;       
    SET_High_Rref;                                       //P2.7�ø�ͨ���ο�������

    delay(120);                                          //��ʱ�ȴ�������
    
    SET_Low_Rref;                                        //P2.7�õ�ͨ���ο�����ŵ�
    
    TACTL=TASSEL_2+TACLR+MC_2;     
    TACCTL1=CAP+CM_2+CCIE+CCIS_1;                        //��TimerA�жϲ����ŵ�ʱ��
    LPM0;
    //delay(100);                                          //��ʱ�ȴ��ŵ����
    
    SET_High_Rref;                                       //P2.7�ø�ͨ���ο�������

    delay(100);                                          //��ʱ�ȴ�������
    
    Cut_Rref;                                            //�Ͽ�P2.7
    P2DIR|=BIT6; 
    SET_Low_Rx;                                          //P2.6�õ�ͨ����������ŵ�
    
    TACTL=TASSEL_2+TACLR+MC_2;
    TACCTL1=CAP+CM_2+CCIE+CCIS_1;                        //��TimerA�жϲ����ŵ�ʱ��
    
    LPM0;
    //delay(100);
    
    Rx=((float)time_Rx/time_Rref)*10000;                 //������������
    
    for( i=0;i<35;i++)                                   //����r���ڷ�Χ
    {
        if(Rx>=TmpTable[i])break;
    }
    line_k=5.0/(TmpTable[i-1]-TmpTable[i]);             
    tempreture=((i-1)*5-40)+(TmpTable[i-1]-Rx)*line_k;   //�����¶�
}
