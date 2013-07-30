#include "stack.h"
#include <stdio.h>




int push(struct seqstack_ *s,DataType x)
{

   if (s->top==maxlen-1)
    {  
		printf("\nStack is full");
        return 0;
     }
  s->top++;
  (s->stack[s->top])=x;
  return 1;
}


void inistack(struct seqstack_ *s)
{
  s->top=-1;
}

int empty(struct seqstack_ s)
{
   if(s.top==-1)
     return 1;
  else
    return 0;
}
int pop(struct seqstack_ *s,DataType *d)
{
	if(s->top==-1)
   { 
	   //printf("\n Stack is empty!");
     return 0;
	} 
else 
   {  *d= s->stack[s->top]; 
     (s->top)--; 
     return 1;
	}
}

int gettop(struct seqstack_ s,DataType *d)
{
  if(s.top==-1)
    {
		//printf("\nStack is empty!");
      return 0;
    }
  else 
    { *d=s.stack[s.top];
      return 1;
    }
}
void printDataType(DataType d)
{
	switch(d.dataType)
	{
	case 1:
		printf("%c ",d.data.operators);break;
	case 2:
		printf("%d ",d.data.integer);break;
	case 3:
		printf("%.3f ",d.data.doubleNum);break;
	default:break;
	}

}
void printStack(struct seqstack_ *s)
{
	DataType d;
	int c=s->top;
	//int res=pop(s,&d);
	int i=0;
	while(i<=c)
	{
		d=s->stack[i];
		printDataType(d);
		i++;
		//res=pop(s,&d);
	}
	printf("\n");
}

void createStackFromArray(struct seqstack_ *s,DataType *d,int dataCount)
{
	int i,res;
	for(i=0;i<dataCount;i++)
	{

		res=push( s,d[i]);
	}



}

void createStackFromStack(struct seqstack_ *s,struct seqstack_ *old)
{
	DataType d;

	int res=pop(old,&d);

	while(res==1)
	{
		push(s,d);
		res=pop(old,&d);
	}
	printf("\n");

}

int isEmpty(struct seqstack_ *s)
{
	if(s->top == -1)
		return 1;
	else
		return 0;

}