

#include <stdio.h>
#include <stdlib.h> 
#include <malloc.h>

void main()
{
	int m;
	int n;
	int *data=NULL;
	int i,j;
	int manNumber=0;
	int manNumberX=0;//横坐标
	int manNumberY=0;//众坐标
	n=m=0;
	printf("输入数组大小，以空格键间隔 \n");
	scanf("%d %d",&n,&m);
	if(m<1 || n <1)
	{
		printf("wrong input \n");
		return;
	}
	else
		printf("动态生成%d* %d数组如下：\n",n,m);
	data=(int *)malloc(sizeof(int)*m*n);
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			data[i*n+j]=rand()%50+50;
			printf("%d  ",data[i*n+j]);


			if(data[i*n+j] >manNumber)
			{
				manNumber=data[i*n+j];
				manNumberX=i+1;
				manNumberY=j+1;

			}
		}
		printf("\n");
	}
	printf("动态数组中其中一个最大值为%d，位置为：（%d，%d）\n",manNumber,manNumberX,manNumberY);
	printf("end here \n");
}