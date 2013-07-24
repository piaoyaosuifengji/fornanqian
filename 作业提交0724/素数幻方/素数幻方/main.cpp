
#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int find(int m)//寻找最小质因数
{
	int i=2;

	while(m%i !=0)
		i++;
	return i;
}
int getReversal(int d)//获取d反转数
{
	int reversal=0;
	int i=0;

	while(d/10 != 0 || d>0)
	{
		reversal=reversal*10+d%10;
		d=d/10;
	}
	return reversal;
}
int checkReversal(int d)//判断d的是否是可逆素数
{

	int res=0;
	if(d<2)
		return res;

	int reversal=1;
	int tmp;


	res=find(d);//判断d是不是素数
	if(d != res)
		return 0;	


	reversal=getReversal( d);
	if(reversal<1000)
		return 0;
	tmp=find(reversal);
	if(reversal == tmp && reversal>1000)
		return 1;	
	else 
		return 0;
}

 
void findAllPrimeNumber(int *data,int *count)//找到所有1000到9999以内可逆素数存于data，coun为找到个数
{
	int i;
	//printf("输出1000到9999以内 全部素数:\n");
	int res=0;
	//int count=0;
	for(i=1000;i<=9999;i++)
	{
		res=checkReversal(i);
		if(1 == res)
		{
			data[*count]=i;(*count)++;
		}
	}
}
void printMatrix(char matrix[][4])
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			printf("%c",matrix[i][j]);

		}
		printf("\n");
	}


}
int  checkMatrix(char matrix[][4])
{
	int res=0;
	int  matrixNum[4][4];
	int i,j;
	char tmp[2]={0};
	int num;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			tmp[0]=matrix[i][j];
			matrixNum[i][j]=atoi(tmp);

		}
	}
	//一些限制条件
	//矩阵的每一行和最后一行中的各个数字都不能为偶数或5


	for(i=0;i<4;i++)
	{
		if(matrixNum[0][i]== 5 || matrixNum[0][i]%2== 0 || matrixNum[3][i]== 5 || matrixNum[3][i]%2== 0)
			return 0;
	}




	//判断行（其实不用判断行，因为放入的就是）
	//for(i=0;i<4;i++)
	//{
	//	num=0;
	//	for(j=0;j<4;j++)
	//	{
	//		num=matrixNum[i][j]* pow((double)10,j)+num;
	//	}
	//	//判断num是否为可逆素数
	//	res=checkReversal(num);
	//	if(res==0)
	//		return 0;
	//}
	//判断列
	for(i=0;i<4;i++)
	{
		num=0;
		for(j=0;j<4;j++)
		{
			num=matrixNum[j][i]* pow((double)10,j)+num;
		}
		//判断num是否为可逆素数
		res=checkReversal(num);
		if(res==0)
			return 0;
	}
	//判断斜对线
	num=0;
	for(j=0;j<4;j++)
	{
		num=matrixNum[j][j]* pow((double)10,j)+num;
	}
	//判断num是否为可逆素数
	res=checkReversal(num);
	if(res==0)
		return 0;
	num=0;
	for(j=0;j<4;j++)
	{
		num=matrixNum[3-j][j]* pow((double)10,j)+num;
	}
	//判断num是否为可逆素数
	res=checkReversal(num);
	if(res==0)
		return 0;

	return 1;

}
void main()
{
	char matrix[4][4]={0};
	char tmp[5];
	int data[500];
	int checkcount=0;
	
	int a,b,c,d;
	int res= 0;
	int count=0;
	findAllPrimeNumber( data, &count);
	for(a=0;a<count && res !=1;a++)
	{
		for(b=0;b<count && res !=1;b++)
		{
			for(c=0;c<count && res !=1;c++)
			{
			
				for(d=0;d<count && res !=1;d++)
				{
	/*				if(d==203 )
					{
						int xxxx=0;
					}*/
					//将4个数填入数组  108  183  24  57
					itoa(data[a],matrix[0],10);
					itoa(data[b],matrix[1],10);
					itoa(data[c],matrix[2],10);
					itoa(data[d],matrix[3],10);
					//printMatrix( matrix);
					//判断矩阵是否为素数幻方矩阵
					//printf("尝试次数%d ,%d ,%d ,%d ,%d ,\n",++checkcount,a,b,c,d);
				//	int i,j;
				//	i=0;
				//	j=0;
				//	matrix[i][j++]='3';
				//	matrix[i][j++]='7';
				//	matrix[i][j++]='1';
				//	matrix[i][j++]='9';
				//i=1;
				//	j=0;
				//	matrix[i][j++]='9';
				//	matrix[i][j++]='5';
				//	matrix[i][j++]='5';
				//	matrix[i][j++]='1';
				//					i=2;
				//	j=0;
				//	matrix[i][j++]='1';
				//	matrix[i][j++]='2';
				//	matrix[i][j++]='8';
				//	matrix[i][j++]='3';
				//					i=3;
				//	j=0;
				//	matrix[i][j++]='1';
				//	matrix[i][j++]='9';
				//	matrix[i][j++]='3';
				//	matrix[i][j++]='3';

					 res= checkMatrix( matrix);
					 if(res==1)
						printMatrix( matrix);
				}
		
			}
		}
	}
	
	//char s1[500];
	//char s2[500];
	//char s3[500];
	//int res;
	//printf("输入3个字符串 \n");
	//scanf("%s",s1);
	//scanf("%s",s2);
	//scanf("%s",s3);
	//res=strcmp(s1,s2);
	//if(res>0)
	//{	
	//	res=strcmp(s1,s3);
	//	if(res>0)
	//		printf("3个字符串较大者为字符串%s \n",s1);
	//	else
	//		printf("3个字符串较大者为字符串%s \n",s3);
	//}	
	//else
	//{
	//	res=strcmp(s2,s3);
	//	if(res>0)
	//		printf("3个字符串较大者为字符串%s \n",s2);
	//	else
	//		printf("3个字符串较大者为字符串%s \n",s3);
	//}108  183  24  57
	printf("end here \n");
}