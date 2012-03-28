/* newpw.c: generates a new random alphanumeric [A-Za-z0-9] password
 * of however many characters is passed to it as an argument
 * 
 * Written by Matthew Proetsch 2012
 */
 
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	
	if(argc != 2) {
		printf("Usage: %s <pwlength>\n", argv[0]);
	}
		
	else {
		
		int pwlen = atoi(argv[1]);
		srand(time(NULL));
		
		int i = 0;
		char pw[pwlen+1];
		
		while(i < pwlen) {
			
			int ch = (int)rand() % 123;
			
			//ASCII values 48-57: 0-9
			//		values 65-90: A-Z
			//		values 97-122: a-z
			
			if( (ch >= 48 && ch <= 57) ||
				(ch >= 65 && ch <= 90) ||
				(ch >= 97 && ch <= 122) ) {
				
				pw[i] = (char)ch;
				i++;
			}
		}
		
		pw[i] = '\0';
		printf("%s", pw);
		printf("\n");
	}
	
	return 0;
}
			
			
