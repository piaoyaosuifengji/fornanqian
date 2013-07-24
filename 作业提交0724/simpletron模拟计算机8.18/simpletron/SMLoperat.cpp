
#include <stdio.h>
#include "SMLoperat.h"


//从键盘接受一个字
void SMLread(int storeAddr,int memory[][5])
{
	int i=0;
	printf("输入五个字符（空格或回车间隔）：\n");
	char s0;
	char tmp;
	int s1;
	int s2;
	int s3;
	int s4;
	scanf("%c",&s0);
	scanf("%d",&s1);
	scanf("%d",&s2);
	scanf("%d",&s3);
	scanf("%d",&s4);
	scanf("%c",&tmp);
	memory[storeAddr][i++]=s0;
	memory[storeAddr][i++]=s1;
	memory[storeAddr][i++]=s2;
	memory[storeAddr][i++]=s3;
	memory[storeAddr][i++]=s4;
	printf("\n");
}
//打印一个字向屏幕
void SMLWrite(int writeAddr,int memory[][5])
{

	int i=writeAddr;
	printf(" SML:%c %d %d %d %d\n",memory[i][0],memory[i][1],memory[i][2],memory[i][3],memory[i][4]);

	 
}

//加载一个字进累加器
void SMLLoad(int addr,int memory[][5],int *accumulator)
{

	*accumulator=memory[addr][1]*1000+memory[addr][2]*100+memory[addr][3]*10+memory[addr][4];
	if(memory[addr][0] != '+')
		*accumulator=(*accumulator)+(-1);

	printf("accumulator=%d \n",(*accumulator));
}
//将累加器中数据存入内存
void SMLStore(int addr,int memory[][5],int *accumulator)
{
	int data= *accumulator;
	int p;
	//*accumulator=memory[addr][1]*1000+memory[addr][2]*100+memory[addr][3]*10+memory[addr][4];
	//if(memory[addr][0] != '+')
	//	*accumulator=(*accumulator)+(-1);
	//printf("accumulator=%d \n",(*accumulator));

	if(data>0)
		memory[addr][0] = '+';
	else
	{
		memory[addr][0] = '-';
		data=data*(-1);
	}
	p=4;
	while(p>0)
	{
		int tmp=data%10;
		memory[addr][p]=tmp;
		data=data/10;
		p--;
	}



}

//将addr处值累加到累加器中
void SMLADD(int addr,int memory[][5],int *accumulator)
{
	int data=memory[addr][1]*1000+memory[addr][2]*100+memory[addr][3]*10+memory[addr][4];
	if(memory[addr][0] != '+')
		data=(data)+(-1);
	*accumulator=(*accumulator)+data;
}
//从累加器中减去addr处的值
void SMLSUB(int addr,int memory[][5],int *accumulator)
{
	int data=memory[addr][1]*1000+memory[addr][2]*100+memory[addr][3]*10+memory[addr][4];
	if(memory[addr][0] != '+')
		data=(data)+(-1);
	*accumulator=(*accumulator)-data;
}
//从累加器中除去addr处的值
void SMLDIVIDE(int addr,int memory[][5],int *accumulator)
{
	int data=memory[addr][1]*1000+memory[addr][2]*100+memory[addr][3]*10+memory[addr][4];
	if(memory[addr][0] != '+')
		data=(data)+(-1);
	if(data)
	*accumulator=(*accumulator)/data;
	else
		*accumulator=0;
}
//从累加器中乘以addr处的值
void SMLMUL(int addr,int memory[][5],int *accumulator)
{
	int data=memory[addr][1]*1000+memory[addr][2]*100+memory[addr][3]*10+memory[addr][4];
	if(memory[addr][0] != '+')
		data=(data)+(-1);
	*accumulator=(*accumulator)*data;
}
////如果累加器为负值，则返回1
//void branchneg(int memory[][5],int *accumulator)
//{
//
//	if
//}







