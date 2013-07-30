#ifndef  STACK_H
#define  STACK_H


    /* Êý×ÖºÍÔËËã·û */  
 union  expressionData
    {  
        char operators;    //dataType=1
        int integer;		//dataType=2
		double doubleNum;	//dataType=3
    }; 

typedef struct node
{
 
	expressionData data;
	int dataType;
}DataType;

typedef  node  DataType;
//typedef  int  DataType;
#define  maxlen  200

  struct seqstack_
{ 
	DataType stack[maxlen];
    int top;     
};     


//int push(struct seqstack s);
int push(struct seqstack_ * s,DataType x);
void inistack(struct seqstack_ *s);
int empty(struct seqstack_ s);
int pop(struct seqstack_ *s,DataType *d);
void createStackFromArray(struct seqstack_ *s,DataType *d,int dataCount);
void createStackFromStack(struct seqstack_ *s,struct seqstack_ *old);
void printStack(struct seqstack_ *s);
void printDataType(DataType d);
int gettop(struct seqstack_ s,DataType *d);
int isEmpty(struct seqstack_ *s);


#endif
