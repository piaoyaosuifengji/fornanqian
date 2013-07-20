#include <stdio.h>
#include <stddef.h>
#include <assert.h>

#define SIZE 80

void push(char *stack, int *pos, char c);
char pop(char *stack, int *pos);

int main()
{
	char string[SIZE];
	char stack[SIZE];
	int position = SIZE;

	gets(string);

	int i = 0;
	while('\0' != string[i])
	{
		if(' ' == string[i])
		{
			putchar(' ');
			i = i + 1;
		}
		else
		{
			while('\0' != string[i] && ' ' != string[i])
				push(stack,&position,string[i++]);
			while(SIZE != position)
				putchar(pop(stack,&position));
		}
	}

}

int strlen(const char *str)
{
	assert(NULL != str);
	int i = 0;
	while(*str++)
		++i;
	return i;
}

void push(char *stack, int *pos, char c)
{
	*pos = *pos - 1;
	stack[*pos] = c;
}

char pop(char *stack, int *pos)
{
	char c = stack[*pos];
	*pos = *pos + 1;
	return c;
}
