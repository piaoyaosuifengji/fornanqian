

#include <stdio.h>
/*交换俩个变量的值
			a=a+b;
			b=a-b;
			a=a-b;
*/
#define swap(a,b)  ((a)=(a) - ( (b) = ( (a) = (a) + (b) ) - (b)))

void qsort(int * data,int left,int right)
{
	//退出条件
	int key;
	int i,j;

	if(left >right || !data)
		return;
	printf("qsort here \n");
	//分块
	i=left;
	j=right;
	key=data[left];
	while(i == right)
	{
		for(;left != right;j--)
		{printf("qsort here \n");
			if(key > data[j])
			{
				swap(data[j],data[i]);
				break;
			}
			
		}
		for(;left != right;i++)
		{printf("qsort here \n");
			if(key < data[i])
			{
				swap(data[j],data[i]);
				break;
			}
			
		}
	}
	//
	qsort(data, left, key-1);
	qsort(data, key+1, right);


}

void main()
{
	const int len=10;
	int data[len] ={1,2,5,6,7,8,9,3,4,0};
	int left=0;
	int right=len-1;
	 qsort(data, left, right);

	int i;
	for(i=0;i<len;i++)
	{

		printf("%d ",data[i]);
	}	
	printf("end here \n");
}