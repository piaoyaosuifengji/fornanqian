

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Reversion(char *s,int start,int len)//���ַ���ָ��λ�ô����ַ�����ת
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
void main()
{
	char str[500];
	int p=0;
	printf("�����ַ��� \n");
	
	scanf("%c",str);
	while(str[p++] != '\n')
		scanf("%c",&str[p]);

	str[p]='\0';

	printf("%s \n",str);
	Reversion(str,0,strlen(str)-1);
	printf("%s \n",str);
	printf("end here \n");
}