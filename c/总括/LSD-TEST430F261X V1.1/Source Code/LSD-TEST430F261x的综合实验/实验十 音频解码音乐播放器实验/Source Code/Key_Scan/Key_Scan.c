//*************************************************
//** 版权:    杭州利尔达科技有限公司
//** 文件名:  Key_Scan.C
//** 版本：   V1.0
//** 工作环境: IAR EmbeddedWorkbench 4.11B
//** 作者: huanglongsong
//** 生成日期: 2008.12
//** 功能:    
//*****************************************************
#include "General_File.h"
#include "Key_Define.h"
/*****************************************************************
函数名：void Delays(uint m)
入口参数：无
出口参数：无
功能:延时子程序
代码编制：黄龙松
日期：2008年12月
******************************************************************/
void Delays(uint m )  
{ 
    uint j; uint i;
    
    for(i=0; i<m; i++)
        for(j=0;j<2000; j++) 
            _NOP();
}
/*************************************************
函数(模块)名称:KeyScan
功能:     扫描得到按键的键码值
输入参数: 无     	          		
输出参数: 无  	
函数返回值说明:无 		
使用的资源：   		
它说明: 无
*************************************************/
uchar KeyScan(void)
{
  uchar scancode,tempcode;    
  
  P1IFG = 0x00;
  
  if( (P1IN&0xF0)!=0xF0 )              //P1.4--P1.7口不全为“1”则与键按下
  {
    Delays( 50 );    
    if((P1IN&0xF0)!=0xF0)              //再判断一次
    {
      scancode=0xFE;             
      while( (scancode&0x10)!=0 )      //判断4次循环扫描结束没？
      { 
        P1OUT = scancode;
        if  ((P1IN&0xF0)!=0xF0)        //是该行有键按下
        {
          tempcode = (P1IN&0xF0)|0x0F;
          P1OUT = 0;
          return((~scancode)|(~tempcode));
        }
        else
        {
          scancode=(scancode<<1)|0x01;   
        }
      }
    }                  
  }
  P1OUT = 0;
  return 0;
}

