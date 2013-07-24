

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "mystr.h"
void main()
{
	char *s=mystr::getStr();
	char strm[31];
	mystr strObj(s);

	strObj.getStrByIndex(5,strm);
	printf("end here \n");
}