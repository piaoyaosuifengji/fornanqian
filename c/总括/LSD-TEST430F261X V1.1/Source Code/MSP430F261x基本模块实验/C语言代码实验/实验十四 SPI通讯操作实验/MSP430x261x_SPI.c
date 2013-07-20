/*************************************************************************
** 版权:     杭州利尔达科技有限公司
** 文件名:   main
** 工作环境: IAR 4.11B
** 作者:     谷丰
** 生成日期: 2008.11.10
** 功能:     使用SPI接口读写EEPROM器件W25X16
             系统主时钟为12M，波特率为4M
** 相关文件: 
** 修改日志：
**************************************************************************/
#include  <msp430x26x.h>

#define FALSE	0
#define TRUE	1
#define CS_H    (P5OUT |= BIT0)
#define CS_L    (P5OUT &= ~BIT0)
unsigned char send_buff[256]=
{
    0x11,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x11,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x12,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x13,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x14,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x15,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x16,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x17,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x18,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x19,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x1a,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x1b,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x1c,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x1d,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x1e,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
    0x1f,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X0A,0X0B,0X0C,0X0D,0X0E,0X0F,
};
unsigned char data_buff[256];
unsigned int temp;
char ManuId,DevId;
unsigned char Read_Status_Register(void);
void write_status(unsigned char data);
void WREN(unsigned char data);
char QueryUC_Reg(char bit);
unsigned int read_ID(char dat);
void Send_Data(unsigned char data);
void Read_Data(char dat);
void Chip_Erase(unsigned char data);
void Delay(unsigned int dly);
void write_rom(unsigned char data);
void read_rom(unsigned char data);
void fast_read(unsigned char data);
/*************************************************************************
** 版权:     杭州利尔达科技有限公司
** 文件名:   main.c
** 工作环境: IAR 4.11B 
** 作者:     谷丰
** 生成日期: 2008.11.10
** 功能:     读写SPI接口的FLASH储器W25X16
** 相关文件: 
**************************************************************************/
void main(void)
{
  volatile unsigned int i;
  WDTCTL = WDTPW+WDTHOLD;                      // 关闭看门狗
  BCSCTL1 &= ~XT2OFF;                          // 开启XT2
  BCSCTL3 |= XT2S_2;                           // 选择驱动模式
  do
  {
    IFG1 &=~OFIFG; 			                  // 清除振荡器失效标志
    for(i=0xff; i>0; i--); 	                  // 等待XT2起振
  }
  while((IFG1&OFIFG)!=0); 		              // 判断是否起振
  BCSCTL2 |= SELS + DIVS_0 + SELM_2 + DIVM_0;           
  P4DIR |= BIT5;
  P4OUT &= ~BIT5;
  P5SEL |= BIT4 + BIT5 + BIT6;		           // SMCLK MCLK ACLK OUTPUT
  P5DIR |= BIT4 + BIT5 + BIT6;
  P1DIR |= BIT0;							// LED OUTPUT
  P5SEL |= BIT1 + BIT2 + BIT3;		       // SEL USCIB1
  P5DIR |= BIT0 + BIT1 + BIT3;		       // CLK SIMO STE IS OUTPUT
  UCB1CTL1 |= UCSSEL_2 + UCSWRST;
  UCB1CTL0 |= UCMST + UCSYNC + UCMSB + UCCKPL;					
  UCB1BR0 = 0x03;								
  UCB1BR1 = 0;							
  UCB1CTL1 &= ~UCSWRST;					
  WREN(0X06);                                  // 写使能 
  Chip_Erase(0XC7);                            // 擦除命令
  while(1)
  {
    WREN(0X06);                                // 写使能
    write_rom(0x02);                           // 写ROM寄存器
    read_rom(0x03);                            // 读状态寄存器
    Delay(5000);
  }
}
/*************************************************************************
** 函数名称：WREN
** 工作环境: IAR 4.11B 
** 作者:     谷丰
** 生成日期: 2008.11.10
** 功能:写使能 
** 相关文件:
** 修改日志：
*************************************************************************/
void WREN(unsigned char data)
{
    CS_L;
    Send_Data(data);                         
    Delay(100);
    CS_H;  
}
/*************************************************************************
** 函数名称：Chip_Erase
** 工作环境: IAR 4.11B 
** 作者:     谷丰
** 生成日期: 2008.11.10
** 功能:全部寄存器擦除
** 相关文件:
** 修改日志：
*************************************************************************/
void  Chip_Erase(unsigned char data)
{
    unsigned char tmp;
    CS_L;
    Delay(10000);
    UCB1TXBUF = data;
    while (!(IFG2 & UCB1TXIFG)); 
    Delay(10000);
    tmp = UCB1RXBUF;						// 空读操作
    UC1IFG &=~UCB1RXIFG;
    CS_H;    
    while(Read_Status_Register()&0x01); 
}
/*************************************************************************
** 函数名称：Read_Status_Register
** 工作环境: IAR 4.11B 
** 作者:     谷丰
** 生成日期: 2008.11.10
** 功能:读状态寄存器，判断BUSY位是否为0、1
** 相关文件:
** 修改日志：
*************************************************************************/
unsigned char Read_Status_Register(void)
{
    unsigned char tmp;
    CS_L;
    Send_Data(0x05);	
    Delay(1000);
    Send_Data(0);	
    Delay(1000);
    UCB1TXBUF = 0X00;                     
    while (!(IFG2 & UCB1TXIFG));
    tmp = UCB1RXBUF;				  // 空读操作
    UC1IFG &=~UCB1RXIFG;
    CS_H;
    return tmp;
}
/*************************************************************************
** 函数名称：read_ID
** 工作环境: IAR 4.11B 
** 作者:     谷丰
** 生成日期: 2008.11.10
** 功能:读取器件ID号，用来判断器件是否被接通
** 相关文件:
** 修改日志：
*************************************************************************/
unsigned int read_ID(char dat)
{
    unsigned int tmp;
    DevId = 0;
    ManuId = 0;
    P5OUT &= ~BIT0;
    UCB1TXBUF = dat;                          
    while (!(IFG2 & UCB1TXIFG));
    tmp = UCB1RXBUF;						// 空读操作
    UC1IFG &=~UCB1RXIFG;
    
    UCB1TXBUF = 0x00;
    while(!(IFG2 & UCB1TXIFG)); 
    tmp = UCB1RXBUF;		
    UC1IFG &=~UCB1RXIFG;
    
    UCB1TXBUF = 0x00;
    while(!(IFG2 & UCB1TXIFG));
    tmp = UCB1RXBUF;						// 读操作
    UC1IFG &=~UCB1RXIFG;
    UCB1TXBUF = 0x00;
    while(!(IFG2 & UCB1TXIFG));
    tmp = UCB1RXBUF;						// 读操作
    UC1IFG &=~UCB1RXIFG;

    UCB1TXBUF = 0x00;
    while(!(IFG2 & UCB1TXIFG));
    tmp = UCB1RXBUF;						// 读操作
    ManuId = tmp;
    UC1IFG &=~UCB1RXIFG;
    tmp <<=8;	
    
    UCB1TXBUF = 0x00;
    while(!(IFG2 & UCB1TXIFG));
    tmp |= UCB1RXBUF;						// 读操作
    UC1IFG &=~UCB1RXIFG;
    DevId = tmp;
    Delay(2);
    P5OUT |= BIT0;
    return tmp;
}
/*************************************************************************
** 函数名称：write_status
** 工作环境: IAR 4.11B 
** 作者:     谷丰
** 生成日期: 2008.11.10
** 功能:写入状态寄存器
** 相关文件:
** 修改日志：
*************************************************************************/
void write_status(unsigned char data)
{
    unsigned char tmp;
    CS_L;                                                   // 选择器件
    Send_Data(data);                                        // 发送写状态寄存器命令0x01 
    Send_Data(0);
    CS_H;
    while(Read_Status_Register()&0x01);                     // 判断是否写成功
}
/*************************************************************************
** 函数名称：Delay
** 工作环境: IAR 4.11B 
** 作者:     谷丰
** 生成日期: 2008.11.10
** 功能:延时程序
** 相关文件:
** 修改日志：
*************************************************************************/
void Delay(unsigned int dly)
{
    while(dly --);
}

/*************************************************************************
** 函数名称：read_rom
** 工作环境: IAR 4.11B 
** 作者:     谷丰
** 生成日期: 2008.11.10
** 功能:读出写入的数据，CS_L时开始读，CS_H时读取结束
** 相关文件:
** 修改日志：
*************************************************************************/
void write_rom(unsigned char data)
{
    CS_L;
    Send_Data(data);                                // 发送写命令0x02
    for(unsigned char i=0;i<3;i++)
    {
        Send_Data(0x00);                            // 发送写开始地址
    }
    for(unsigned int i=0;i<256;i++)                 // 发送数据
    {
        Send_Data(send_buff[i]);
    }
    CS_H;                                    
    while(Read_Status_Register()&0x01);              // 读状态寄存器判断写操作是否被完成
}
/*************************************************************************
** 函数名称：read_rom
** 工作环境: IAR 4.11B 
** 作者:     谷丰
** 生成日期: 2008.11.10
** 功能:读出写入的数据，CS_L时开始读，CS_H时读取结束
** 相关文件:
** 修改日志：
*************************************************************************/
void read_rom(unsigned char data)
{
    CS_L;                                          // 选择器件
    Send_Data(data);                               // 发送读命令0x03
    for(unsigned char i=0;i<4;i++)
    {
        Send_Data(0x0);                            // 发送读开始地址
    }
    for(unsigned int i=0;i<256;i++)
    {
        UCB1TXBUF = 0;
        while (!(UC1IFG & UCB1TXIFG));
        data_buff[i] = UCB1RXBUF;                  // 发送写入的数据
        UC1IFG &=~UCB1RXIFG;
    }
    CS_H;
}
/*************************************************************************
** 函数名称：SendData
** 工作环境: IAR 4.11B 
** 作者:     谷丰
** 生成日期: 2008.11.10
** 功能:发送数据子函数，发送数据或者用来产生时钟
** 相关文件:
** 修改日志：
*************************************************************************/
void Send_Data(unsigned char data)
{
    UCB1TXBUF = data;                          // 判断数据是否发送完成
    while (!(UC1IFG & UCB1TXIFG));
    UC1IFG &=~UCB1RXIFG;
}

