/* ==============
 *    pwgen.c
 * ==============
 * Written by Matthew Proetsch
 * Using open-source aescrypt.exe by Packetizer, Inc
 * Available from http://www.aescrypt.com/download.html

 * A simple console-based keychain
 * Name your accounts something you won't forget, and
 * pwgen will copy your password for that account to the clipboard
 * or, if one doesn't exist, generate one
 */

#include <stdio.h>
#include <string.h>

#DEFINE NAMELEN 64
#DEFINE PASSLEN 12
#DEFINE FILENAME "keychain.kcn"
#DEFINE FILENAME_ENCRYPTED "keychain.aes"

#IFDEF _WIN32
#DEFINE AESCRYPT "aescrypt.exe"
#DEFINE DELCMD "DEL"
#ENDIF

#IFDEF __linux__
#DEFINE AESCRYPT "aescrypt"
#DEFINE DELCMD "rm"
#ENDIF

typedef struct _Passfield {

    char domain[NAMELEN+1];
    char password[PASSLEN+1];

} Passfield;

Passfield pwgen(char *);
void getKey(char*);


int main(int argc, char **argv[]) {

    if(argc > 1) {
        printf("\tPWGen doesn't take any arguments\n");
        printf("\tBind it to a hotkey (I use Ctrl+F6)\n");
        printf("\tand call it when you need to enter a password.\n");

    }
    else {
        char encryptionKey[BUFSIZ];
        getKey(encryptionKey);

        char commandStr[BUFSIZ];
        strcpy(commandStr, AESCRYPT);
        strcat(commandStr, " -d -p "); //--decrypt, --password:
        strcat(commandStr, encryptionKey);
        strcat(commandStr, " -o ");
        strcat(commandStr, FILENAME);
        strcat(commandStr, " ");
        strcat(commandStr, FILENAME_ENCRYPTED);

        FILE *pwRead = fopen(FILENAME, "r");
        if(pwRead == NULL) {
            printf("Generating new keychain %s\n", FILENAME);
            pwRead = fopen(FILENAME, "w");
        }

        //We expect the file to have the form
        //DOMAIN1 PASSWORD1
        //DOMAIN2 PASSWORD2
        //and so on



    ///Check to see if the domain that the user entered is present
    ///If it is, return the Passfield.password property
    ///If not, take domain and turn it into a Passfield with pwgen
    ///and append it to the file
    }

Passfield pwgen(char *domain) {

    Passfield newField;
    srand(time(NULL));

    int i;
    int position = 0;
    char password[PASSLEN+1];

    while(position < PASSLEN)
    {
            //valid ANSI numbers: 48 - 57, 65 - 90 , 97 - 122
            //                      0 - 9   A - Z     a - z
        i = rand() % 123;
        if((i >= 48 && i <= 57 ) ||
           (i >= 65 && i <= 90) ||
           (i >= 97 && i <= 122))
        {
            password[position] = (char)i;
            //printf("%d", i);
            position++;
        }
    }

    password[PASSLEN] = '\0';

    strcpy(newField.domain, domain);
    strcpy(newField.password, password);

    return newField;
}

char* getKey(char *password)
{
  int ch;
  char pword[BUFSIZ];
  int i = 0;

  puts ("Enter your password");
  fflush(stdout);

  while ((ch = getchar()) != EOF
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
