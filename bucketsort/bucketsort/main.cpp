
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

void prinfdata(int *data,int len)
{
		 //打印排序结果
	int i;
	for(i=0;i<len;i++)
	{
		printf("%2d ",data[i]);
	}	
	printf("\n");
}
int getnumDigit(int num,int numDigit)//获取数字num第numDigit位上的数
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
int getMax(int * data,const int  size)//找到最大数
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
	int point=0;//指向数组中即将放入桶中的数据
	int digit;
	int * bucket[10];
	int  bucketPoint[10];//桶中每行空位置指针，指向可以用来存储数据的位置
	int bucketDataNum=0;//记录桶中存储的数据个数
	int i=0,j;
	int max=getMax(data,size);
	int maxNumDigits=1;//最大位数
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
		//清理桶中数据
		for(i=0;i<10;i++)
		{
			for(j=0;j<size;j++)
			{

				bucket[i][j]=0;
			}
		}
		bucketDataNum=0;
		//遍历数据，放入桶中
		for(point=0;point<size;point++)
		{

	
			digit=getnumDigit(data[point],numDigits);

			(bucket[digit])[bucketPoint[digit]]=data[point];//将数据放入桶中
			bucketPoint[digit]++;
			bucketDataNum++;
			//printbucket(bucket,size);
		}
		 //printbucket(bucket,size);
		//遍桶中数据，放回数组
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