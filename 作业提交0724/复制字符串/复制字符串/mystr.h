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
	static char * getStr();//�Ӽ��̽�������
	void getStrByIndex(int index,char *s);//�����ַ�����index���ַ���������ַ���������index������s��
	virtual ~mystr(void);
};

