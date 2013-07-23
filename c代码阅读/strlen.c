
#include <string.h>
#include <stdlib.h>

#undef strlen

/* Return the length of the null-terminated string STR.  Scan for
   the null terminator quickly by testing four bytes at a time.  */
size_t
strlen (str)
     const char *str;
{
  const char *char_ptr;
  const unsigned long int *longword_ptr;
  unsigned long int longword, himagic, lomagic;//4个字节

  /* Handle the first few characters by reading one character at a time.
     Do this until CHAR_PTR is aligned on a longword boundary.

数据对齐(data alignment)，是指数据所在的内存地址必须是该数据长度的整数倍，这样CPU的存取速度最快。
比如在32位的计算机中，一个WORD为4 byte，则WORD数据的起始地址能被4整除的时候CPU的存取效率比较高。

	 实现了数据对齐，如果在对齐之前就遇到'\0'则可以直接return char_ptr - str;
	 循环中的判断：
	 一般来说，char_ptr是4字节对齐的，就是说它的末尾俩为是00,
		char_ptr
	&
		     011
这里是做一个循环来判断4-byte对齐的那个地址,这里一般的编译器传递进来的字符串指针都是4-byte对其的'
,因此这步在很多编译器都是跳过的. 	 */

  for (char_ptr = str; ((unsigned long int) char_ptr
			& (sizeof (longword) - 1)) != 0;
       ++char_ptr)
    if (*char_ptr == '\0')
      return char_ptr - str;

  /* All these elucidatory comments refer to 4-byte longwords,
     but the theory applies equally well to 8-byte longwords.  */

  longword_ptr = (unsigned long int *) char_ptr;

  /* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
     the "holes."  Note that there is a hole just to the left of
     each byte, with an extra at the end:

     bits:  01111110 11111110 11111110 11111111
     bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD

     The 1-bits make sure that carries propagate to the next 0-bit.
     The 0-bits provide holes for carries to fall into.  */
  himagic = 0x80808080L;
  lomagic = 0x01010101L;
  if (sizeof (longword) > 4)
    {
      /* 64-bit version of the magic.  */
      /* Do the shift in two steps to avoid a warning if long has 32 bits.  */
      himagic = ((himagic << 16) << 16) | himagic;
      lomagic = ((lomagic << 16) << 16) | lomagic;
    }
  if (sizeof (longword) > 8)
    abort ();

  /* Instead of the traditional loop which tests each character,
     we will test a longword at a time.  The tricky part is testing
     if *any of the four* bytes in the longword in question are zero.
第一部分(longword - lomagic) 这个表达式是用来判断最高位的,它的意思是:
如果longword的任何一个字节如果大于0x80或者等于0的情况下,它的这个字节的最高位的值都会是不等于0的. 

第二部分(~longword & himagic)这个表达式是用来判断longword的每个字节的最高位的设置,
也就是判断longword是否小于0x80,如果小于0x80则这个表达式的值是himagic(0x80808080).否则就是0. 


因此这两部分合并起来刚好就是如果longword的某个字节小于0x80,并且某个字节为0,则整个表达式的值不为0. 

而我们知道传进来的字符串的每个字符的大小的ascii码的范围就是[0,127],也就是肯定是小于0x80的.所以这个表达式就刚好是我们所要的. */
  for (;;)
    {
      longword = *longword_ptr++;

      if (((longword - lomagic) & ~longword & himagic) != 0)
	{
	  /* Which of the bytes was the zero?  If none of them were, it was
	     a misfire; continue the search.  */

	  const char *cp = (const char *) (longword_ptr - 1);

	  if (cp[0] == 0)
	    return cp - str;
	  if (cp[1] == 0)
	    return cp - str + 1;
	  if (cp[2] == 0)
	    return cp - str + 2;
	  if (cp[3] == 0)
	    return cp - str + 3;
	  if (sizeof (longword) > 4)
	    {
	      if (cp[4] == 0)
		return cp - str + 4;
	      if (cp[5] == 0)
		return cp - str + 5;
	      if (cp[6] == 0)
		return cp - str + 6;
	      if (cp[7] == 0)
		return cp - str + 7;
	    }
	}
    }
}
libc_hidden_builtin_def (strlen)


	简单版本
size_t strlen_b(const char * str) 
{
     const char *cp =  str;
     while (*cp++ )
          ;
     return (cp - str - 1 );
}