/*************************************************************************
** ��Ȩ:     ����������Ƽ����޹�˾
** �ļ���:   main
** ��������: IAR 4.11B
** ����:     �ȷ�
** ��������: 2008.11.10
** ����:     ʹ��SPI�ӿڶ�дEEPROM����W25X16
             ϵͳ��ʱ��Ϊ12M��������Ϊ4M
** ����ļ�: 
** �޸���־��
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
** ��Ȩ:     ����������Ƽ����޹�˾
** �ļ���:   main.c
** ��������: IAR 4.11B 
** ����:     �ȷ�
** ��������: 2008.11.10
** ����:     ��дSPI�ӿڵ�FLASH����W25X16
** ����ļ�: 
**************************************************************************/
void main(void)
{
  volatile unsigned int i;
  WDTCTL = WDTPW+WDTHOLD;                      // �رտ��Ź�
  BCSCTL1 &= ~XT2OFF;                          // ����XT2
  BCSCTL3 |= XT2S_2;                           // ѡ������ģʽ
  do
  {
    IFG1 &=~OFIFG; 			                  // �������ʧЧ��־
    for(i=0xff; i>0; i--); 	                  // �ȴ�XT2����
  }
  while((IFG1&OFIFG)!=0); 		              // �ж��Ƿ�����
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
  WREN(0X06);                                  // дʹ�� 
  Chip_Erase(0XC7);                            // ��������
  while(1)
  {
    WREN(0X06);                                // дʹ��
    write_rom(0x02);                           // дROM�Ĵ���
    read_rom(0x03);                            // ��״̬�Ĵ���
    Delay(5000);
  }
}
/*************************************************************************
** �������ƣ�WREN
** ��������: IAR 4.11B 
** ����:     �ȷ�
** ��������: 2008.11.10
** ����:дʹ�� 
** ����ļ�:
** �޸���־��
*************************************************************************/
void WREN(unsigned char data)
{
    CS_L;
    Send_Data(data);                         
    Delay(100);
    CS_H;  
}
/*************************************************************************
** �������ƣ�Chip_Erase
** ��������: IAR 4.11B 
** ����:     �ȷ�
** ��������: 2008.11.10
** ����:ȫ���Ĵ�������
** ����ļ�:
** �޸���־��
*************************************************************************/
void  Chip_Erase(unsigned char data)
{
    unsigned char tmp;
    CS_L;
    Delay(10000);
    UCB1TXBUF = data;
    while (!(IFG2 & UCB1TXIFG)); 
    Delay(10000);
    tmp = UCB1RXBUF;						// �ն�����
    UC1IFG &=~UCB1RXIFG;
    CS_H;    
    while(Read_Status_Register()&0x01); 
}
/*************************************************************************
** �������ƣ�Read_Status_Register
** ��������: IAR 4.11B 
** ����:     �ȷ�
** ��������: 2008.11.10
** ����:��״̬�Ĵ������ж�BUSYλ�Ƿ�Ϊ0��1
** ����ļ�:
** �޸���־��
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
    tmp = UCB1RXBUF;				  // �ն�����
    UC1IFG &=~UCB1RXIFG;
    CS_H;
    return tmp;
}
/*************************************************************************
** �������ƣ�read_ID
** ��������: IAR 4.11B 
** ����:     �ȷ�
** ��������: 2008.11.10
** ����:��ȡ����ID�ţ������ж������Ƿ񱻽�ͨ
** ����ļ�:
** �޸���־��
*************************************************************************/
unsigned int read_ID(char dat)
{
    unsigned int tmp;
    DevId = 0;
    ManuId = 0;
    P5OUT &= ~BIT0;
    UCB1TXBUF = dat;                          
    while (!(IFG2 & UCB1TXIFG));
    tmp = UCB1RXBUF;						// �ն�����
    UC1IFG &=~UCB1RXIFG;
    
    UCB1TXBUF = 0x00;
    while(!(IFG2 & UCB1TXIFG)); 
    tmp = UCB1RXBUF;		
    UC1IFG &=~UCB1RXIFG;
    
    UCB1TXBUF = 0x00;
    while(!(IFG2 & UCB1TXIFG));
    tmp = UCB1RXBUF;						// ������
    UC1IFG &=~UCB1RXIFG;
    UCB1TXBUF = 0x00;
    while(!(IFG2 & UCB1TXIFG));
    tmp = UCB1RXBUF;						// ������
    UC1IFG &=~UCB1RXIFG;

    UCB1TXBUF = 0x00;
    while(!(IFG2 & UCB1TXIFG));
    tmp = UCB1RXBUF;						// ������
    ManuId = tmp;
    UC1IFG &=~UCB1RXIFG;
    tmp <<=8;	
    
    UCB1TXBUF = 0x00;
    while(!(IFG2 & UCB1TXIFG));
    tmp |= UCB1RXBUF;						// ������
    UC1IFG &=~UCB1RXIFG;
    DevId = tmp;
    Delay(2);
    P5OUT |= BIT0;
    return tmp;
}
/*************************************************************************
** �������ƣ�write_status
** ��������: IAR 4.11B 
** ����:     �ȷ�
** ��������: 2008.11.10
** ����:д��״̬�Ĵ���
** ����ļ�:
** �޸���־��
*************************************************************************/
void write_status(unsigned char data)
{
    unsigned char tmp;
    CS_L;                                                   // ѡ������
    Send_Data(data);                                        // ����д״̬�Ĵ�������0x01 
    Send_Data(0);
    CS_H;
    while(Read_Status_Register()&0x01);                     // �ж��Ƿ�д�ɹ�
}
/*************************************************************************
** �������ƣ�Delay
** ��������: IAR 4.11B 
** ����:     �ȷ�
** ��������: 2008.11.10
** ����:��ʱ����
** ����ļ�:
** �޸���־��
*************************************************************************/
void Delay(unsigned int dly)
{
    while(dly --);
}

/*************************************************************************
** �������ƣ�read_rom
** ��������: IAR 4.11B 
** ����:     �ȷ�
** ��������: 2008.11.10
** ����:����д������ݣ�CS_Lʱ��ʼ����CS_Hʱ��ȡ����
** ����ļ�:
** �޸���־��
*************************************************************************/
void write_rom(unsigned char data)
{
    CS_L;
    Send_Data(data);                                // ����д����0x02
    for(unsigned char i=0;i<3;i++)
    {
        Send_Data(0x00);                            // ����д��ʼ��ַ
    }
    for(unsigned int i=0;i<256;i++)                 // ��������
    {
        Send_Data(send_buff[i]);
    }
    CS_H;                                    
    while(Read_Status_Register()&0x01);              // ��״̬�Ĵ����ж�д�����Ƿ����
}
/*************************************************************************
** �������ƣ�read_rom
** ��������: IAR 4.11B 
** ����:     �ȷ�
** ��������: 2008.11.10
** ����:����д������ݣ�CS_Lʱ��ʼ����CS_Hʱ��ȡ����
** ����ļ�:
** �޸���־��
*************************************************************************/
void read_rom(unsigned char data)
{
    CS_L;                                          // ѡ������
    Send_Data(data);                               // ���Ͷ�����0x03
    for(unsigned char i=0;i<4;i++)
    {
        Send_Data(0x0);                            // ���Ͷ���ʼ��ַ
    }
    for(unsigned int i=0;i<256;i++)
    {
        UCB1TXBUF = 0;
        while (!(UC1IFG & UCB1TXIFG));
        data_buff[i] = UCB1RXBUF;                  // ����д�������
        UC1IFG &=~UCB1RXIFG;
    }
    CS_H;
}
/*************************************************************************
** �������ƣ�SendData
** ��������: IAR 4.11B 
** ����:     �ȷ�
** ��������: 2008.11.10
** ����:���������Ӻ������������ݻ�����������ʱ��
** ����ļ�:
** �޸���־��
*************************************************************************/
void Send_Data(unsigned char data)
{
    UCB1TXBUF = data;                          // �ж������Ƿ������
    while (!(UC1IFG & UCB1TXIFG));
    UC1IFG &=~UCB1RXIFG;
}

