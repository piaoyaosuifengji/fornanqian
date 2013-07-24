
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

/*交换俩个变量的值
			a=a+b;
			b=a-b;
			a=a-b;
*/
#define swap(a,b)  ((a)=(a) - ( (b) = ( (a) = (a) + (b) ) - (b)))

void qsort(int * data,int left,int right)
{

	//printf("left=%d, right=%d \n",left,right);
	//退出条件
	static int count=0;
	int key;
	int i,j;
	int index;
	count++;
	if(left >=right || !data || left<0)
	{	
		//printf("qsort quit count=%d\n",count);
		return;
	}
	//分块
	i=left;
	j=right;
	key=data[left];
	while(i < j)
	{
		for(;i < j && key <= data[j];j--)
		{
			//printf("1 \n");
		}
		if(i == j)
		{
			index=i;
			break;
		}
		else
			swap(data[j],data[i]);
		for(;i < j && key >= data[i];i++)
		{
			//printf("2 \n");
		}
		if(i == j)
		{
				index=i;
				break;
		}
		else
			swap(data[j],data[i]);
		
		//printf("3 i,j  %d,%d,%d,%d  key=%d\n",i,j,data[i],data[j],key);
	}
	//
	
	qsort(data, left, index-1);
	qsort(data, index+1, right);


}

void main()
{
	//32位int的取值范围是 min:-2147483648  max:2147483647
	const int len=1678;///////////////////123456789
	int *data=(int *)malloc(sizeof(int)*len);
	int left=0;
	int right=len-1;

	int i;
	for(i=0;i<len;i++)
	{
		data[i]=rand()%1500;
	}	
	//排序
	 qsort(data, left, right);
	 //打印排序结果
	for(i=0;i<len;i++)
	{
		printf("%d ",data[i]);
	}	
	printf("end here \n");
}