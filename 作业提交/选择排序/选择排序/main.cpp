

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>
void prinfdata(int *data,int len)
{
		 //´òÓ¡ÅÅÐò½á¹û
	int i;
	for(i=0;i<len;i++)
	{
		printf("%2d ",data[i]);
	}	
	printf("\n");
}
void selectionSort(int *data,int len)
{
	if(len<2)
		return;

	int min=data[0];
	int point=0;
	int tmp;
	int i;
	for(i=0;i<len;i++)
	{

		if(min > data[i])
		{
			min=data[i];
			point=i;
		}
	}
	tmp=data[0];
	data[0]=data[point];
	data[point]=tmp;

	selectionSort(data+1, len-1);
}
void main()
{

	const int size=4000;
	int *data=(int *)malloc(sizeof(int)*size);

	int i;

	for(i=0;i<size;i++)
	{
		data[i]=rand()%150;
	}
	//prinfdata(data, size);
	selectionSort(data,size);
	prinfdata(data, size);
	printf("end here \n");
}