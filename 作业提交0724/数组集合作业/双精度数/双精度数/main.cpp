

#include <stdio.h>


void main()
{
	double a,b,c;
	//double res;
	int res=3.6;
	scanf("%lf %l9 %lf",&a,&b,&c);
	res=100*(a+b+c)/3;
	//根据浮点数向整形类型转换时自动去掉小数部分来实现四舍五入
	if(res%10  >4)
		res=((res/10) +1)*10;
	else
		res=((res/10) )*10;
	printf("%lf",((double)res/100));
	printf("end here \n");
}