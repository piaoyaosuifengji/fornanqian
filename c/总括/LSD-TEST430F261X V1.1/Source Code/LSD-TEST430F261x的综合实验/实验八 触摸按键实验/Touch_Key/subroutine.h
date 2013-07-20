
#ifndef __SUBROUTINE_H_
#define __SUBROUTINE_H_
//===========================常量定义================================

#define NUM_KEYS 4


//===================================================================

//===========================变量声明================================

unsigned int cnt_time;
unsigned int sum;
signed   int reg_key_value;    
//===================================================================

//==========================数据结构定义=============================
typedef struct
{
  unsigned char port_m;                                        //端口号
  unsigned char port_n;                                        //端口号
}key_config_data_t;

typedef struct
{
   int base_capacitance;                                      //存储按键的基本电容量
   int filtered;                                              //存储滤波后的增益电容量
   int adapt;                                                 //存储慢速适应变化值
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

//==========================子函数声明===============================

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
