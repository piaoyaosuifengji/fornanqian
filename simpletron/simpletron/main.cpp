

#include <stdio.h>
#include "file.h"
#include <stdlib.h>





void myfree(void * p)
{
	if(p)
		free(p);
	p=NULL;
}
//����ڴ��е�����
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
int explanationCode(int memory[][5],int currentCode,int *parameter)//����ָ��,����ָ����𣬲���parameter��������Ӧ����
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

	int currentCode=0;//��ǰִ��ָ��ָ��
	int nextCode=0;//����ִ������ָ��ָ��
	int operating;//��������
	int Parameter;//��ǰָ������Ҫ�Ĳ���
	while(1)
	{
		//������ǰָ��
		operating= explanationCode( memory, currentCode,&Parameter);//����ָ��


	}
}

void main()
{
	//char * lin=readFileLine("D:\\my.txt",1);
	//printf("%s \n",lin);
	//myfree(lin);
	//lin[1]=1;
	char * file="D:\\my.txt";
	int memory[100][5];//����ָ��
	int accumulator=0;//�ۼ���
	int res=0;
	//����
	int codeCount=loadSMLCode(file,memory );
	//������ص�ָ��
	pintMemory( memory,codeCount);


	//ִ��ָ��
	res=executionSML( memory,&accumulator);


}