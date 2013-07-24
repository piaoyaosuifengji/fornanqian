

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
void printmatrix(int matrix[][8])
{
	int i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			printf("%3d ",matrix[i][j]);

		}
		printf("\n");
	}
	printf("\n");

}
int	calculatePriority(int  currentRow,int currentColumn,int * horizontal,int *vertical,int  broad[][8])
{
	int i;
	int nextRow;
	int nextColumn;
	int Priority=0;
	for(i=0;i<8;i++)
	{
		if(broad[currentRow][currentColumn] >0)
		{
			return  0;
		}
		nextRow=vertical[i]+(currentRow);
		nextColumn=horizontal[i]+(currentColumn);
		if(nextRow >=0 &&  nextRow <=7  && nextColumn >=0 &&  nextColumn <=7  && broad[nextRow][nextColumn] ==0)
		{
			Priority++;
		}
	}
	return Priority;
}
void updatapriority(int * horizontal,int *vertical,int  broad[][8],int priority[][8])
{

	//int  currentRow,currentColumn;

	int i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			priority[i][j]=calculatePriority(i,j,  horizontal, vertical,  broad);

		}

	}

}

int getmoveNumber(int * currentRow,int *currentColumn,int * horizontal,int *vertical,int  broad[][8],int priority[][8])
{
	int num=-1;
	int i;
	int nextRow;
	int nextColumn;
	int bestRow;
	int bestColumn;
	int pointOfBest=-1;
	//需要先更新priority
	 //updatapriority( horizontal, vertical,  broad, priority);


	for(i=0;i<8;i++)
	{

		nextRow=vertical[i]+(*currentRow);
		nextColumn=horizontal[i]+(*currentColumn);
		if(nextRow >=0 &&  nextRow <=7  && nextColumn >=0 &&  nextColumn <=7  && broad[nextRow][nextColumn] ==0)
		{
			//return i;
		
			if(pointOfBest<0)
			{
				bestRow=nextRow;
				bestColumn=nextColumn;
				pointOfBest=i;
			}
			else
			{
				if(priority[nextRow][nextColumn] <=  priority[bestRow][bestColumn] )
				{
				bestRow=nextRow;
				bestColumn=nextColumn;
				pointOfBest=i;
				}
			}
		}
	}

	return pointOfBest;
}

void move(int * currentRow,int *currentColumn,int moveNumber,int * horizontal,int *vertical,int  broad[][8],int step)
{
	(*currentRow)+=vertical[moveNumber];
	(*currentColumn)+=horizontal[moveNumber];

	broad[(*currentRow)][*currentColumn]=step;
}
void main()
{


	int i=0,step=0,moveNumber;
	int vertical[8];
	int horizontal[8];
	int currentRow=3;
	int currentColumn=4;
	int currentRowRecord=0;
	int currentColumnRecord=0;
	int broad[8][8]={0};
	int priority[8][8]={{2,3,4,4,4,4,3,3},{3,4,6,6,6,6,4,3},{4,6,8,8,8,8,6,4},{4,6,8,8,8,8,6,4},{4,6,8,8,8,8,6,4},{4,6,8,8,8,8,6,4},{3,4,6,6,6,6,4,3},{2,3,4,4,4,4,3,3}};
	horizontal[i++]=2;
	horizontal[i++]=1;
	horizontal[i++]=-1;
	horizontal[i++]=-2;
	horizontal[i++]=-2;
	horizontal[i++]=-1;
	horizontal[i++]=1;
	horizontal[i++]=2;
	i=0;
	vertical[i++]=-1;
	vertical[i++]=-2;
	vertical[i++]=-2;
	vertical[i++]=-1;
	vertical[i++]=1;
	vertical[i++]=2;
	vertical[i++]=2;
	vertical[i++]=1;
	while(1)
	{
		step++;	
		//决定下一步走向，返回负值说明走入死局
		 moveNumber=getmoveNumber( & currentRow,&currentColumn,  horizontal, vertical,broad,priority);
		if(moveNumber >7 || moveNumber<0)
		{
			
			if(step>=64)
				break;

			//重新开始
			i++;//循环次数记录
			//system("CLS");

			//设置循环开始起跳位置：从上到下，从左往右的规则
			if(currentColumnRecord ==7)
			{
				if(currentRowRecord ==7)
					break;
				else
				{
					currentRowRecord++;
					currentColumnRecord=0;
				}
			}
			else
				currentColumnRecord++;		
			currentRow=currentRowRecord;
			currentColumn=currentColumnRecord;
			//初始化变量，以便重新开始骑士跳
			step=0;
			memset(broad,0,sizeof(int)*64);
			//printf("try again:%d   %d,%d\n",i,currentRow,currentColumn);
			
			continue;

		}
		 move(&currentRow,&currentColumn, moveNumber,  horizontal, vertical,  broad, step);

		 

		 //Sleep(1000);
	}
	
	printmatrix( broad);
	printf("end here \n");
}