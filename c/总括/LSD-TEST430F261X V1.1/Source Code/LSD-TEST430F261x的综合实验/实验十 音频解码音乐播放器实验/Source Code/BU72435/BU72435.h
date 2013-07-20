/*****************************************************************
** ��Ȩ:   ����������Ƽ����޹�˾
** �ļ���: BU72435.h
** �汾��V1.0
** ��������:IAR Embedded Workbench Evaluation for MSP430 4.10A
** ����:Huanglongsong
** ��������:2008��11��
*****************************************************************/
#include "General_File.h"
#include "Key_Define.h"
/****************************************************************/

#define         Write_BU72435             0x80
#define         Read_BU72435              0x81
#define         BU72435_BUSY              BIT2
#define         MCHNG                     BIT1
#define         BU72435_Port              P6IN

typedef struct
{
    const uchar Com_PKG[5];
    const uchar Data_CNT;
}Struct_PKG_Type;

//BU9435�������������
Struct_PKG_Type PlayCmd = {{0x50, 0X01},0x02 };                 //PLAY
Struct_PKG_Type PauseCmd = {{0x50, 0X02},0x02 };                //PAUSE
Struct_PKG_Type StopCmd = {{0x50, 0X03},0x02 };                 //STOP
Struct_PKG_Type VolAddCmd = {{0x50, 0X04},0x02 };               //VOL+
Struct_PKG_Type VolDecCmd = {{0x50, 0X05},0x02 };               //VOL-
Struct_PKG_Type RepeatCmd = {{0x50, 0X06},0x02 };               //REPEAT
Struct_PKG_Type RandomCmd = {{0x50, 0X07},0x02 };               //RANDOM
Struct_PKG_Type ChngDevCmd = {{0x50, 0X08},0x02 };              //CHHG_DEV
Struct_PKG_Type AbortCmd = {{0x50, 0X0C},0x02 };                //ABORT
Struct_PKG_Type FFCmd = {{0x55, 0X01, 0X00, 0X00},0x04 };       //FF
Struct_PKG_Type FF_PlayCmd = {{0x55, 0X01, 0X01, 0X00},0x04};  //FF & PLAY
Struct_PKG_Type FB_PlayCmd = {{0x55, 0X02, 0X01, 0X00},0x04};  //FB & PLAY
Struct_PKG_Type TenAdd_PlayCmd = {{0x55, 0X05, 0X01, 0X00},0x04};  //10+ & PLAY
Struct_PKG_Type TenDec_PlayCmd = {{0x55, 0x06, 0x01, 0x00},0x04};   // 10- & PLAY
Struct_PKG_Type FOL_AddPlayCmd = {{0x55, 0X03, 0X01, 0X00},0x04}; //FOL+ & PLAY  
Struct_PKG_Type FOL_DecPlayCmd = {{0x55, 0X04, 0X01, 0X00},0x04}; //FOL- & PLAY
Struct_PKG_Type Sel_ID3 = {{0x56, 0X01},0x02};                 //����ID3��Ϣ����   
Struct_PKG_Type Read_ID3_File = {{0x5F, 0X07},0x02};           //��ȡID3�ļ�����Ϣ
Struct_PKG_Type Read_ID3_Title = {{0x5F, 0X04},0x02};          //��ȡID3��Ϣ����
Struct_PKG_Type Read_ID3_Artist = {{0x5F, 0X05},0x02};         //��ȡID3��Ϣ������
Struct_PKG_Type Read_ID3_Album = {{0x5F, 0X06},0x02};          //��ȡID3��Ϣר��
//Struct_PKG_Type Read_Buff = {{0x5E, 0X02},0x02};               //��BUFF��Ϣ
Struct_PKG_Type Playing_Time1 = {{0x5E, 0X0E},0x02};           //��ȡ����Ϣ
Struct_PKG_Type Playing_Time2 = {{0x5E, 0X0F},0x02};           //��ȡ����Ϣ 
//Struct_PKG_Type Read_Status = {{0X5F, 0X00},0x02};             //��STATUS REGISTOR��Ϣ
Struct_PKG_Type Read_UsbSD = {{0X5E, 0X00},0x02};              //����ѡ����豸 
Struct_PKG_Type Read_Status = {{ 0x5F,0x00},0x02};
Struct_PKG_Type Read_Vol = {{0X5F, 0X02},0x02};                //����������
Struct_PKG_Type SET_EQ_Cmd[] = {                               //����Ч������
    {{ 0x52,0x00},0x02 },          //EQ OFF
    {{ 0x52,0x01},0x02 },          //POPS          
    {{ 0x52,0x02},0x02 },          //JAZZ
    {{ 0x52,0x03},0x02 },          //ROCK
    {{ 0x52,0x04},0x02 },          //CLASSIC
    {{ 0x52,0x05},0x02 },          //R&B
    {{ 0x52,0x07},0x02 },          //EQ OFF
    {{ 0x52,0x08},0x02 },          //BASS BOOST1
    {{ 0x52,0x09},0x02 },          //POPS + BASS BOOST1
    {{ 0x52,0x0A},0x02 },          //JAZZ + BASS BOOST1
    {{ 0x52,0x0B},0x02 },          //ROCK + BASS BOOST1
    {{ 0x52,0x0C},0x02 },          //CLASSIC + BASS BOOST1
    {{ 0x52,0x0D},0x02 },          //R&B + BASS BOOST1
    {{ 0x52,0x0F},0x02 }           //BASS BOOST2
};
const uchar EQ_CNT = sizeof( SET_EQ_Cmd ) / sizeof( Struct_PKG_Type );
//*************************�ⲿ��������*************************************
extern void I2C_Start(void);
extern void I2C_Stop(void);
extern void I2C_Write_ACK( void );
extern void I2C_Read_Ack(void);
extern void I2C_Read_NoAck( void );
extern uchar I2C_Receiveuchar(void);
extern void I2C_Senduchar( uchar Wr_Data );
extern uchar KeyScan( void );
extern void Delays(uint m );
