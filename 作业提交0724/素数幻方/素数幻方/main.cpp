
#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int find(int m)//Ѱ����С������
{
	int i=2;

	while(m%i !=0)
		i++;
	return i;
}
int getReversal(int d)//��ȡd��ת��
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
int checkReversal(int d)//�ж�d���Ƿ��ǿ�������
{

	int res=0;
	if(d<2)
		return res;

	int reversal=1;
	int tmp;


	res=find(d);//�ж�d�ǲ�������
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

 
void findAllPrimeNumber(int *data,int *count)//�ҵ�����1000��9999���ڿ�����������data��counΪ�ҵ�����
{
	int i;
	//printf("���1000��9999���� ȫ������:\n");
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
	//һЩ��������
	//�����ÿһ�к����һ���еĸ������ֶ�����Ϊż����5


	for(i=0;i<4;i++)
	{
		if(matrixNum[0][i]== 5 || matrixNum[0][i]%2== 0 || matrixNum[3][i]== 5 || matrixNum[3][i]%2== 0)
			return 0;
	}




	//�ж��У���ʵ�����ж��У���Ϊ����ľ��ǣ�
	//for(i=0;i<4;i++)
	//{
	//	num=0;
	//	for(j=0;j<4;j++)
	//	{
	//		num=matrixNum[i][j]* pow((double)10,j)+num;
	//	}
	//	//�ж�num�Ƿ�Ϊ��������
	//	res=checkReversal(num);
	//	if(res==0)
	//		return 0;
	//}
	//�ж���
	for(i=0;i<4;i++)
	{
		num=0;
		for(j=0;j<4;j++)
		{
			num=matrixNum[j][i]* pow((double)10,j)+num;
		}
		//�ж�num�Ƿ�Ϊ��������
		res=checkReversal(num);
		if(res==0)
			return 0;
	}
	//�ж�б����
	num=0;
	for(j=0;j<4;j++)
	{
		num=matrixNum[j][j]* pow((double)10,j)+num;
	}
	//�ж�num�Ƿ�Ϊ��������
	res=checkReversal(num);
	if(res==0)
		return 0;
	num=0;
	for(j=0;j<4;j++)
	{
		num=matrixNum[3-j][j]* pow((double)10,j)+num;
	}
	//�ж�num�Ƿ�Ϊ��������
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
					//��4������������  108  183  24  57
					itoa(data[a],matrix[0],10);
					itoa(data[b],matrix[1],10);
					itoa(data[c],matrix[2],10);
					itoa(data[d],matrix[3],10);
					//printMatrix( matrix);
					//�жϾ����Ƿ�Ϊ�����÷�����
					//printf("���Դ���%d ,%d ,%d ,%d ,%d ,\n",++checkcount,a,b,c,d);
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
	//printf("����3���ַ��� \n");
	//scanf("%s",s1);
	//scanf("%s",s2);
	//scanf("%s",s3);
	//res=strcmp(s1,s2);
	//if(res>0)
	//{	
	//	res=strcmp(s1,s3);
	//	if(res>0)
	//		printf("3���ַ����ϴ���Ϊ�ַ���%s \n",s1);
	//	else
	//		printf("3���ַ����ϴ���Ϊ�ַ���%s \n",s3);
	//}	
	//else
	//{
	//	res=strcmp(s2,s3);
	//	if(res>0)
	//		printf("3���ַ����ϴ���Ϊ�ַ���%s \n",s2);
	//	else
	//		printf("3���ַ����ϴ���Ϊ�ַ���%s \n",s3);
	//}108  183  24  57
	printf("end here \n");
}