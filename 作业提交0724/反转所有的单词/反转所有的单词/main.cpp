

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Reversion(char *s,int start,int len)//把字符串指定位置处的字符串反转
{
	int end=start+len-1;
	char tmp;
	while(start<end)
	{
		tmp=s[start];

		s[start]=s[end];
		s[end]=tmp;
		start++;
		end--;
	}

}

void findAllWords(char str[],int pos[],int *posPoint)
{
	int start=-1;
	int end=0;
	int i=0;
	//int posPoint=0;
	char * c=str;
	*posPoint=0;
	while((*c) != '\0')
	{
		//认为字符串中间只有空格,但是最后一个可能为换行符，
		if((*c) != 32 && (*c) != 10)
		{
			if(start < 0)
			{
				start=i;
			}
			else
			{

			}
		}
		else
		{	if(start < 0)
			{
				
			}
			else
			{
				end=i-1;
				pos[*posPoint]=start;
				(*posPoint)++;
				pos[*posPoint]=end;
				(*posPoint)++;
				start=-1;
			}
		}
		c++;
		i++;
		//(*posPoint)++;
	}
	(c)--;
	if((*c) !=32 && start >= 0)
	{
		//pos[*posPoint]=i-1;
				end=i-1;
				pos[*posPoint]=start;
				(*posPoint)++;
				pos[*posPoint]=end;
				(*posPoint)++;
				start=-1;
	}
}
void main()
{
	int pos[500];
	int posPoint=0;
	char str[500];
	int p=0;
	printf("输入字符串 \n");
	
	scanf("%c",str);
	while(str[p++] != '\n')
		scanf("%c",&str[p]);
	str[p]='\0';





	findAllWords( str,pos,&posPoint);

	printf("%s \n",str);

	int i;
	for(i=0;i<(posPoint);)
	{
		int s=pos[i++];
		int l=pos[i++]-s+1;
		Reversion(str,s,l);

	}
	
	printf("%s \n",str);
	printf("end here \n");
}