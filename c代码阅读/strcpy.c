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
  char *__unbounded s = (char *__unbounded) CHECK_BOUNDS_LOW (src);/* 检查指针src的值是否 >= low，返回原来的指针值 */ 
  const ptrdiff_t off = CHECK_BOUNDS_LOW (dest) - s - 1;//求出src和dest的距离，用于后面的赋值。注意－1
  size_t n;

  do
    {
      c = *s++;//第一次到此处，s == dest, off == dest - s - 1,  
      s[off] = c;//第一次到此处，s == dest + 1 , s[off] == dest 
    }
  while (c != '\0');

  n = s - src;
  (void) CHECK_BOUNDS_HIGH (src + n);//用来检测src+n是否合法的宏
  (void) CHECK_BOUNDS_HIGH (dest + n);

  return dest;
}
libc_hidden_builtin_def (strcpy)


/*
	GCC支持bounded类型指针（bounded指针用__bounded关键字标出，若默认为bounded指针，则普通指针用__unbounded标出），这种指针占用3个指针的空间，
	在第一个空间里存储原指针的值，第二个空间里存储下限值，第三个空间里存储上限值。__ptrvalue、__ptrlow、__ptrhigh分别返回这3个值，有了3个值以后，
	内存越界错误便很容易查出来了。并且要定义了__BOUNDED_POINTERS__这个宏才有作用，否则这3个宏定义都是空的。

不过，尽管bounded指针看上去似乎很有用，但是这个功能却在2003年被去掉了。因此现在所有关于bounded指针的关键字其实都是一个空的宏。

不过还是分析下这几个宏的作用吧。__ptrvalue (ARG) < __ptrlow (ARG)判断目标指针是否小于合法指针的下界，如果其结果为真，即指针越界，
则执行 && 后面的BOUNDS_VIOLATED 陷入中断程序；反之，指针没有越界，则不执行BOUNDS_VIOLATED，整个表达式的值为逗号表达式后面的值，即__ptrvalue (ARG)，
即目标指针的值。 这么写主要是为了后面可以直接对CHECK_BOUNDS_LOW(ARG) 进行操作。
好了，鉴于此， 以上代码等价于一下代码：

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
值得指出的是：此算法利用了进程平坦的内存模型，虚拟内存平坦铺开，于是任意两个指针的差就是两者之间的距离。得到地址间的相对距离off后，
就不需要再用绝对地址寻址了，这样在每一次的循环中可以少一次dest++操作，而多出来的相对地址操作则完全可以用寄存器高效地完成！



一般glibc会根据不同的CPU实现不同版本的库函数，如strcpy。
*/



其他版本：
char* strcpy1(char *d, const char *s)
{
	char *r=d;
	if ((d==NULL)||(s==NULL))
	   exit(-1);
	while((*d++=*s++));
return r;
}