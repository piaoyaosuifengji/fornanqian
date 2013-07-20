

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
		 //��ӡ������
	int i;
	for(i=0;i<len;i++)
	{
		printf("%2d ",data[i]);
	}	
	printf("\n");
}

/*��������������ֵ
			a=a+b;
			b=a-b;
			a=a-b;
*/
#define swap(a,b)  ((a)=(a) - ( (b) = ( (a) = (a) + (b) ) - (b)))
void quick_sort(int s[], int l, int r)//�ݹ�(���縴��)
{

    if (l < r)
    {
              //Swap(s[l], s[(l + r) / 2]); //���м��������͵�һ�������� �μ�ע1
        int i = l, j = r, x = s[l];
        while (i < j)
        {
            while(i < j && s[j] >= x) // ���������ҵ�һ��С��x����
                            j--; 
            if(i < j)
                            s[i++] = s[j];
                    
            while(i < j && s[i] < x) // ���������ҵ�һ�����ڵ���x����
                            i++; 
            if(i < j)
                            s[j--] = s[i];
        }
        s[i] = x; //�˳�ʱ��i����j��
		   
		quick_sort(s, l, i - 1); 

		quick_sort(s, i + 1, r);
    }
}

//�ǵݹ�汾
void qsort(int data[], int left, int right)
{
	if(left>=right)
		return;
	//int i=left;
	//int j=right;
	struct arrayInterval  head;//�ṹstruct arrayInterval������������һ�����������ʼ�±�
	struct arrayInterval  *tmp;
	struct arrayInterval  *currentArry;
	

	//���õ�һ������ռ�
	tmp=(struct arrayInterval  *)malloc(sizeof( struct arrayInterval));
	INIT_LIST_HEAD(&(head.p));
	tmp->left=left;
	tmp->right=right;
	list_add_tail(&(tmp->p),&(head.p));//����̬����Ľڵ�tmp��������
	//ʵ�ַǵݹ飬��Ҫ����������Ҫ����������䣬�������䱣����һ��������
	while(list_if_empty( &(head.p)) == 0)
	{
		currentArry=(struct arrayInterval  *)list_get_tail(&(head.p));

		if ((currentArry->left) < (currentArry->right))
		{
				  //Swap(s[l], s[(l + r) / 2]); //���м��������͵�һ�������� �μ�ע1
			int i = (currentArry->left), j = (currentArry->right), x = data[(currentArry->left)];
			while (i < j)
			{
				while(i < j && data[j] >= x) // ���������ҵ�һ��С��x����
					j--; 
				if(i < j)
					data[i++] = data[j];
                    
				while(i < j && data[i] < x) // ���������ҵ�һ�����ڵ���x����
					i++; 
				if(i < j)
					data[j--] = data[i];
			}
			data[i] = x; //�˳�ʱ��i����j��
		   
			tmp=(struct arrayInterval  *)malloc(sizeof( struct arrayInterval));
			tmp->left=(currentArry->left);
			tmp->right=i - 1;
			list_add_tail(&(tmp->p),&(head.p));//����̬����Ľڵ��������

			tmp=(struct arrayInterval  *)malloc(sizeof( struct arrayInterval));
			tmp->left=i + 1;
			tmp->right=(currentArry->right);
			list_add_tail(&(tmp->p),&(head.p));//����̬����Ľڵ��������
		}
		//�ͷŵ�ǰ�ڵ��ڴ棬ע�⵱ǰ�ڵ�����Ѿ��������ڵ���		
		__list_del_entry(&(currentArry->p));
		free(currentArry);
	}


}
void main()
{
	//32λint��ȡֵ��Χ�� min:-2147483648  max:2147483647
	const int len=12345678;
	int *data=(int *)malloc(sizeof(int)*len);
	int left=0;
	int right=len-1;
	int i;



	for(i=0;i<len;i++)
	{
		data[i]=rand()%15000;
	}	
	//printf("ԭʼ���ݣ� \n");
	//prinfdata(data, len);
	printf("start quick_sort \n");
	 qsort(data, left, right);
	 printf("��������ݣ� \n");
	 prinfdata(data, len);
	printf("end here \n");
}