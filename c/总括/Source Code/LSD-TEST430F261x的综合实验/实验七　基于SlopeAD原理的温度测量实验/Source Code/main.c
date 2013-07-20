
//**********************************************************
//** ��Ȩ:  ����������Ƽ����޹�˾
//** �ļ���: slop.c
//** �汾:1.0
//** ��������: IAR 4.11 
//** ����: ZHAO YUAN GUO
//** ��������: 2008.11
//** ����: slop AD �¶Ȳ���
//** ���ñȽ���ʵ���¶Ȳ���
//** �޸���־����
/*************************************************************
      �ӿ����ӣ� P2.7<-------------->Rref
                 P2.6<-------------->Rx
                 P2.4<-------------->CAP_Vx
*/
//*********************************************************** 

#include "msp430x26x.h"
#include "Disp.c"
#include "slop.c"

/***************************************************************
�������ƣ�main( void )
�������ܣ�������
��ڲ�������
����ֵ��  ��
****************************************************************/

int main( void )
{                                          
    unsigned  char cnt_point=0;
    int temp;
    unsigned char *data_pr;
    P2SEL |= BIT4; 
    CACTL1=CARSEL+CAREF_1+CAON;                           //�Ƚ�������
    CACTL2=P2CA4+CAF;
    Init_MCU();                                           //��ʼ��MCU 
    WDTCTL=WDT_ADLY_1000;                                 //���Ź����ã���ʱһ��
    Init_BU9796FS();                                      //��ʼ��LCD����
    P5OUT |= BIT7;                                        //����ָʾ��
    IE1=WDTIE;                                            //ʹ�ܿ��Ź��ж�
    _EINT();                                              //���ж�                                                                                           
    
    while(1)
    {           
        if( Nable_measure_flg==1)
        {   
            P5OUT|=BIT7;                                  //����ָʾ��
            Nable_measure_flg=0;
            Measure_tempreture();                         //���¶�
            temp=(int)tempreture;
            while(temp=temp/10)                           //��������λ
            {
                cnt_point++;
            }
            cnt_point++;
            temp=(int)(tempreture*10);
            data_pr=led_data(temp);                       //��ʾ��λ�ֽ⣬�����׵�ַ
            for(unsigned char k=0;k<3;k++)                //��ʾ����
            {   
                Segment_Display( k,Num_Code+(*data_pr),cnt_point );
                data_pr++;                                
            }
            cnt_point=0;
            P5OUT&=~BIT7;                                //Ϩ��ָʾ��
        }                                              
    }
}
/***************************************************************
�������ƣ�void T_A1(void)
�������ܣ�Timer�жϷ������
��ڲ�������
����ֵ��  ��
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
�������ƣ�void WDT_timer(void)
�������ܣ����Ź���ʱ���жϷ������
��ڲ�������
����ֵ��  ��
****************************************************************/
#pragma  vector=WDT_VECTOR
__interrupt  void WDT_timer(void)
{
    IFG1=0;
    cnt_time++;
    if(cnt_time%2==0)                                     //ÿ���������һ�β���
        Nable_measure_flg=1;                              //��λ����ʹ�ܱ�־   
}
