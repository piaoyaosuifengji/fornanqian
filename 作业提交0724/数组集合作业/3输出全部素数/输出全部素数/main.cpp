

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
	int i;
	printf("���1000���� ȫ������:\n");
	int res=0;
	for(i=2;i<=1000;i++)
	{
		res=find(i);
		if(i == res)
			printf("%d ",res);
	}
	printf("end here \n");
}