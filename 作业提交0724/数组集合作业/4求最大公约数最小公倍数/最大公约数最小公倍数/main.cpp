
#include <math.h>
#include <stdio.h>
int find(int m)//寻找最小质因数
{
	int i=2;
	while(m%i !=0)
		i++;
	return i;
}
int search(int *src,int len,int toSearch,int *loc)//在长度为len的数组src中寻找数toSearch，找到返回1.否则返回0
{
	int res=0;
	int i;
	for(i=0; i<len ;i++)
	{
		if(src[i]==toSearch )
		{
			res=1;
			*loc=i;
			break;
		}
	}

	return res;
}
int count(int *src,int len,int toSearch)//在长度为len的数组src中统计数toSearch出现的次数,并会标记所有找到的数据（变为负数）
{
	int res=0;
	int i;
	for(i=0; i<len ;i++)
	{
		if(src[i]==toSearch )
		{
			src[i]=src[i] *(-1);
			res++;
		}
	}

	return res;
}
void main()
{
	int m,n;
	int rightNumber=1;
	int resForM[15];//保存数字m的所有质数
	int resPointForM=0;
	int resForN[15];//保存数字n的所有质数
	int resPointForN=0;
	int res=1;
	int loc;
	int i,j,haveFind;
	printf("输入俩个数，以空格间隔 \n");
	scanf("%d %d",&m,&n);
	if(m<2 || n<2)
	{
		printf("wrong input \n");
		return;
	}
	while(m>1)//计算m的所有质数，存于数组resForM【】
	{	

		rightNumber=find(m);
		resForM[resPointForM++]=rightNumber;
		m=m/rightNumber;
	}
	while(n>1)
	{	

		rightNumber=find(n);
		resForN[resPointForN++]=rightNumber;
		n=n/rightNumber;
	}
	
	//现在 resForN[15];和 resForM[15]中分别数保存了n，m的约数
	res=1;
	haveFind=0;
	//计算最大公约数：寻找俩个质数数组中的交集，求积
	for(i=0;i<resPointForM;i++)
	{
		for(j=0;j<resPointForN;j++)
		{
			haveFind=search(resForN,resPointForN,resForM[i],&loc);
			if(haveFind ==1)
			{
				//需要标记已经被找到过的约数:方法是变为负数,防止search（）中重复查找
				resForN[loc]=resForN[loc] *(-1);
				res=res * resForM[i];
				break;
			}
			
		}

	}
	//恢复被标记的数据
	for(j=0;j<resPointForN;j++)
	{
		if(resForN[j] <0)
		{
			resForN[j]=resForN[j] *(-1);
		}		
	}
	printf("最大公约数=%d\n",res);
	res=1;
	haveFind=0;
	//计算最小公倍数：最小公倍数等于它们所有的质因数的乘积（如果有几个质因数相同，则比较两数中哪个数有该质因数的个数较多，乘较多的次数）。 
	//分俩次寻找，第一次以resForM【】数组为基准，第二次以resForN【】为基准（找出第一次遍历时漏掉的）

	//第一次遍历
	for(i=0;i<resPointForM;i++)
	{
		for(j=0;j<resPointForN;j++)
		{
				if(resForM[i] >0)
				{
					int count1=count(resForM,resPointForM,resForM[i]);
					int count2=count(resForN,resPointForN,abs(resForM[i]));
					int count=count1>count2 ?count1:count2;
					int lin=resForM[i];
					
					res= (res * pow((double)resForM[i],count));
					//printf("res=%d,count=%d  resForM[i]=%d\n",res,count,resForM[i]);
				}
		}

	}
	//第二次遍历(内部循环可以去掉)
	for(i=0;i<resPointForN;i++)
	{
		for(j=0;j<resPointForM;j++)
		{
				if(resForN[i] >0)
				{
				int count1=count(resForM,resPointForM,resForN[i]);
				int count2=count(resForN,resPointForN,abs(resForN[i]));
				int count=count1>count2 ?count1:count2;
					
					int lin=resForN[i];
				res= (res * pow((double)resForN[i],count));
				//printf("res=%d,count=%d  resForM[i]=%d\n",res,count,resForN[i]);
				}
		}

	}	
	if(res<0)
		res=res*(-1);
	printf("最小公倍数=%d\n",res);



	printf("end here \n");
}