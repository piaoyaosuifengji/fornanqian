

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
	printf("����һ���� \n");
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
		printf("��ͬʱ��3��5��7���� \n");

	}
	if(count==0 )
	{
		printf("���ܱ�3��5��7��һ������ \n");

	}
	if(count==1 )
	{
		if(flag1==0  )
			printf("�ܱ�3 ����\n");
		if(flag2==0  )
			printf("�ܱ�5 ����\n");
		if(flag3==0  )
			printf("�ܱ�7 ����\n");


	}
	if(count== 2)
	{
		if(flag1!=0  )
			printf("�ܱ�5,7 ����\n");
		if(flag2!=0  )
			printf("�ܱ�3,7 ����\n");
		if(flag3!=0  )
			printf("�ܱ�3,5 ����\n");

	}

	printf("end here \n");
}