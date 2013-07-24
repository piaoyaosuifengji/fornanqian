

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
	int dataDecimal;
	char dataBinary[150] ;
	char dataOctets[150] ;
	char dataHex[150];
	printf("输入一个10进制数 \n");
	scanf("%d",&dataDecimal);

	sprintf(dataOctets,"%o",dataDecimal);
	sprintf(dataHex,"%x",dataDecimal);
	itoa(dataDecimal,dataBinary, 2); 
	printf("输出相应的2,8,16进制数分别为： \n");
	printf("%s \n",dataBinary);
	printf("%s \n",dataOctets);
	printf("%s \n",dataHex);
	printf("end here \n");
}