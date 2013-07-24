

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
	char s1[500];
	char s2[500];
	char s3[500];
	int res;
	printf("输入3个字符串 \n");
	scanf("%s",s1);
	scanf("%s",s2);
	scanf("%s",s3);
	res=strcmp(s1,s2);
	if(res>0)
	{	
		res=strcmp(s1,s3);
		if(res>0)
			printf("3个字符串较大者为字符串%s \n",s1);
		else
			printf("3个字符串较大者为字符串%s \n",s3);
	}	
	else
	{
		res=strcmp(s2,s3);
		if(res>0)
			printf("3个字符串较大者为字符串%s \n",s2);
		else
			printf("3个字符串较大者为字符串%s \n",s3);
	}
	printf("end here \n");
}