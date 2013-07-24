

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
	int data;
	int flag1;
	int flag2;
	int flag3;
	int count=0;
	printf("输入一个数 \n");
	scanf("%d",&data);

	flag1=data%3;
	flag2=data%5;
	flag3=data%7;
	
	if(flag1==0  )
		count++;
	if(flag2==0  )
		count++;
	if(flag3==0  )
		count++;


	if(count==3 )
	{
		printf("能同时被3，5，7整除 \n");

	}
	if(count==0 )
	{
		printf("不能被3，5，7任一个整除 \n");

	}
	if(count==1 )
	{
		if(flag1==0  )
			printf("能被3 整除\n");
		if(flag2==0  )
			printf("能被5 整除\n");
		if(flag3==0  )
			printf("能被7 整除\n");


	}
	if(count== 2)
	{
		if(flag1!=0  )
			printf("能被5,7 整除\n");
		if(flag2!=0  )
			printf("能被3,7 整除\n");
		if(flag3!=0  )
			printf("能被3,5 整除\n");

	}

	printf("end here \n");
}