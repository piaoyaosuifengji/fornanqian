#include "stack.h"

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void strResolve(char *s,DataType * d,int *DataTypeNum);
void getPostfixExpression(struct seqstack_  *stack,struct seqstack_  *reselt);
int comparePriority(char operatorA,char operatorB);//���������operatorA���ȼ��ߣ�����1�����򷵻�0����-1������-2

double Calculate(struct seqstack_  *stack);

void main()
{

	char data[100];
	DataType  expression[100];
	int DataTypeNum=0;
    struct seqstack_  stack;
	struct seqstack_  postfixExpression;
	struct seqstack_  tmp;
	inistack( &stack);
	inistack( &tmp);
	inistack( &postfixExpression);
	printf("������ʽ�� \n");
	scanf("%s",data);
	strResolve(data,expression,&DataTypeNum);
	createStackFromArray(&stack,expression,DataTypeNum);

	//���Ƶ�tmp��
	createStackFromStack(&tmp,&stack);
	inistack( &stack);

	getPostfixExpression(&tmp,&postfixExpression);
	printf("��׺���ʽ���£�\n");
	printStack(&postfixExpression);


	createStackFromStack(&stack,&postfixExpression);
	double res=Calculate(&stack);
	printf("res=%f \n",res);
	//printStack(&stack);
	//int i;
	//for(i=0;i<DataTypeNum;i++)
	//	 printDataType(expression[i]);

	printf("end here \n");
}
double Calculate(struct seqstack_  *stack)
{
	struct seqstack_  tmp;
	inistack( &tmp);

	char currentOperator;
	DataType data1;
	DataType data2;
	double   a;
	double  b;
	DataType operators;
	DataType current;
	int end=pop(stack,&current);
	double sum=0;
	while(end ==1 )
	{
		if(current.dataType!=1)
			push(&tmp,current);
		else
		{
			pop(&tmp,&data1);
			pop(&tmp,&data2);
			operators=current;
			if(data1.dataType ==2)
				a=data1.data.integer;
			else
				a=data1.data.doubleNum;

			if(data2.dataType ==2)
				b=data2.data.integer;
			else
				b=data2.data.doubleNum;
			switch(operators.data.operators )
			{
			case '+':
				sum=a+b;
				current.dataType=3;
				current.data.doubleNum=sum;
				push(&tmp,current);
				break;
			case '-':
				sum=a-b;
				current.dataType=3;
				current.data.doubleNum=sum;
				push(&tmp,current);
				break;
			case '*':
				sum=a*b;
				current.dataType=3;
				current.data.doubleNum=sum;
				push(&tmp,current);
				break;
			case '/':
				sum=a/b;
				current.dataType=3;
				current.data.doubleNum=sum;
				push(&tmp,current);
				break;
			default:
				break;
			}
			//sum=

		}
		end=pop(stack,&current);
	}
	end=pop(&tmp,&current);
	if(end ==1)
	{
			if(current.dataType ==2)
				a=current.data.integer;
			else
				a=current.data.doubleNum;	
		
		return a;
	}
	return sum;
}


void getPostfixExpression(struct seqstack_  *stack,struct seqstack_  *reselt)
{
//0������һ�����operators��ջ,
//1�����δ�stack pop������Ϊ��������ֱ�����������reselt��
//2���統ǰ�����currentOperator����ջ�������topOperator����ջ��
//3���統ǰ���������ջ���������ջ���������ջ�����������reselt����ǰ���������ջ��������Ƚϣ�
//4���統ǰ���������ջ�����������ջ�������Ϊ����������ǰ�����Ϊ����������ջ���������ջ����������һ���ţ����Ƕ�����������
//5���統ǰ���������ջ�����������ջ�������Ϊ��#������ǰ�����ҲΪ��#������ջ���������ջ����ʾ���������

	struct seqstack_  operators;
	struct seqstack_  tmp;
	inistack( &operators);
	inistack( reselt);
	inistack( &tmp);
	DataType currentOperator;
	DataType topOperator;
	DataType d;
	int end,hasTop,priority,emp;
	end=pop(stack,&d);
	emp=isEmpty(&operators);
	while(( end) ==1 )//||   emp != 1)
	{
		//printf("222 \n");
		//printStack(stack);
		//printStack(reselt);
		//printStack(&operators);
		//printf("3333 \n");
		if(d.data.operators == '(' || d.data.operators == ')')
		{
			end++;
		}
		switch(d.dataType )
		{

		case 1://�ǲ�����
			currentOperator=d;
			hasTop= gettop(operators,&topOperator);
			if(hasTop !=1)//operatorsΪ��ֱ����ջ
			{
				push(&operators,currentOperator);
				break;
			}
			priority=comparePriority(currentOperator.data.operators,topOperator.data.operators);


			if(priority == 1)//----------------------------------------2
			{
				push(&operators,currentOperator);
				break;
			}
			else if (priority == -1)
			{
	
	
				pop(&operators,&d);//----------------------------------------3
				push(reselt,d);//

				//�ص�case 1�����¿�ʼ��operators�е�top�Ƚ�--����push   currentOperator�����¿�ʼ
				push(stack,currentOperator);
				break;

			}
			else if (priority == 0)
			{
				if(currentOperator.data.operators == ')' && topOperator.data.operators == '(')//------------------4
				{
						pop(&operators,&d);
						break;
				}
			}
			else if (priority == -2)
			{
				break;
			}


			break;
		case 2:
		case 3:
			push(reselt,d);//----------------------------------------1
			break;
		default:
			break;
		}


	end=pop(stack,&d);
	emp=isEmpty(&operators);
	}

	//���operators�е�����װ��reselt��
	while(   isEmpty(&operators) != 1)
	{
				pop(&operators,&d);
				push(reselt,d);

	}

}
int comparePriority(char operatorA,char operatorB)//b��ջ��
{
	int Priority[6][6]={ {1,1,-1,-1,-1,1},{1,1,-1,-1,-1,1},{1,1,1,1,-1,1},{1,1,1,1,-1,1},{-1,-1,-1,-1,-1,0},{1,1,1,1,-2,1},};
	/*
	��	��	��	��	(	)	#
��	>	>	<	<	<	>	>
��	>	>	<	<	<	>	>
��	>	>	>	>	<	>	>
��	>	>	>	>	<	>	>
(	<	<	<	<	<	=	
)	>	>	>	>		>	>
#	<	<	<	<	<		=

*/
	int i,j;
	switch(operatorA )
	{
	case '+':
		j=0;
		break;
	case '-':
		j=1;
		break;
	case '*':
		j=2;
		break;
	case '/':
		j=3;
		break;
	case '(':
		j=4;
		break;
	case ')':
		j=5;
		break;
	default:
		break;
	}
	switch(operatorB )
	{
	case '+':
		i=0;
		break;
	case '-':
		i=1;
		break;
	case '*':
		i=2;
		break;
	case '/':
		i=3;
		break;
	case '(':
		i=4;
		break;
	case ')':
		i=5;
		break;
	default:
		break;
	}


	return (Priority[i][j])*(-1);

}
void strResolve(char *s,DataType * d,int *DataTypeNum)
{
	char subStr[20];
	int pointOFSubStr=0;
	int i,j,k,isInteger;
	j=*DataTypeNum;
	for(i=0;i<strlen(s);i++)
	{
		if(s[i] == '.')
			subStr[pointOFSubStr++]=s[i];
		else if(s[i] >= '0' && s[i] <= '9')
		{
			subStr[pointOFSubStr++]=s[i];

			if(i==(strlen(s)-1))
			{
				//֮ǰ��δ�������
				if(pointOFSubStr != 0)
				{
						subStr[pointOFSubStr]='\0';
					//�ж���int����double
					isInteger=1;
					for(k=0;k<pointOFSubStr;k++)
					{
						if(subStr[k]  == '.')
						{
							isInteger=0;
							break;
						}
					}
					//������
					if(isInteger ==1)
					{
						d[j].dataType=2;
						d[j].data.integer=atoi(subStr);
					}
					else
					{
						d[j].dataType=3;
						d[j].data.doubleNum=atof(subStr);
					}
						j++;pointOFSubStr=0;
				}
			}
		}
		else //if(pointOFSubStr != 0 || s[i] !=0 )
		{
	
				//֮ǰ��δ�������
				if(pointOFSubStr != 0)
				{
						subStr[pointOFSubStr]='\0';
					//�ж���int����double
					isInteger=1;
					for(k=0;k<pointOFSubStr;k++)
					{
						if(subStr[k]  == '.')
						{
							isInteger=0;
							break;
						}
					}
					//������
					if(isInteger ==1)
					{
						d[j].dataType=2;
						d[j].data.integer=atoi(subStr);
					}
					else
					{
						d[j].dataType=3;
						d[j].data.doubleNum=atof(subStr);
					}
						j++;pointOFSubStr=0;
				}
			
			//
			d[j].dataType=1;
			d[j].data.operators=s[i];
			j++;
		
		}

	}

	//DataType * d,int *DataTypeNum
	//�����ŵ����




	*DataTypeNum=j;
}