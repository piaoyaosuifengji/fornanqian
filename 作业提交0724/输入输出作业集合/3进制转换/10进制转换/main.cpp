

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
	printf("����һ��10������ \n");
	scanf("%d",&dataDecimal);

	sprintf(dataOctets,"%o",dataDecimal);
	sprintf(dataHex,"%x",dataDecimal);
	itoa(dataDecimal,dataBinary, 2); 
	printf("�����Ӧ��2,8,16�������ֱ�Ϊ�� \n");
	printf("%s \n",dataBinary);
	printf("%s \n",dataOctets);
	printf("%s \n",dataHex);
	printf("end here \n");
}