

#include <string.h>
#include <memcopy.h>

#undef strcat

/* Append SRC on the end of DEST.  */
char *
strcat (dest, src)
     char *dest;//�����C����ԭʼ����ŵ��д����
				//�����C�ǲ�֧��ԭ��д���ġ�
     const char *src;
{
  char *s1 = dest;
  const char *s2 = src;
  char c;

  /* Find the end of the string.  */
  do
    c = *s1++;
  while (c != '\0');

  /* Make S1 point before the next character, so we can increment
     it while memory is read (wins on pipelined cpus).  */
  s1 -= 2;

  do
    {
      c = *s2++;
      *++s1 = c;
    }
  while (c != '\0');

  return dest;
}
libc_hidden_builtin_def (strcat)
