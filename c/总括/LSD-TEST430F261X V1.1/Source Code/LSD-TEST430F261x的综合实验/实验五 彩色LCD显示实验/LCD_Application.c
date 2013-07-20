/*****************************************************************
** 版权:   杭州利尔达科技有限公司
** 文件名:LCD_Application.c
** 版本：V1.0
** 工作环境:IAR Embedded Workbench Evaluation for MSP430 4.10A
** 作者:Huanglongsong
** 描述：关于LM35T液晶模块的相关函数的定义，应用类型函数，供外部函数调用
** 生成日期:2008年11月
*****************************************************************/
#include "LM35T_Head.h"

extern void SdCmd(uchar Command);
extern void CmdEnd(void);
extern void WritePKG(uchar const *pkg);
extern void SetFgColor(uint color);
extern void SetFontFgColor(uint color);
/*************************************************************************
函数名：ShowBMP160
入口参数：
出口参数：无
功能：图片驱动程序(在LCD上显示一幅图片)
代码编制：黄龙松
日期：2008年12月
*************************************************************************/
void ShowBMP160(ulong X, ulong Y,const uchar *Pic)
{	
    uchar i,j,k,Buffer[5];
    uint  p;
    ulong addr;
    
    addr=Y*5;				        // 列地地址计算
    addr=addr<<7;			        //
    addr=addr+X*2;			        // 行地址计算(addr= X*2 + Y*320*2)
    p=0;			                // Data ROM pointer
    for(i=0;i<154;i++)
    {	
        Buffer[0]=4;
        Buffer[1]=0x81;
        Buffer[2]=addr;
        Buffer[3]=addr>>8;
        Buffer[4]=addr>>16;
        WritePKG(Buffer);
        
        for(j=0;j<8;j++)			
        {
            SdCmd(0x84);			// 数据包长度指令
            SdCmd(40);				// 数据包长度定义
            for(k=0;k<20;k++)
            {
                SdCmd(Pic[p+1]);	        // low byte
                SdCmd(Pic[p]);	                // high byte
                p+=2;				
            }	
            CmdEnd();
        }
        addr+=640;			        // 换行
    }		
}
/*********************************************************************************
函数名：ShowBMP160
入口参数：
出口参数：无
功能：图片驱动程序(显示一个单色的图片)
代码编制：黄龙松
日期：2008年12月
**********************************************************************************/
void ShowSingleBMP(ulong X, ulong Y,const uchar *Pic)
{	
    uchar Buffer[5];
    //uint  Index = 0;
    ulong addr;
    uint Fgcolor = WHITE;                        //前景色变量
    uint Bgcolor = BLUE;                         //背景色变量
    
    addr=Y*5;				        // 列地地址计算
    addr=addr<<7;			        //
    addr=addr+X*2;			        // 行地址计算（ addr= X*2 + Y*320*2）			// Data ROM pointer
    
    
    for(uint i=0;i<3200;i++)
    {	
        Buffer[0]=4;
        Buffer[1]=0x81;
        Buffer[2]=addr;
        Buffer[3]=addr>>8;
        Buffer[4]=addr>>16;	
        WritePKG(Buffer);
        
        SdCmd(0x84);			                // 数据包长度指令
        SdCmd(48);	
        for(uchar j=0;j<3;j++)			        // sprit one line data to 8 packet
        {
            uchar Temp_Data = *Pic++;
            for( uchar l=0;l<8;l++ )
            {
                if( Temp_Data&0x80 )                    // 当前数据位判断，为1为前景色，0为背景色
                {
                    SdCmd((uchar)(Fgcolor&0xff));	// low byte
                    SdCmd((uchar)(Fgcolor>>8));	        // high byte	
                }
                else
                {   
                    SdCmd((uchar)(Bgcolor&0xff));	// low byte
                    SdCmd((uchar)(Bgcolor>>8));	        // high byte	
                }
                Temp_Data <<= 1;
            }
        }
        CmdEnd();
        addr+=48;					// 下一行
    }		
}