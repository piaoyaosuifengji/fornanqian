

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
	data2[i++]=tmp%10;//��λ
	tmp=tmp/10;
	data2[i++]=tmp%10;//10λ
	tmp=tmp/10;
	data2[i++]=tmp%10;//100λ
	tmp=tmp/10;
	data2[i++]=tmp%10;//1000λ

	int aa;//��λ

	int bb;//10λ

	int cc;//100λ

	int dd;//1000λ
	
	printf("���ֲ��룺����4λ�����ÿո��� \n");
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
		printf("������ȷ��λ���ĸ���%d,����λ����ȷ����=%d \n",rightNum,rightPos);
		if(rightNum==4 && rightPos==4 )
			break;
		//printf("data=%d,rightNum=%d,rightPos=%d \n",data,rightNum,rightPos);
		printf("�������� \n");
	}
	printf("end here \n");
}