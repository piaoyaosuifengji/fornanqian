
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

void prinfdata(int *data,int len)
{
		 //��ӡ������
	int i;
	for(i=0;i<len;i++)
	{
		printf("%2d ",data[i]);
	}	
	printf("\n");
}
int getnumDigit(int num,int numDigit)//��ȡ����num��numDigitλ�ϵ���
{
	double chushu=pow((double)10,(numDigit-1));
	if(num<0)
		num=num*(-1);
	if(chushu>num)
		return 0;
	num=num/chushu;
	return  num%10;
}
void printbucket(int * bucket[10],int len)
{
	int i,j;
		for(i=0;i<10;i++)
		{		
			int point=0; 
			for(j=0;j<len;j++)
			{
				printf("%3d ",(bucket[i])[j]);

			}
			printf("\n");
		}

		printf("\n");
}
int getMax(int * data,const int  size)//�ҵ������
{

	int max=0;
	int i;
	for(i=0;i<size;i++)
	{	
		if(data[i] >max)
			max=data[i];

	}
	return max;
}
void bucketsort(int * data,const int  size)
{

	int numDigits=0;
	int point=0;//ָ�������м�������Ͱ�е�����
	int digit;
	int * bucket[10];
	int  bucketPoint[10];//Ͱ��ÿ�п�λ��ָ�룬ָ����������洢���ݵ�λ��
	int bucketDataNum=0;//��¼Ͱ�д洢�����ݸ���
	int i=0,j;
	int max=getMax(data,size);
	int maxNumDigits=1;//���λ��
	for(i=0;i<10;i++)
	{
		bucket[i]=(int *)malloc(sizeof(int)*size);
		for(j=0;j<size;j++)
		{

			bucket[i][j]=0;
		}
	}
	

	 while((int)(max=max/10) >0)
	 {
		 maxNumDigits++;
	 }
	while((++numDigits) <=maxNumDigits)
	{
		
		for(i=0;i<10;i++)
		{
			bucketPoint[i]=0;
		}
		//����Ͱ������
		for(i=0;i<10;i++)
		{
			for(j=0;j<size;j++)
			{

				bucket[i][j]=0;
			}
		}
		bucketDataNum=0;
		//�������ݣ�����Ͱ��
		for(point=0;point<size;point++)
		{

	
			digit=getnumDigit(data[point],numDigits);

			(bucket[digit])[bucketPoint[digit]]=data[point];//�����ݷ���Ͱ��
			bucketPoint[digit]++;
			bucketDataNum++;
			//printbucket(bucket,size);
		}
		 //printbucket(bucket,size);
		//��Ͱ�����ݣ��Ż�����
		point=0; 
		for(i=0;i<10;i++)
		{

			int j;
			
			for(j=0;j<bucketPoint[i];j++)
			{
				
				data[point++]=bucket[i][j];

			}
		}
		//prinfdata(data, size);
	}

}
void main()
{
	const int size=40;
	int *data=(int *)malloc(sizeof(int)*size);

	int i;

	for(i=0;i<size;i++)
	{
		data[i]=rand()%150;
	}
	prinfdata(data, size);
	bucketsort(data,size);
	prinfdata(data, size);
	printf("end here \n");

}