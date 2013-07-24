

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
	int data=rand()%10000;
	int tmp;
	while(data<1000)
		data=rand()%10000;
	tmp=data;
	int data2[4];
	int i=0;
	int rightPos;
	int rightNum;
	data2[i++]=tmp%10;//个位
	tmp=tmp/10;
	data2[i++]=tmp%10;//10位
	tmp=tmp/10;
	data2[i++]=tmp%10;//100位
	tmp=tmp/10;
	data2[i++]=tmp%10;//1000位

	int aa;//个位

	int bb;//10位

	int cc;//100位

	int dd;//1000位
	
	printf("数字猜想：输入4位数，用空格间隔 \n");
	while(1)
	{
		rightPos=0;
		rightNum=0;
		scanf("%d %d %d %d",&dd,&cc,&bb,&aa);
		printf("\n");
		i=0;
		if(aa==data2[i++])
			rightPos++;
		if(bb==data2[i++])
			rightPos++;
		if(cc==data2[i++])
			rightPos++;
		if(dd==data2[i++])
			rightPos++;

		for(i=0;i<4;i++)
		{
		if(aa==data2[i])
		{
			rightNum++;continue;
		}
		if(bb==data2[i])
		{
			rightNum++;continue;
		}
		if(cc==data2[i])
		{
			rightNum++;continue;
		}
		if(dd==data2[i])
		{
			rightNum++;continue;
		}

		}
		printf("输入正确‘位’的个数%d,其中位置正确个数=%d \n",rightNum,rightPos);
		if(rightNum==4 && rightPos==4 )
			break;
		//printf("data=%d,rightNum=%d,rightPos=%d \n",data,rightNum,rightPos);
		printf("重新输入 \n");
	}
	printf("end here \n");
}