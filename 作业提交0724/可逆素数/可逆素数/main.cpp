

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find(int m)//Ѱ����С������
{
	int i=2;

	while(m%i !=0)
		i++;
	return i;
}
int getReversal(int d)//��ȡd��ת��
{
	int reversal=0;
	int i=0;

	while(d/10 != 0 || d>0)
	{
		reversal=reversal*10+d%10;
		d=d/10;
	}
	return reversal;
}
int checkReversal(int d)//�ж�d�ķ�ת���Ƿ�������
{

	int res=0;
	if(d<2)
		return res;

	int reversal=1;
	int tmp;
	reversal=getReversal( d);
	if(reversal<1000)
		return 0;
	tmp=find(reversal);
	if(reversal == tmp && reversal>1000)
		return 1;	
	else 
		return 0;
}
void main()
{
	int i;
	printf("���1000��9999���� ȫ������:\n");
	int res=0;
	int count=0;
	for(i=1000;i<=9999;i++)
	{

		res=find(i);
		if(i != res)
			continue;	

		res=checkReversal(i);
		if(1 == res)
		{
			printf("%d ",i);count++;
		}
	}
	printf("\nend here \n");

}