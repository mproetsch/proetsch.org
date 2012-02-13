#include <stdio.h>
#include <ctype.h>
#include <string.h>

void getKey(char *password)
{
  int ch;
  char pword[30];
  int i = 0;

  puts ("Enter your password");
  fflush(stdout);

  while ((ch = getch()) != EOF
          && ch != '\n'
          && ch != '\r'
          && i < sizeof(pword) - 1)
  {
    if (ch == '\b' && i > 0)
    {
      printf("\b \b");
      fflush(stdout);
      i--;
      pword[i] = '\0';
    }
    else if (isalnum(ch))
    {
      putchar('*');
      pword[i++] = (char)ch;
    }
  }

  pword[i] = '\0';

  strcpy(password, pword);

}
