

#include <stdio.h>
int find(int m)//Ѱ����С������
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
	int res[15];//������������
	int resPoint=0;//res[15]���鱣�����ݸ���
	printf("����һ���� \n");
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