/*GNU C Library fuc */

#include <stddef.h>
#include <string.h>
#include <memcopy.h>
#include <bp-checks.h>

#undef strcpy


//# define CHECK_BOUNDS_LOW(ARG) (ARG)
//# define CHECK_BOUNDS_HIGH(ARG) (ARG)
# define __unbounded	/* nothing */
# define CHECK_BOUNDS_LOW(ARG)					\
  (((__ptrvalue (ARG) < __ptrlow (ARG)) && BOUNDS_VIOLATED),	\
   __ptrvalue (ARG))



/* Copy SRC to DEST.  */
char *
strcpy (dest, src)
     char *dest;
     const char *src;
{
  char c;
  char *__unbounded s = (char *__unbounded) CHECK_BOUNDS_LOW (src);/* ���ָ��src��ֵ�Ƿ� >= low������ԭ����ָ��ֵ */ 
  const ptrdiff_t off = CHECK_BOUNDS_LOW (dest) - s - 1;//���src��dest�ľ��룬���ں���ĸ�ֵ��ע�⣭1
  size_t n;

  do
    {
      c = *s++;//��һ�ε��˴���s == dest, off == dest - s - 1,  
      s[off] = c;//��һ�ε��˴���s == dest + 1 , s[off] == dest 
    }
  while (c != '\0');

  n = s - src;
  (void) CHECK_BOUNDS_HIGH (src + n);//�������src+n�Ƿ�Ϸ��ĺ�
  (void) CHECK_BOUNDS_HIGH (dest + n);

  return dest;
}
libc_hidden_builtin_def (strcpy)


/*
	GCC֧��bounded����ָ�루boundedָ����__bounded�ؼ��ֱ������Ĭ��Ϊboundedָ�룬����ָͨ����__unbounded�����������ָ��ռ��3��ָ��Ŀռ䣬
	�ڵ�һ���ռ���洢ԭָ���ֵ���ڶ����ռ���洢����ֵ���������ռ���洢����ֵ��__ptrvalue��__ptrlow��__ptrhigh�ֱ𷵻���3��ֵ������3��ֵ�Ժ�
	�ڴ�Խ����������ײ�����ˡ�����Ҫ������__BOUNDED_POINTERS__�����������ã�������3���궨�嶼�ǿյġ�

����������boundedָ�뿴��ȥ�ƺ������ã������������ȴ��2003�걻ȥ���ˡ�����������й���boundedָ��Ĺؼ�����ʵ����һ���յĺꡣ

�������Ƿ������⼸��������ðɡ�__ptrvalue (ARG) < __ptrlow (ARG)�ж�Ŀ��ָ���Ƿ�С�ںϷ�ָ����½磬�������Ϊ�棬��ָ��Խ�磬
��ִ�� && �����BOUNDS_VIOLATED �����жϳ��򣻷�֮��ָ��û��Խ�磬��ִ��BOUNDS_VIOLATED���������ʽ��ֵΪ���ű��ʽ�����ֵ����__ptrvalue (ARG)��
��Ŀ��ָ���ֵ�� ��ôд��Ҫ��Ϊ�˺������ֱ�Ӷ�CHECK_BOUNDS_LOW(ARG) ���в�����
���ˣ����ڴˣ� ���ϴ���ȼ���һ�´��룺

[cpp] view plaincopyprint?
char *  
strcpy(char *dest, const char *src)  
 {  
     char c;  
     char *s = src;  
     const ptrdiff_t off = dest - s - 1;  
     do {  
          c = *s++;  
          s[off] = c;  
     } while (c != '\0');  
     return dest;  
}  
ֵ��ָ�����ǣ����㷨�����˽���ƽ̹���ڴ�ģ�ͣ������ڴ�ƽ̹�̿���������������ָ��Ĳ��������֮��ľ��롣�õ���ַ�����Ծ���off��
�Ͳ���Ҫ���þ��Ե�ַѰַ�ˣ�������ÿһ�ε�ѭ���п�����һ��dest++�����������������Ե�ַ��������ȫ�����üĴ�����Ч����ɣ�



һ��glibc����ݲ�ͬ��CPUʵ�ֲ�ͬ�汾�Ŀ⺯������strcpy��
*/



�����汾��
char* strcpy1(char *d, const char *s)
{
	char *r=d;
	if ((d==NULL)||(s==NULL))
	   exit(-1);
	while((*d++=*s++));
return r;
}