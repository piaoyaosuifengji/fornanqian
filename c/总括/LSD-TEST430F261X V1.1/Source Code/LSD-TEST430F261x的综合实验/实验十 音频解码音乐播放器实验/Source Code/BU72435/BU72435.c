/******************************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���: BU72435.C
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ��������:2008��12��
**  ���������ļ�����BU72435ģ�������������ʵ��BU72435��Ƶ����������ܣ��������š���
**  ͣ��ֹͣ����Чѡ�񡢲����豸ѡ�񡢲����ļ���ѡ���ظ�ģʽѡ�����ģʽѡ��������
**  Сѡ��ȡ� BU9435�����ڴӻ���MODE2ģʽ���������ָ�ʽΪMP3������ο�DEMOԭ��ͼ����
**  ϸ�������á�
*******************************************************************************/
#include "BU72435.h"
/*****************************************************************
*** ��������Write_PKG                                                      
** ���ܣ�дBU27435�Ŀ���ָ�����������ָ��ṹ������
** ����: huanglongsong
** ��������: 2008.12
** ����ļ�:
** �޸���־��
******************************************************************/
void Write_Data_PKG( const Struct_PKG_Type *Ptr_PKG )
{
    uchar PKG_Length = Ptr_PKG -> Data_CNT;
    
    I2C_Start();
    I2C_Senduchar( Write_BU72435 );
    I2C_Write_ACK();
    for( uchar i = 0;i< PKG_Length;i++ )
    {
        I2C_Senduchar( *( Ptr_PKG -> Com_PKG + i ) );
        I2C_Write_ACK();
    }
    I2C_Stop();
}
/*************************************************
** ����(ģ��)����:Key_Process
** ����: ���ֲ���ģ��������̴����ο�BU72435�����ֲᷢ����Ӧָ��ʵ�ָ��ֹ���
** ����: huanglongsong
** ��������: 2008.12
** ����ļ�:
** �޸���־��		
*************************************************/
#pragma vector = PORT1_VECTOR
__interrupt void Key_Process(void)
{
    static uchar Play_Flag = 0;
    static Struct_PKG_Type *Struct_Prt = &SET_EQ_Cmd[1];
    
    switch( KeyScan())
    {
    case K1:
        if( !Play_Flag )            
        {
            Write_Data_PKG( &PlayCmd );         // ����ģʽ
            Play_Flag = 1;
        }
        else
        {
            Write_Data_PKG( &PauseCmd );        // ��ͣģʽ
            Play_Flag = 0;
        }
        break;   
    case K2:
        Write_Data_PKG( &StopCmd );             // ֹͣģʽ
        Play_Flag = 0;
        break;   
    case K3:
        Write_Data_PKG( &ChngDevCmd );          // USB��SD�л�
        Play_Flag = 0;
        break;
    case K4:
        Write_Data_PKG( &VolAddCmd );           // ������
        break;
    case K5:
        Write_Data_PKG( &FB_PlayCmd );          // ��һ��
        while( BU72435_Port & MCHNG );
        Delays(10);
        Write_Data_PKG( &FB_PlayCmd );
        break;
    case K6:
        Write_Data_PKG( &FF_PlayCmd );          // ��һ��
        break;
    case K7:
        Write_Data_PKG( &TenAdd_PlayCmd );      // ������ʮ��
        break;
    case K8:
        Write_Data_PKG( &VolDecCmd );           // ������
        break;    
    case K9:
        Write_Data_PKG( &FOL_AddPlayCmd );      // ������һ���ļ���
        break;   
    case K10:
        Write_Data_PKG( &FOL_DecPlayCmd );      // ������һ���ļ���
        break;   
    case K11:
        Write_Data_PKG( &TenDec_PlayCmd );      // ������ʮ��    
        break;
        
    case K12:
        Write_Data_PKG( Struct_Prt++ );         // ��Ч�л�
        if( Struct_Prt >= SET_EQ_Cmd + EQ_CNT )
        {
            Struct_Prt = SET_EQ_Cmd;
        }
        break;
    default:
        break; 
    }
    P1IFG = 0;
}