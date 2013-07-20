
//============================头 文 件=================================
#include"subroutine.h"
//=====================================================================

//=============================子程序定义==============================   

//=====================================================================
//函数名称：void delay(unsigned int time)
//型参类型：无符号整形
//返回类型：空
//函数功能：延时
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
//函数名称：void Init_Mcu(void)
//型参类型：空
//返回类型：空
//函数功能：完成Mcu的各寄存器的初始化工作
//=====================================================================
void Init_Mcu_Key(void)
{
  unsigned int i;                                   
  for(i=0;i<20000;i++);                                                // 等待时钟稳定
  P1OUT=0;                                                             // 端口二输出低电平，关中断使能，上升沿触发
  P1DIR=0XFF;
  P1IE=0;
  P1IES=0;
  P1IFG=0;    
  TACTL = TASSEL_2 + MC_2;                                             //开TAR ,时钟选择SMCLK, 计数方式选择为连续计数
  _EINT();
}

//=====================================================================
//函数名称：void Init_key(key_data_t *key,const key_config_t *key_config)
//型参类型：空
//返回类型：空
//函数功能：清零电容基本特性
//=====================================================================
void Init_Key(key_data_t *key,const key_config_data_t *key_config)
{ 
  key->base_capacitance = 0;                                            // 清基本电容量为0
  key->filtered = 0;                                                    // 清当前电容差值为0
  key->adapt = 0; 
  P1IFG=0;                
  P1IE=0;
}

//=====================================================================
//函数名称：unsigned int Measure_Key_Capacitance(unsigned int keyno)
//型参类型：unsigned int
//返回类型：unsigned int
//函数功能：先测触摸式按键的充电时间，再测触模式按键的放电时间，最后两个时间相加
//=====================================================================
unsigned int Measure_Key_Capacitance(unsigned int keyno)
{
   const key_config_data_t *keyp;                                        //定义当前按键结构指针                               
   keyp = &key_config[keyno];                                            //赋予当前按键结构指针相应的数值                                  
   
   P1IES=~keyp->port_n;                                                  //设置为上升沿中断
   P1IE=keyp->port_n;                                                    //中断使能   
   P1OUT=keyp->port_m;                                                   //当前端口输出高电平进入充电状态
   P1DIR=~keyp->port_n;                                                  //相应管脚置为输入模式
   _NOP();_NOP();_NOP();_NOP();
   cnt_time=TAR;                                                         //获取定时器基值
   LPM0;                                                                 //进入低功耗零
   _NOP();                                                             
   
   sum=cnt_time;                                                         //获得充电时间
   
   P1IES=keyp->port_n;                                                   //修改中断条件，设置为下降沿中断
   _NOP();_NOP();_NOP();
   cnt_time=TAR;                                                         //获取定时器基值
   
   P1OUT&=~keyp->port_m;
   _NOP();_NOP();_NOP();_NOP();//相应管脚置低进行放电
   LPM0;                                                                 //进入低功耗零
   _NOP();
   P1IE&=~keyp->port_n;                                                  //屏蔽响应中断
   
   sum+=cnt_time;                                                        //充放电时间相加
   return sum;                                                           //返回充放电时间
}
  
  
//=====================================================================
//函数名称：void Scan_Key(void)
//参数类型；空
//函数类型：空
//函数功能：扫描按键电容量的变化
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
//函数名称: int find_mean_position(void)
//形参类型: void
//函数类型: int
//函数功能: 获取当前按键值
//=====================================================================
    
int find_mean_position(void)
{
    long min;                                                         //最小按键值
    long max;                                                         //最大按键值                                       
    int id_max_pos;                                                   //最大按键值的按键编号
    int key_threshold;                                                //按键阀值
    int i; 
      
    key_threshold=200;                                                 

    min = 32767;                                                      //设置最小电容量的初值
    max = -32768;                                                     //设置最大电容量的初值
    id_max_pos = -1;                                                  //设置最大电容量位置的初值
                             
    for (i = 0;  i < NUM_KEYS;  i++)                                  //查询最小电容量,最大电容量和最大电容量的位置并记录
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
    if (max < key_threshold)                                           // 按键自适应处理基电容处理
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
       
        return -1;                                                   //没有按键按下
    }
    return id_max_pos;
}
//=====================================================================
//函数名称: void Lcd_Display(int reg_key_value)
//型参类型：空
//返回类型：空
//函数功能：把当前键值显示出来
//=====================================================================
void Lcd_Display(int reg_key_value)
{
   unsigned int i;
   if(reg_key_value == -1)                                             // 如果当前没有任何按键按下,则LCD清屏             
   {
       for(i=0;i<4;i++)
       Segment_Display(i,Num_Code+12,1 );
   }
   else                                                               // 显示当前键值,0~255,高位在前
   {   
     Segment_Display( reg_key_value,Num_Code+reg_key_value,1 );      
   }
}

//=====================================================================
//函数名称：void Port2_ISR(void)
//型参类型：空
//返回类型：空
//函数功能：读取触摸式按键的充放电时间
//======================================================================

#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
  cnt_time = TAR - cnt_time;
  P1IFG=0;
  LPM0_EXIT;
}

