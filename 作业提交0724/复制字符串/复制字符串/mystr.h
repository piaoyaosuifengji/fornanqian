#pragma once

//#include <iostream>
//using std::string;
class mystr
{

private :
	char *str;
	const int strMaxLen;
	int findEnd(char *s);
public:
	mystr(void);
	mystr(char *t);
	static char * getStr();//从键盘接收数据
	void getStrByIndex(int index,char *s);//返回字符串第index个字符后面的子字符串，包括index，放入s中
	virtual ~mystr(void);
};

