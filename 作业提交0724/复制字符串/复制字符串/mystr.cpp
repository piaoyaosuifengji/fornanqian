#include "mystr.h"
#include <iostream>
using namespace std;
mystr::mystr(void):strMaxLen(30)
{
	str=NULL;
	//strMaxLen=30;
}

mystr::mystr(char * t):strMaxLen(30)
{
	str=NULL;
	int len=findEnd(t);
	if(len >0)
		str=(char *)malloc(sizeof(char)*(len+1));

	strcpy(str,t);
	cout<<"mystr created  \n";
	printf("%s \n",str);
}
mystr::~mystr(void)
{
	if(str)
		free(str);
}
int mystr::findEnd(char *s)//当字符串s长度小于strMaxLen时返回其长度，否则返回-1
{

	int i=-1;
	char c;
	if(s == NULL)
	{
		return i;
	}
	for(i=0;i<=strMaxLen;i++)
	{
		c=s[i];
		if(c == '\0')
		{
			break;
		}
	}

	if(i <strMaxLen)
	{
		return i;
	}
	else
		return -1;
}

 char *  mystr::getStr()
{
	printf("请输入一个不超过30个字符的字符串 \n");
	char *s=(char *)malloc(sizeof(char)*31);
	scanf("%s",s);
	return s;
}

void  mystr::getStrByIndex(int index,char *s)
{

	if(s==NULL)
		return;
	//int myStrlen=strlen(str);
	strcpy(s,(str+index-1));
	printf("get str after index %d is :%s \n",index,s);
}