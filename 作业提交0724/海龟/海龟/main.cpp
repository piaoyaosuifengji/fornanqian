

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct code
{
	int commond;
	int arg;
};
struct CPoint
{
	int x;
	int y;
};
void printfloor(int matrix[][20])
{
	int i,j;
	for(i=0;i<20;i++)
	{
		for(j=0;j<20;j++)
		{
			printf("%d ",matrix[i][j]);

		}
		printf("\n");
	}


}

void tortoiseMove(int floor[][20],struct code *cd ,int codeNum)
{

	int i;
	CPoint current;
	current.x=0;
	current.y=0;
	CPoint last;
	last.x=0;
	last.y=0;
	int tortoisePoint=4;//tortoise正面朝向，上左下右分别对应1,2,3,4
	int penPoint=1;//默认笔尖朝上

	for(i=0;i<codeNum;i++,cd++)
	{
		switch(((cd)->commond))
		{
		case 1:
			penPoint=1;
			break;
		case 2:
			penPoint=0;
			break;
		case 3:
			if(tortoisePoint ==1)
				tortoisePoint=4;
			else
				tortoisePoint--;
			break;
		case 4:
			if(tortoisePoint ==4)
				tortoisePoint=1;
			else
				tortoisePoint++;
			break;
		case 5:
			last.x=current.x;
			last.y=current.y;
			if(tortoisePoint ==1)
			{
				current.y=current.y-cd->arg;
				if(penPoint ==0)
				{
					int i;
					for(i=current.y;i<=last.y;i++)
						floor[current.x][i]=1;
				}
			}
			if(tortoisePoint ==2)
			{
				current.x=current.x-cd->arg;
				if(penPoint ==0)
				{
					int i;
					for(i=current.x;i<=last.x;i++)
						floor[i][current.y]=1;
				}
			}
			if(tortoisePoint ==3)
			{
				current.y=current.y+cd->arg;
				if(penPoint ==0)
				{
					int i;
					for(i=last.y;i<=current.y;i++)
						floor[current.x][i]=1;
				}
			}
			if(tortoisePoint ==4)
			{
				current.x=current.x+cd->arg;
				if(penPoint ==0)
				{
					int i;
					for(i=last.x;i<=current.x;i++)
						floor[i][current.y]=1;
				}
			}
			break;
		case 6:
			printfloor( floor);
			break;
		case 9:
			return;
			break;

		default:break;

		}

	}
}
void main()
{


	struct code cd[200];
	int codeNum=0;//设置画正方形的命令
	cd[codeNum++].commond=2;

	cd[codeNum].commond=5;
	cd[codeNum++].arg=12;

	cd[codeNum++].commond=3;

	cd[codeNum].commond=5;
	cd[codeNum++].arg=12;

	cd[codeNum++].commond=3;

	cd[codeNum].commond=5;
	cd[codeNum++].arg=12;
	cd[codeNum++].commond=3;

	cd[codeNum].commond=5;
	cd[codeNum++].arg=12;
	cd[codeNum++].commond=1;
	cd[codeNum++].commond=6;
	cd[codeNum++].commond=9;
	int floor[20][20]={0};
	printfloor( floor);
	printf(" \n");
		
	tortoiseMove( floor, cd , codeNum);


	printf("end here \n");
}