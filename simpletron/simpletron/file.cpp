



#include <stdlib.h>
#include <stdio.h>
#include "file.h"
#include <string.h>




//读取文件第n行
char * readFileLine(char * file,int lineNumber)
{
	if(!file  || lineNumber<1 )
	{
		return NULL;
	}
	int count=0;
	const int lineLength=200;
	char *line=(char *)malloc(sizeof(char)*lineLength);
	FILE *fp;

	fp=fopen(file,"r");
	while(fgets(line,lineLength,fp) )
	{
		//printf("%s",line);
		count++;
		if(count ==lineNumber)
			break;
	}
	if(count !=lineNumber)
	{
		free(line);
		line=NULL;
	}
	fclose(fp);
	return line;
}


//读取SML代码文件,将指令信息存于code中,返回取得的代码行数
int loadSMLCode(char * file , int  code[][5])
{
	int codeLinede=0;
	const int lineLength=200;
	char *line;
	FILE *fp;
	char tmp[]="1";
	if(!file  || !code)
	{
		return codeLinede;
	}
	line=(char *)malloc(sizeof(char)*lineLength);
	fp=fopen(file,"r");
	while(fgets(line,lineLength,fp) )
	{
		int len=strlen(line);
		if(strlen(line) <5)
			break;
		code[codeLinede][0]=line[0];
		tmp[0]=line[1];
		code[codeLinede][1]=atoi(tmp);
		tmp[0]=line[2];
		code[codeLinede][2]=atoi(tmp);
		tmp[0]=line[3];
		code[codeLinede][3]=atoi(tmp);
		tmp[0]=line[4];
		code[codeLinede][4]=atoi(tmp);
		//printf("%c,%d \n",symbol[codeLinede],code[codeLinede]);
		codeLinede++;
	}

	free(line);
	fclose(fp);

	return codeLinede;
}







