
//============================ͷ �� ��=================================
#include"subroutine.h"
//=====================================================================

//=============================�ӳ�����==============================   

//=====================================================================
//�������ƣ�void delay(unsigned int time)
//�Ͳ����ͣ��޷�������
//�������ͣ���
//�������ܣ���ʱ
//=====================================================================
void delay(unsigned int time)
{
  unsigned int i;
  for(i=0;i<time;i++)
  {
    _NOP();
  }
}
//=====================================================================
//�������ƣ�void Init_Mcu(void)
//�Ͳ����ͣ���
//�������ͣ���
//�������ܣ����Mcu�ĸ��Ĵ����ĳ�ʼ������
//=====================================================================
void Init_Mcu_Key(void)
{
  unsigned int i;                                   
  for(i=0;i<20000;i++);                                                // �ȴ�ʱ���ȶ�
  P1OUT=0;                                                             // �˿ڶ�����͵�ƽ�����ж�ʹ�ܣ������ش���
  P1DIR=0XFF;
  P1IE=0;
  P1IES=0;
  P1IFG=0;    
  TACTL = TASSEL_2 + MC_2;                                             //��TAR ,ʱ��ѡ��SMCLK, ������ʽѡ��Ϊ��������
  _EINT();
}

//=====================================================================
//�������ƣ�void Init_key(key_data_t *key,const key_config_t *key_config)
//�Ͳ����ͣ���
//�������ͣ���
//�������ܣ�������ݻ�������
//=====================================================================
void Init_Key(key_data_t *key,const key_config_data_t *key_config)
{ 
  key->base_capacitance = 0;                                            // �����������Ϊ0
  key->filtered = 0;                                                    // �嵱ǰ���ݲ�ֵΪ0
  key->adapt = 0; 
  P1IFG=0;                
  P1IE=0;
}

//=====================================================================
//�������ƣ�unsigned int Measure_Key_Capacitance(unsigned int keyno)
//�Ͳ����ͣ�unsigned int
//�������ͣ�unsigned int
//�������ܣ��Ȳⴥ��ʽ�����ĳ��ʱ�䣬�ٲⴥģʽ�����ķŵ�ʱ�䣬�������ʱ�����
//=====================================================================
unsigned int Measure_Key_Capacitance(unsigned int keyno)
{
   const key_config_data_t *keyp;                                        //���嵱ǰ�����ṹָ��                               
   keyp = &key_config[keyno];                                            //���赱ǰ�����ṹָ����Ӧ����ֵ                                  
   
   P1IES=~keyp->port_n;                                                  //����Ϊ�������ж�
   P1IE=keyp->port_n;                                                    //�ж�ʹ��   
   P1OUT=keyp->port_m;                                                   //��ǰ�˿�����ߵ�ƽ������״̬
   P1DIR=~keyp->port_n;                                                  //��Ӧ�ܽ���Ϊ����ģʽ
   _NOP();_NOP();_NOP();_NOP();
   cnt_time=TAR;                                                         //��ȡ��ʱ����ֵ
   LPM0;                                                                 //����͹�����
   _NOP();                                                             
   
   sum=cnt_time;                                                         //��ó��ʱ��
   
   P1IES=keyp->port_n;                                                   //�޸��ж�����������Ϊ�½����ж�
   _NOP();_NOP();_NOP();
   cnt_time=TAR;                                                         //��ȡ��ʱ����ֵ
   
   P1OUT&=~keyp->port_m;
   _NOP();_NOP();_NOP();_NOP();//��Ӧ�ܽ��õͽ��зŵ�
   LPM0;                                                                 //����͹�����
   _NOP();
   P1IE&=~keyp->port_n;                                                  //������Ӧ�ж�
   
   sum+=cnt_time;                                                        //��ŵ�ʱ�����
   return sum;                                                           //���س�ŵ�ʱ��
}
  
  
//=====================================================================
//�������ƣ�void Scan_Key(void)
//�������ͣ���
//�������ͣ���
//�������ܣ�ɨ�谴���������ı仯
//=====================================================================
void Scan_Key(void)
{
  int i;
  int margin;
  for(i=0;i<NUM_KEYS;i++)
  {
    margin = Measure_Key_Capacitance(i)-key[i].base_capacitance;
    key[i].filtered += (margin-(key[i].filtered>>4));
  }
}
//=====================================================================
//��������: int find_mean_position(void)
//�β�����: void
//��������: int
//��������: ��ȡ��ǰ����ֵ
//=====================================================================
    
int find_mean_position(void)
{
    long min;                                                         //��С����ֵ
    long max;                                                         //��󰴼�ֵ                                       
    int id_max_pos;                                                   //��󰴼�ֵ�İ������
    int key_threshold;                                                //������ֵ
    int i; 
      
    key_threshold=200;                                                 

    min = 32767;                                                      //������С�������ĳ�ֵ
    max = -32768;                                                     //�������������ĳ�ֵ
    id_max_pos = -1;                                                  //������������λ�õĳ�ֵ
                             
    for (i = 0;  i < NUM_KEYS;  i++)                                  //��ѯ��С������,��������������������λ�ò���¼
    {
        if (key[i].filtered < min)
            min = key[i].filtered;
        if (key[i].filtered > max)
        {
            max = key[i].filtered;
            id_max_pos = i;
        }
    }
    max>>=2;
    if (max < key_threshold)                                           // ��������Ӧ��������ݴ���
    {                                 
        for (i = 0;  i < NUM_KEYS;  i++)                               
        {
            if(key[i].filtered>0)    
              key[i].adapt+=8;
            else
              key[i].adapt+=-2;
                                   
            if(key[i].adapt>320)
            {  
              ++key[i].base_capacitance;
              key[i].adapt=0;             
            }
            else if(key[i].adapt<-320)
            {
              --key[i].base_capacitance;             
              key[i].adapt=0;                 
            }             
        }
       
        return -1;                                                   //û�а�������
    }
    return id_max_pos;
}
//=====================================================================
//��������: void Lcd_Display(int reg_key_value)
//�Ͳ����ͣ���
//�������ͣ���
//�������ܣ��ѵ�ǰ��ֵ��ʾ����
//=====================================================================
void Lcd_Display(int reg_key_value)
{
   unsigned int i;
   if(reg_key_value == -1)                                             // �����ǰû���κΰ�������,��LCD����             
   {
       for(i=0;i<4;i++)
       Segment_Display(i,Num_Code+12,1 );
   }
   else                                                               // ��ʾ��ǰ��ֵ,0~255,��λ��ǰ
   {   
     Segment_Display( reg_key_value,Num_Code+reg_key_value,1 );      
   }
}

//=====================================================================
//�������ƣ�void Port2_ISR(void)
//�Ͳ����ͣ���
//�������ͣ���
//�������ܣ���ȡ����ʽ�����ĳ�ŵ�ʱ��
//======================================================================

#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
  cnt_time = TAR - cnt_time;
  P1IFG=0;
  LPM0_EXIT;
}

