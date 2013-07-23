
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
  unsigned long int longword, himagic, lomagic;//4���ֽ�

  /* Handle the first few characters by reading one character at a time.
     Do this until CHAR_PTR is aligned on a longword boundary.

���ݶ���(data alignment)����ָ�������ڵ��ڴ��ַ�����Ǹ����ݳ��ȵ�������������CPU�Ĵ�ȡ�ٶ���졣
������32λ�ļ�����У�һ��WORDΪ4 byte����WORD���ݵ���ʼ��ַ�ܱ�4������ʱ��CPU�Ĵ�ȡЧ�ʱȽϸߡ�

	 ʵ�������ݶ��룬����ڶ���֮ǰ������'\0'�����ֱ��return char_ptr - str;
	 ѭ���е��жϣ�
	 һ����˵��char_ptr��4�ֽڶ���ģ�����˵����ĩβ��Ϊ��00,
		char_ptr
	&
		     011
��������һ��ѭ�����ж�4-byte������Ǹ���ַ,����һ��ı��������ݽ������ַ���ָ�붼��4-byte�����'
,����ⲽ�ںܶ����������������. 	 */

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
��һ����(longword - lomagic) ������ʽ�������ж����λ��,������˼��:
���longword���κ�һ���ֽ��������0x80���ߵ���0�������,��������ֽڵ����λ��ֵ�����ǲ�����0��. 

�ڶ�����(~longword & himagic)������ʽ�������ж�longword��ÿ���ֽڵ����λ������,
Ҳ�����ж�longword�Ƿ�С��0x80,���С��0x80��������ʽ��ֵ��himagic(0x80808080).�������0. 


����������ֺϲ������պþ������longword��ĳ���ֽ�С��0x80,����ĳ���ֽ�Ϊ0,���������ʽ��ֵ��Ϊ0. 

������֪�����������ַ�����ÿ���ַ��Ĵ�С��ascii��ķ�Χ����[0,127],Ҳ���ǿ϶���С��0x80��.����������ʽ�͸պ���������Ҫ��. */
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


	�򵥰汾
size_t strlen_b(const char * str) 
{
     const char *cp =  str;
     while (*cp++ )
          ;
     return (cp - str - 1 );
}