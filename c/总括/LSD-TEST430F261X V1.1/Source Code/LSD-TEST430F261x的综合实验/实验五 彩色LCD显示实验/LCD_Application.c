/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���:LCD_Application.c
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ����������LM35TҺ��ģ�����غ����Ķ��壬Ӧ�����ͺ��������ⲿ��������
** ��������:2008��11��
*****************************************************************/
#include "LM35T_Head.h"

extern void SdCmd(uchar Command);
extern void CmdEnd(void);
extern void WritePKG(uchar const *pkg);
extern void SetFgColor(uint color);
extern void SetFontFgColor(uint color);
/*************************************************************************
��������ShowBMP160
��ڲ�����
���ڲ�������
���ܣ�ͼƬ��������(��LCD����ʾһ��ͼƬ)
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
void ShowBMP160(ulong X, ulong Y,const uchar *Pic)
{	
    uchar i,j,k,Buffer[5];
    uint  p;
    ulong addr;
    
    addr=Y*5;				        // �еص�ַ����
    addr=addr<<7;			        //
    addr=addr+X*2;			        // �е�ַ����(addr= X*2 + Y*320*2)
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
            SdCmd(0x84);			// ���ݰ�����ָ��
            SdCmd(40);				// ���ݰ����ȶ���
            for(k=0;k<20;k++)
            {
                SdCmd(Pic[p+1]);	        // low byte
                SdCmd(Pic[p]);	                // high byte
                p+=2;				
            }	
            CmdEnd();
        }
        addr+=640;			        // ����
    }		
}
/*********************************************************************************
��������ShowBMP160
��ڲ�����
���ڲ�������
���ܣ�ͼƬ��������(��ʾһ����ɫ��ͼƬ)
������ƣ�������
���ڣ�2008��12��
**********************************************************************************/
void ShowSingleBMP(ulong X, ulong Y,const uchar *Pic)
{	
    uchar Buffer[5];
    //uint  Index = 0;
    ulong addr;
    uint Fgcolor = WHITE;                        //ǰ��ɫ����
    uint Bgcolor = BLUE;                         //����ɫ����
    
    addr=Y*5;				        // �еص�ַ����
    addr=addr<<7;			        //
    addr=addr+X*2;			        // �е�ַ���㣨 addr= X*2 + Y*320*2��			// Data ROM pointer
    
    
    for(uint i=0;i<3200;i++)
    {	
        Buffer[0]=4;
        Buffer[1]=0x81;
        Buffer[2]=addr;
        Buffer[3]=addr>>8;
        Buffer[4]=addr>>16;	
        WritePKG(Buffer);
        
        SdCmd(0x84);			                // ���ݰ�����ָ��
        SdCmd(48);	
        for(uchar j=0;j<3;j++)			        // sprit one line data to 8 packet
        {
            uchar Temp_Data = *Pic++;
            for( uchar l=0;l<8;l++ )
            {
                if( Temp_Data&0x80 )                    // ��ǰ����λ�жϣ�Ϊ1Ϊǰ��ɫ��0Ϊ����ɫ
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
        addr+=48;					// ��һ��
    }		
}