
#ifndef __SUBROUTINE_H_
#define __SUBROUTINE_H_
//===========================��������================================

#define NUM_KEYS 4


//===================================================================

//===========================��������================================

unsigned int cnt_time;
unsigned int sum;
signed   int reg_key_value;    
//===================================================================

//==========================���ݽṹ����=============================
typedef struct
{
  unsigned char port_m;                                        //�˿ں�
  unsigned char port_n;                                        //�˿ں�
}key_config_data_t;

typedef struct
{
   int base_capacitance;                                      //�洢�����Ļ���������
   int filtered;                                              //�洢�˲�������������
   int adapt;                                                 //�洢������Ӧ�仯ֵ
}key_data_t;

const key_config_data_t key_config[NUM_KEYS]=
{
  {BIT1,BIT0},//0
  {BIT0,BIT1},//1
  {BIT3,BIT2},//2
  {BIT2,BIT3} //3
};

const key_config_data_t key_partner_config[NUM_KEYS]=
{
  {BIT1,BIT0},//0
  {BIT0,BIT1},//1
  {BIT3,BIT2},//2
  {BIT2,BIT3} //3
};

key_data_t key[NUM_KEYS];

  
  
  
//===================================================================

//==========================�Ӻ�������===============================

//__interrupt void Port1_ISR(void);
//__interrupt void Port2_ISR(void);
void Init_Mcu_Key(void);
void Init_Key(key_data_t *key,const key_config_data_t *key_config);
void delay(unsigned int time);
unsigned int Measure_Key_Capacitance(unsigned int keyno);
void Scan_Key(void);
//int Key_Position(void);
void Lcd_Display(int reg_key_value);
void Uart_Tx(int reg_key_value);
//===================================================================

#endif
