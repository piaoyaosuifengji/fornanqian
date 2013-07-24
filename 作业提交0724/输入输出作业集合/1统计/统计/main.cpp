

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

void main()
{
	printf("ÇëÊäÈëÒ»ÐÐ×Ö·û \n");
	char c=0;
	char countAlphabet=0;
	char countSpace=0;
	char countDigital=0;
	while(c != '\n')
	{
		scanf("%c",&c);
		if(c == ' ')
			countSpace++;
		if( (c >= '0') && (c<='9'))
			countDigital++;
		if( (c >= 'a') && (c<='z'))
			countAlphabet++;
		if( (c >= 'A') && (c<='Z'))
			countAlphabet++;

	}
	printf("Alphabet=%d,Space=%d,Digital=%d \n",countAlphabet,countSpace,countDigital);

	printf("end here \n");
}