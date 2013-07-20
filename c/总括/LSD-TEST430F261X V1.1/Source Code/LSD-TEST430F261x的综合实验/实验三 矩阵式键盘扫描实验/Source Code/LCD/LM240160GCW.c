/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���:LM240160GCW.c
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ����������LM240160GCWҺ��ģ�����غ����Ķ��壬���ײ�Һ�����������ⲿ��������
** ��������:2008��12��
**************************************************************************/
#include "General_File.h"
//*************************************************************************
#define     Fgcolor     0xF8 
#define     Bgcolor     0x00
#define     EndAddr_x   82
#define     EndAddr_y   160

void Delays(uint m );

/*****************************************************************************
��������SdCmd
��ڲ�����uchar Command
���ڲ�������
����:дָ�LCD������
������ƣ�������
���ڣ�2008��12��
*****************************************************************************/
void SdCmd(uchar Command)   //send command
{
    LCDCOM &= ~RS;
    LCDBUS = Command;
    LCDCOM &= ~CS;
    LCDCOM &= ~WR;
    LCDCOM |= WR;
    LCDCOM |= CS;
}
/*****************************************************************************
��������void SdData(uchar DData)
��ڲ�����uchar DData
���ڲ�������
����:д���ݵ�LCD��RAM
������ƣ�������
���ڣ�2008��12��
*****************************************************************************/
void SdData(uchar DData)    //send display data
{
    LCDCOM |= RS;
    LCDBUS = DData;
    LCDCOM &= ~CS;
    LCDCOM &= ~WR;
    LCDCOM |= WR;
    LCDCOM |= CS;
}
/*****************************************************************************
��������void SdData(uchar DData)
��ڲ�����uchar DData
���ڲ�������
����:д���ݵ�LCD��RAM
������ƣ�������
���ڣ�2008��12��
*****************************************************************************/
void Reset_LCD( void )
{
    LCDCOM |= RST;   
    LCDCOM |= CS;
    LCDBUS = 0xff;  
    LCDCOM |= WR;
    LCDCOM |= RD;
    LCDCOM &= ~RST; 
    Delays(2);                                 //Ӳ����λ
    LCDCOM |= RST;  
    Delays(800);                               //��λ���ӳ�һ��ʱ��
}
    
/*****************************************************************************
��������InitLCDM
��ڲ�������
���ڲ�������
����:��ʼ��MSP430F449ϵͳ
������ƣ�������
���ڣ�2008��12��
*****************************************************************************/
void InitLCD(void )
{  
    Reset_LCD();
    SdCmd(0x30);                                // ѡ��ext0ָ�
    SdCmd(0x94);                                // �˳�����ģʽ
    SdCmd(0xd1);                                // �ڲ�OSC����
    SdCmd(0x20); SdData(0x08);                  // �����ڲ���ѹ��·��VB=1
    Delays(1);
    SdCmd(0x20); SdData(0x0b);                  // �����ڲ�LCD������ѹ��·VB=VF=VR=1                                          //��ģʽ��V0=15.2V �������ã�25,2V.  ��ģʽ��V0=15V  �������ã�1D 4
    SdCmd(0x81); SdData(0x1d); SdData(0x04);    // ���öԱȶ�VOֵ
    SdCmd(0xca); SdData(0x00); SdData(0x27); SdData(0x00);// ������ʾɨ��ģʽ��CLD=0; DT=39=160/4-1;FI=0;LF=0;
    SdCmd(0xa7);                                // ������ʾ
    SdCmd(0xbb); SdData(0x01);                  // ������ɨ�跽��== 0-79,159-80
    SdCmd(0xbc); SdData(0x02); SdData(0x01); SdData(0x02);//�������ݴ�����ʽLI=1;CI=0 ;C/L=0 ;CLR=0 ;GS=2 3b3p
    SdCmd(0x31);                               // ѡ��ext1ָ� 
    SdCmd(0x32); SdData(0x00); SdData(0x03); SdData(0x01);//����������·���� Fosc=12.7khz;Fbooster=6hz;bias=1/13 
    SdCmd(0x30);                               // ѡ��ext0ָ�
    SdCmd(0xaf);                               // ����ʾ 
}
/*****************************************************************************
��������ClearRAM
��ڲ�����
���ڲ�������
����:��������
������ƣ�������
���ڣ�2008��12��
*****************************************************************************/
void ClearRAM( void )
{ 
    // �Ա���ɫΪ������ɫ
    SdCmd(0x30);                                  // ѡ��ext0ָ�
    SdCmd(0x15); SdData(0x05); SdData(0x54);      // �е�ַ��������
    SdCmd(0x75); SdData(0x00); SdData(0x9f);      // �е�ַ��������
    SdCmd(0x5c);                                  // ����д��ģʽ
    for (uint i=0;i<160*80;i++)                   // ѭ���ܵ����������п�*������
    {
        SdData(Bgcolor);                          // д������
        SdData(Bgcolor);                          // д������
        SdData(Bgcolor);                          // д������
    }
}
/*****************************************************************************
��������ShowBMP
��ڲ�����uchar x,uchar y,uchar width,uchar high,const uchar bmp[]
���ڲ�������
����:ˢ��һ��������ͼƬ��LCD
������ƣ�������
���ڣ�2008��12��
*****************************************************************************/
void ShowBMP( uchar x,uchar y,uchar width,uchar high,const uchar *bmp )
{  
    uint i,p; 
    uchar j,k,Ddata;
    uchar Addr_x = x + 5;
    
    SdCmd(0x30);                                             // ѡ��ext0ָ�
    SdCmd(0x15); 
    SdData(Addr_x); 
    SdData(Addr_x+width/3-1);                                // �е�ַ��������
    SdCmd(0x75); 
    SdData(y); 
    SdData(y+high-1);                                        // �е�ַ��������
    SdCmd(0x5c);                                             // ����д��ģʽ
    p=0;                                                     // ����ָ���ʼ��
    width=width/8;                                           // ����ˮƽ�ֽ���==ͼ�ο��/8
    for(i=0;i<high*width;i++)                                // ѭ���ܵ����������ֽ�����
    {
        k=bmp[p];                                            // ȡͼ������
        for (j=0;j<8;j++)                                    // ѭ��8��������д�루���ֽڷ�ʽ��
        {
            if( k & 0x80 )                                   // ��������λֵ������ʾɫ����
                Ddata = Fgcolor;                             // ����Ϊ1��ѡ��ǰ��ɫ����
	    else
                Ddata = Bgcolor;                             // ����Ϊ0��ѡ�񱳾�ɫ����
            SdData( Ddata );                                 // ����д��
  	    k=k<<1;                                          // ��ģ������λ
        }
        p++;                                                 // ����ָ���1
    }
}
/*************************************************************************
��������PrintGB
��ڲ�����const uchar x, const uchar y,const uchar *chinesecode
���ڲ�������
���ܣ���LCD�ϴ�ӡһ�������ĺ���
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
void PrintGB(const uchar x, const uchar y,const uchar *chinesecode) 
{  
    uchar i,j,k,Ddata;
    uchar Addr_x = x + 5;
    uchar Addr_y = y;
    
    SdCmd(0x30);                                    // ѡ��ext0ָ�
    SdCmd(0x15); SdData(Addr_x); SdData(Addr_x+5);  // �е�ַ�������ã�18���У�
    SdCmd(0x75); SdData(Addr_y); SdData(Addr_y+15); // �е�ַ�������ã�16���У�
    SdCmd(0x5c);                                    // ����д��ģʽ

    for(i=0;i<32;i++)                               // ѭ��16������д��
    {
        k = *(chinesecode+i);                       // ȡ������벿����ģ����
        for (j=0;j<8;j++)                           // ѭ������8������д��
        {
            if( k & 0x80 )                          // ��������λֵ������ʾɫ����
                Ddata=Fgcolor;                      // ����Ϊ1��ѡ��ǰ��ɫ������
            else
                Ddata=Bgcolor;                      // ����Ϊ0��ѡ�񱳾�ɫ����
            SdData(Ddata);                          // ����д��
            k <<= 1;                                // ��ģ������λ
        }
        if( i % 2 )
        {
            SdData( Bgcolor );                       // �յ�д��
            SdData( Bgcolor );                       // �յ�д��
        }
    }	
}
/*************************************************************************
��������Print_ASCII
��ڲ�����const uchar x, const uchar y,const uchar *ASCII_Code
���ڲ�������
���ܣ�18X16ASCII�����������ܹ���LCD�ϴ�ӡһ���������ַ���
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
void Print_ASCII( const uchar x, const uchar y,const uchar *ASCII_Code )
{
   uchar i,j,k,Ddata;
   uchar Addr_x = x + 5;
   uchar Addr_y = y;
   
   SdCmd(0x30);                                         // ѡ��ext0ָ�
   SdCmd(0x15); SdData( Addr_x ); SdData( Addr_x +2 );  // �е�ַ�������ã�12���У�
   SdCmd(0x75); SdData(Addr_y); SdData(Addr_y +15);     // �е�ַ�������ã�16���У�
   SdCmd(0x5c); 
                                                        // ����д��ģʽ
   for(i=0;i<16;i++)                                    // ѭ��24������д��
   {
       k = *(ASCII_Code + i);                           // ȡ������벿����ģ����
       for (j=0;j<8;j++)                                // ѭ������8������д��
       {
           if( k & 0x80 )                               // ��������λֵ������ʾɫ����
               Ddata = Fgcolor;                         // ����Ϊ1��ѡ��ǰ��ɫ����
           else
               Ddata = Bgcolor;                         // ��Ϊ0��ѡ�񱳾�ɫ����
           SdData( Ddata );                             // ����д��
           k <<= 1;                                     // ��ģ������λ
       }
       SdData( Bgcolor ); 
   }
}
/*************************************************************************
��������Draw_Line
��ڲ�����const uchar x, const uchar y
���ڲ�������
���ܣ���LCD�ϻ�һ���ߣ������(x)���յ�(y)
������ƣ�������
���ڣ�2008��12��
*************************************************************************/
void Draw_Line( const uchar start_x,const uchar start_y,const uchar End_x )
{
    uchar Addr_x = start_x + 5;
    uchar Addr_y = start_y;
    uchar Line_Length = End_x - start_x ;
    
    SdCmd(0x30);                                    // ѡ��ext0ָ�
    SdCmd(0x15); SdData( Addr_x ); SdData( Addr_x+ Line_Length/3-1 );  // �е�ַ�������ã�18���У�
    SdCmd(0x75); SdData( Addr_y ); SdData( Addr_y ); // �е�ַ�������ã�16���У�
    SdCmd(0x5c);  
    
    for( uchar i = 0;i < Line_Length;i++ )
    {
        SdData( Fgcolor );
    }
}
//********************************************************************
//---------------------��ʱ����----------------------------------------
//*********************************************************************
void Delays(uint m )  
{ 
  uint j; uint i;
  for(i=0; i<m; i++)
    for(j=0;j<2000; j++) 
      _NOP();
}