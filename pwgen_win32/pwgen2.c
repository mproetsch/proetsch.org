#include <math.h>
#include <stdio.h>
#include <windows.h>
#include "pwInput.c"
#include "keypressinit.c"

//These records will be stored in keychain.kcn
typedef struct {
    char domain[20];
    char password[13];
    } Record;

void pwopen(Record);
Record pwgen(char*);

void consumeWhitespace( void ) {
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    }

void deleteFile(char *filename);

int main()
{
    char key[30];

    getKey(key);

    char decryptStr[120] = "aescrypt.exe -d -p ";
    strcat(decryptStr, key);
    strcat(decryptStr, " -o keychain.kcn keychain.aes");
    system(decryptStr);
    system("CLS");

    Sleep(300); //Give the system time to decrypt

    FILE *fin = fopen("keychain.kcn", "r");
    if (fin == NULL)
    {
        system("CLS");
        perror ("WELL THAT SIMPLY WON'T DO\n<keychain.kcn> Open file failed");
        Sleep(1000);
        return -1;
    }
else{
    printf("domain (TLD, lowercase) or e-mail address: "); //prompt the user for a domain name (or email address)
    char userDomain[20];
    scanf("%s", userDomain);
    consumeWhitespace();
    /*********************************
    *********************************/

    //Search through keychain.kcn to ensure that domain hasn't already been entered
    Record tempRecord;

    system("CLS");
    while(fscanf(fin, "%s %s",
          tempRecord.domain,
          tempRecord.password) != EOF )
    {
        printf("Read %s, %s into buffer\n", tempRecord.domain, tempRecord.password);
        if(strcmp(userDomain, (tempRecord.domain)) == 0)
        {
            system("CLS");
            printf("Found a match\n");
            fclose(fin);

            pwopen(tempRecord);

            char encryptStr[120] = "aescrypt.exe -e -p ";
            strcat(encryptStr, key);
            strcat(encryptStr, " -o keychain.aes keychain.kcn");
            system(encryptStr);
            deleteFile("keychain.kcn");

            KeyboardPaste();

            return 0;
        }
        else
        {
            printf("%s not found this pass through keychain.kcn\n", userDomain);
            continue;
        }
    }

    system("CLS");

    printf("Making a new record\n");
    Record newRecord = pwgen(userDomain);

    FILE *fout = fopen("password_masterTemp.txt", "w");

    fclose(fin);
    fin = fopen("keychain.kcn", "r");

    //********
    //Copy data from password_master into password_masterTemp
    //********
    while(fscanf(fin, "%s %s", tempRecord.domain, tempRecord.password) != EOF)
    {
        printf("Read %s, %s into buffer\n", tempRecord.domain, tempRecord.password);
        fprintf(fout, "%s %s\n", tempRecord.domain, tempRecord.password);
        fgetc(fin);
    }

    //Add our newest record
    fprintf(fout, "%s %s\n", newRecord.domain, newRecord.password);

    fclose(fin);
    fclose(fout);

    //Clean up by deleting old keychain.kcn and renaming
    //our "temp" file to become the new password_master
    //(which we will encrypt at the end)
    deleteFile("keychain.kcn");
    system("COPY password_masterTemp.txt keychain.kcn");
    deleteFile("password_masterTemp.txt");


    /*************
    fin = fopen("password_masterTemp.txt", "r");
    fout = fopen("keychain.kcn", "w");

    Record finalRecord;
    while(fscanf(fin2, "%s %s", finalRecord.domain, finalRecord.password) != EOF)
    {
        //printf("Copying %s %s from password_masterTemp.txt to keychain.kcn\n", finalRecord.domain, finalRecord.password);
        fprintf(fout2, "%s %s\n", finalRecord.domain, finalRecord.password);
    }
    *****************/

    pwopen(newRecord);

    char encryptStr[120] = "aescrypt.exe -e -p ";
    strcat(encryptStr, key);
    strcat(encryptStr, " -o keychain.aes keychain.kcn");
    system(encryptStr);
    deleteFile("keychain.kcn");

    KeyboardPaste();

    return 0;
}
} //because of that "Else" statement up top

void pwopen(Record existingRecord)
{

    //Make a new string containing the domain (or account name)
    //for easy usage with strcpy()
    char filename[30];
    strcpy(filename, existingRecord.domain);
    strcat(filename, ".txt");

    //Write a new file named domain and write to it
    //the corresponding password
    FILE *fWrite = fopen(filename, "w");
    fprintf(fWrite, "%s", existingRecord.password);
    printf("File successfully written\n");
    fclose(fWrite);

    //use clipboard.exe <name of domain> to get the new password
    //and copy it to clipboard
    char commandStr[30];
    strcpy(commandStr, "clipboard.exe ");
    strcat(commandStr, filename);
    system(commandStr);

    deleteFile(filename);
}


Record pwgen(char *domain)
{
    Record newRecord;
    srand(time(NULL));

    //initialize all of our interating vars
    //for maxiumum random
    int i;
    int position = 0;
    char password[13];

    while(position < 12)
    {
            //valid ANSI numbers: 48 - 57, 65 - 90 , 97 - 122
            //                      0 - 9   A - Z     a - z
        i = rand() % 123;
        if((i >= 48 && i <= 57 ) ||
           (i >= 65 && i <= 90) ||
           (i >= 97 && i <= 122)    )
        {
            password[position] = (char)i;
            //printf("%d", i);
            position++;
        }
    }

    password[12] = '\0';

    strcpy(newRecord.domain, domain);
    strcpy(newRecord.password, password);

    return newRecord;
}

void deleteFile(char *filename)
{
    char deleteCmd[30] = "DEL ";
    strcat(deleteCmd, filename);

    system(deleteCmd);
}
