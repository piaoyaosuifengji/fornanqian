
#include <math.h>
#include <stdio.h>
int find(int m)//Ѱ����С������
{
	int i=2;
	while(m%i !=0)
		i++;
	return i;
}
int search(int *src,int len,int toSearch,int *loc)//�ڳ���Ϊlen������src��Ѱ����toSearch���ҵ�����1.���򷵻�0
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
int count(int *src,int len,int toSearch)//�ڳ���Ϊlen������src��ͳ����toSearch���ֵĴ���,�����������ҵ������ݣ���Ϊ������
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
	int resForM[15];//��������m����������
	int resPointForM=0;
	int resForN[15];//��������n����������
	int resPointForN=0;
	int res=1;
	int loc;
	int i,j,haveFind;
	printf("�������������Կո��� \n");
	scanf("%d %d",&m,&n);
	if(m<2 || n<2)
	{
		printf("wrong input \n");
		return;
	}
	while(m>1)//����m��������������������resForM����
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
	
	//���� resForN[15];�� resForM[15]�зֱ���������n��m��Լ��
	res=1;
	haveFind=0;
	//�������Լ����Ѱ���������������еĽ��������
	for(i=0;i<resPointForM;i++)
	{
		for(j=0;j<resPointForN;j++)
		{
			haveFind=search(resForN,resPointForN,resForM[i],&loc);
			if(haveFind ==1)
			{
				//��Ҫ����Ѿ����ҵ�����Լ��:�����Ǳ�Ϊ����,��ֹsearch�������ظ�����
				resForN[loc]=resForN[loc] *(-1);
				res=res * resForM[i];
				break;
			}
			
		}

	}
	//�ָ�����ǵ�����
	for(j=0;j<resPointForN;j++)
	{
		if(resForN[j] <0)
		{
			resForN[j]=resForN[j] *(-1);
		}		
	}
	printf("���Լ��=%d\n",res);
	res=1;
	haveFind=0;
	//������С����������С�����������������е��������ĳ˻�������м�����������ͬ����Ƚ��������ĸ����и��������ĸ����϶࣬�˽϶�Ĵ������� 
	//������Ѱ�ң���һ����resForM��������Ϊ��׼���ڶ�����resForN����Ϊ��׼���ҳ���һ�α���ʱ©���ģ�

	//��һ�α���
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
	//�ڶ��α���(�ڲ�ѭ������ȥ��)
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
	printf("��С������=%d\n",res);



	printf("end here \n");
}