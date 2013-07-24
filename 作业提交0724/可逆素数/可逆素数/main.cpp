

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find(int m)//寻找最小质因数
{
	int i=2;

	while(m%i !=0)
		i++;
	return i;
}
int getReversal(int d)//获取d反转数
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
int checkReversal(int d)//判断d的反转数是否是素数
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
	printf("输出1000到9999以内 全部素数:\n");
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