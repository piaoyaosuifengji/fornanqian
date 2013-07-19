

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
	int i;
	printf("输出1000以内 全部素数:\n");
	int res=0;
	for(i=2;i<=1000;i++)
	{
		res=find(i);
		if(i == res)
			printf("%d ",res);
	}
	printf("end here \n");
}