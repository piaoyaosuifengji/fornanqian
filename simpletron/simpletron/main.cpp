

#include <stdio.h>
#include "file.h"
#include <stdlib.h>





void myfree(void * p)
{
	if(p)
		free(p);
	p=NULL;
}
//输出内存中的数据
void pintMemory(int memory[][5],int codeCount )
{
	int i,j;
	for(i=0;i<codeCount;i++)
	{
		//for(j=0;j<5;j++)
		{
			printf("symbol=%c,code=%d %d %d %d\n",memory[i][0],memory[i][1],memory[i][2],memory[i][3],memory[i][4]);

		}

	}
}
int explanationCode(int memory[][5],int currentCode,int *parameter)//解析指令,返回指令类别，并在parameter中设置相应参数
{
	int operating=0;
	operating=memory[currentCode][2]+memory[currentCode][1] *10;
	*parameter=memory[currentCode][4]+memory[currentCode][3] *10;

	printf("operating %d parameter=%d\n",operating,*parameter);
	return  operating;
}
int executionSML(int memory[][5],int *accumulator)
{
	int res=0;

	int currentCode=0;//当前执行指令指针
	int nextCode=0;//下条执行命令指令指针
	int operating;//命令种类
	int Parameter;//当前指令所需要的参数
	while(1)
	{
		//解析当前指令
		operating= explanationCode( memory, currentCode,&Parameter);//解析指令


	}
}

void main()
{
	//char * lin=readFileLine("D:\\my.txt",1);
	//printf("%s \n",lin);
	//myfree(lin);
	//lin[1]=1;
	char * file="D:\\my.txt";
	int memory[100][5];//保存指令
	int accumulator=0;//累加器
	int res=0;
	//加载
	int codeCount=loadSMLCode(file,memory );
	//输出加载的指令
	pintMemory( memory,codeCount);


	//执行指令
	res=executionSML( memory,&accumulator);


}