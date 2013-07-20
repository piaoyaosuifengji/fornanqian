

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
struct arrayInterval
{
	struct  list_head   p;
	int left;
	int right;
};
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

/*交换俩个变量的值
			a=a+b;
			b=a-b;
			a=a-b;
*/
#define swap(a,b)  ((a)=(a) - ( (b) = ( (a) = (a) + (b) ) - (b)))
void quick_sort(int s[], int l, int r)//递归(网络复制)
{

    if (l < r)
    {
              //Swap(s[l], s[(l + r) / 2]); //将中间的这个数和第一个数交换 参见注1
        int i = l, j = r, x = s[l];
        while (i < j)
        {
            while(i < j && s[j] >= x) // 从右向左找第一个小于x的数
                            j--; 
            if(i < j)
                            s[i++] = s[j];
                    
            while(i < j && s[i] < x) // 从左向右找第一个大于等于x的数
                            i++; 
            if(i < j)
                            s[j--] = s[i];
        }
        s[i] = x; //退出时，i等于j。
		   
		quick_sort(s, l, i - 1); 

		quick_sort(s, i + 1, r);
    }
}

//非递归版本
void qsort(int data[], int left, int right)
{
	if(left>=right)
		return;
	//int i=left;
	//int j=right;
	struct arrayInterval  head;//结构struct arrayInterval就是用来保存一个子数组的起始下标
	struct arrayInterval  *tmp;
	struct arrayInterval  *currentArry;
	

	//设置第一个数组空间
	tmp=(struct arrayInterval  *)malloc(sizeof( struct arrayInterval));
	INIT_LIST_HEAD(&(head.p));
	tmp->left=left;
	tmp->right=right;
	list_add_tail(&(tmp->p),&(head.p));//将动态分配的节点tmp加入链表
	//实现非递归，需要保存所有需要子数组的区间，所有区间保存在一个链表当中
	while(list_if_empty( &(head.p)) == 0)
	{
		currentArry=(struct arrayInterval  *)list_get_tail(&(head.p));

		if ((currentArry->left) < (currentArry->right))
		{
				  //Swap(s[l], s[(l + r) / 2]); //将中间的这个数和第一个数交换 参见注1
			int i = (currentArry->left), j = (currentArry->right), x = data[(currentArry->left)];
			while (i < j)
			{
				while(i < j && data[j] >= x) // 从右向左找第一个小于x的数
					j--; 
				if(i < j)
					data[i++] = data[j];
                    
				while(i < j && data[i] < x) // 从左向右找第一个大于等于x的数
					i++; 
				if(i < j)
					data[j--] = data[i];
			}
			data[i] = x; //退出时，i等于j。
		   
			tmp=(struct arrayInterval  *)malloc(sizeof( struct arrayInterval));
			tmp->left=(currentArry->left);
			tmp->right=i - 1;
			list_add_tail(&(tmp->p),&(head.p));//将动态分配的节点加入链表

			tmp=(struct arrayInterval  *)malloc(sizeof( struct arrayInterval));
			tmp->left=i + 1;
			tmp->right=(currentArry->right);
			list_add_tail(&(tmp->p),&(head.p));//将动态分配的节点加入链表
		}
		//释放当前节点内存，注意当前节点可能已经不是最后节点了		
		__list_del_entry(&(currentArry->p));
		free(currentArry);
	}


}
void main()
{
	//32位int的取值范围是 min:-2147483648  max:2147483647
	const int len=12345678;
	int *data=(int *)malloc(sizeof(int)*len);
	int left=0;
	int right=len-1;
	int i;



	for(i=0;i<len;i++)
	{
		data[i]=rand()%15000;
	}	
	//printf("原始数据： \n");
	//prinfdata(data, len);
	printf("start quick_sort \n");
	 qsort(data, left, right);
	 printf("排序后数据： \n");
	 prinfdata(data, len);
	printf("end here \n");
}