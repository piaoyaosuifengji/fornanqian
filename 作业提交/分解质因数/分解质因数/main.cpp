

#include <stdio.h>
int find(int m)//寻找最小质因数
{
	int i=2;
	while(m%i !=0)
		i++;
	return i;
}

void main()
{
	int m;
	int rightNumber=1;
	int res[15];//保存质数数组
	int resPoint=0;//res[15]数组保存数据个数
	printf("输入一个数 \n");
	scanf("%d",&m);
	if(m<2 )
	{
		printf("wrong input \n");
		return;
	}
	while(m>1)
	{	

		rightNumber=find(m);
		res[resPoint++]=rightNumber;
		m=m/rightNumber;
		//printf("m=%d,rightNumber=%d \n",m,rightNumber);
	}
	printf("output:");
	int i=0;
	for(;i<resPoint;i++)
	{
		if(i<(resPoint-1))
			printf("%d*",res[i]);
		else
			printf("%d\n ",res[i]);
	}
	printf("end here \n");
}