

#include <stdio.h>


void main()
{
	double a,b,c;
	//double res;
	int res=3.6;
	scanf("%lf %l9 %lf",&a,&b,&c);
	res=100*(a+b+c)/3;
	//���ݸ���������������ת��ʱ�Զ�ȥ��С��������ʵ����������
	if(res%10  >4)
		res=((res/10) +1)*10;
	else
		res=((res/10) )*10;
	printf("%lf",((double)res/100));
	printf("end here \n");
}