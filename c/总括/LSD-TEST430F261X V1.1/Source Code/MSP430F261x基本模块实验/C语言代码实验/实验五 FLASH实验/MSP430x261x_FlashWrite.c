/*******************************************************************************
// 版权:   杭州利尔达科技有限公司
// 文件名:MSP430x261_flshwrite.c
// 版本：V1.0
// 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
// 作者:Huanglongsong
// 描述：MSP430x26x在线编程演示，从段Ｃ复制到段Ｄ：
//     　首先擦除段Ｃ,再将自增量值写入，然后再擦除段Ｄ，将段Ｃ复制到段Ｄ
//       段Ｃ的起始地址：0x1040
//       段Ｄ的起始地址：0x1000
// ACLK = n/a, MCLK = SMCLK = CALxxx_1MHZ = 1MHz
//
// 生成日期:2008年11月
//
//            MSP430F261x/241x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//
******************************************************************************/
#include "msp430x26x.h"


char value;                                 // 字节型变量定义

// 函数原型声明
void write_SegC(char value);
void copy_C2D(void);

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // 停止看门狗

  P1DIR |= 0x10;                            // P1.4 输出模式
  P1SEL |= 0x10;                            // P1.4 SMCLK 输出
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 的时钟作为Flash的内部时序
  value = 0;                                // 初始化变量

  while(1)                                 
  {
    write_SegC(value++);                    // 写值到段Ｃ
    copy_C2D();                             // 复制段C到段D
    __no_operation();                       // 调试时在此处放置断点
  }
}
// 先擦除再写入
void write_SegC(char value)
{
  char *Flash_ptr;                          // Flash指针
  unsigned int i;

  Flash_ptr = (char *)0x1040;               // 初始化指针，指向0x1040
  FCTL3 = FWKEY;                            // 解锁
  FCTL1 = FWKEY + ERASE;                    // 开擦除功能
  *Flash_ptr = 0;                           // 一次空写起动擦除C段

  FCTL1 = FWKEY + WRT;                      // 字节写功能允许

  for (i = 0; i < 64; i++)
  {
    *Flash_ptr++ = value;                   // 写值到Flash
  }

  FCTL1 = FWKEY;                            // 字节写功能禁止
  FCTL3 = FWKEY + LOCK;                     // 上锁保护FLash
}

void copy_C2D(void)
{
  char *Flash_ptrC;                         // C段的指针
  char *Flash_ptrD;                         // D段的指针
  unsigned int i;

  Flash_ptrC = (char *)0x1040;              // 初始化C段的指针
  Flash_ptrD = (char *)0x1000;              // 初始化D段的指针
  FCTL3 = FWKEY;                            // 解锁
  FCTL1 = FWKEY + ERASE;                    // 开擦除功能
  *Flash_ptrD = 0;                          //  一次空写起动擦除D段
  FCTL1 = FWKEY + WRT;                      // 字节写功能允许

  for (i = 0; i < 64; i++)
  {
    *Flash_ptrD++ = *Flash_ptrC++;          // 从Ｃ段把值复制到Ｄ段
  }

  FCTL1 = FWKEY;                            // 字节写功能禁止
  FCTL3 = FWKEY + LOCK;                     // 上锁保护FLash
}

