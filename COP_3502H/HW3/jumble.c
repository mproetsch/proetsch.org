/* Matthew Proetsch
 * For COP 3502H
 * jumble.c: Read input jumbled words from jumble.in, 
 * a dictionary from dictionary.in, and compare jumbled input permutations
 * iteratively and recursively to words from the dictionary
 */

#include <stdio.h>

void RecursivePermute(char *, int, char *, char **, int);
int findBinSearch(char **, int, char *);
void exchangeCharacters(char *, int, int);
int fact(int);
void nextPerm(int perm[], int length);

int main (void) {

    FILE *dict_fp = fopen("dictionary.in", "r");
    FILE *jumbles_in = fopen("jumble.in", "r");

    //go ahead and use some external vars here
    //makes looking up words easier from within functions
    int dictSize; 
    fscanf(dict_fp, "%d", &dictSize);
    int numJumbles;
    fscanf(jumbles_in, "%d", &numJumbles);

    /*We want an array of pointers to strings*/
    char *dict[dictSize];
    
    /*Fill up dict with words from dict_fp*/
    int i;
    for(i = 0; i < dictSize; i++) {

        /*Use auxiliary character array to get exact string size*/
        /*Don't want to use up more memory than we have to*/

        char temp[31] = { '\0' };
        fscanf(dict_fp, "%s", temp);

        int len = strlen(temp);
        dict[i] = (char *)malloc(sizeof(char) * len + 1);
        strncpy(dict[i], temp, len);
    }

    int algo;
    fscanf(jumbles_in, "%d", &algo);

    int z = 0;
    for(z; z < numJumbles; z++) {
        
        char current[31];
        fscanf(jumbles_in, "%s", current);

        //This copy will be passed to the recursive algo
        //so that we can print out input cases in the jumbled
        //and the unjumbled form
        //We also use it in the iterative algo as a base for
        //switching various characters, explained shortly
        char currentCopy[(strlen(current) + 1)];
        strcpy(currentCopy, current);

        //Print the case header
        printf("Jumble #%d\n", z+1);

        if(algo == 1) //Recursive
            RecursivePermute(current, 0, currentCopy, dict, dictSize);

        if(algo == 2) { //Iterative

            /* The algorithm by Dr. Guha we were asked to implement was designed for
             * arrays in which all elements are distinct - but we will definitely encounter
             * inputs for which this is not the case! So, I use an integer array {1, 2, ... n}
             * to represent the input case then switch the letters found in the permutation
             * with those in the input string*/

            int stringRepr[strlen(current)];
            int r;
            for(r = 0; r < strlen(current); r++) {
                stringRepr[r] = r+1;
            }

            //Now permute it and check each against the dictionary
            for(r = 0; r < fact(strlen(current)); r++) {

                //We will reposition each character in string "current" based
                //on the values of the integer array stringRepr
                //The values of currentCopy are never modified and so we use
                //them to reposition the characters in current

                int q;
                for(q = 0; q < strlen(current); q++) {
                    int replace = stringRepr[q] - 1;
                    char chReplace = currentCopy[replace];
                    current[q] = chReplace;
                }

                if(findBinSearch(dict, dictSize, current) >= 0) {
                    printf("A permutation of %s that is a valid "
                            "word is %s.\n", currentCopy, current);
                }

                nextPerm(stringRepr, strlen(current));
            }
        }
        printf("\n");
    }

    //Free up all our memory and close our file descriptors
    fclose(dict_fp);
    fclose(jumbles_in);

    for(i = 0; i < dictSize; i++) {
        free(dict[i]);
    }

    return 0;
}

//this function will return the position in **dictionary that *word occupies
//or -1 if it is unable to find *word in **dictionary
findBinSearch(char **dictionary, int size, char *word) {

    int low = 0, high = size - 1;

    while(low <= high) {

        int mid = (low + high) / 2;
        int cmp = strcmp(word, dictionary[mid]);

        if(cmp > 0 ) {
            //word comes after dict[mid]
            low = mid + 1;
        }
        else if(cmp < 0) {
            //word comes before dict[mid]
            high = mid - 1;
        }
        else {
            return mid;
        }
    }

    return -1;
}

//Found this function in my notes from class
//RecursivePermute(): takes in a string and permutes it into
//(length)! sets, each of size strlen(str)
void RecursivePermute(char *str, int k, char *orig, char **dict, int dictSize) {

    //Check to see if the permutation we wound up with
    //can be found in "dict" by calling findBinSearch

    if(k == strlen(str)) {
        if(findBinSearch(dict, dictSize, str) >= 0) {
            printf("A permutation of %s that is a valid"
                    " word is %s.\n", orig, str);
        }
    }
    else {

        int j;
        for(j = k; j < strlen(str); j++) {

            ExchangeCharacters(str, k, j);
            RecursivePermute(str, k+1, orig, dict, dictSize);
            ExchangeCharacters(str, j, k);

        }
    }
}

/*ExchangeCharacters(): Take in a string and two indices,*/
/*and switch the characters at those indices*/
void ExchangeCharacters(char *str, int i, int j) {

    char temp = str[i];
    str[i] = str[j];
    str[j] = temp;

}

int fact(int num) {

    if(num == 0) return 1;
    if(num == 1) return 1;

    return num*fact(num-1);
}

//nextPerm(): Written by Arup Guha
//For an input array, find the first position, starting
//from the end, that isn't in descending order
//Then swap that with the position that comes before i
void nextPerm(int perm[], int length) {

    //Find the spot that needs to change
    int i = length - 1;
    while(i > 0 && perm[i] < perm[i-1])
        //Advance the the index that needs to be swapped
        i--;
    i--;

    //Handle the last perm
    if(i == -1)
        return;

    //Find the spot we need to swap i with
    //Start at the back
    int j = length - 1;
    while(j > i && perm[j] < perm[i])
        //Find the first position that is larger than that at i
        j--;

    //Perform the swap
    int temp = perm[i];
    perm[i] = perm[j];
    perm[j] = temp;

    //Reverse the array starting at i+1 to the end
    int k, m;
    for(k = i+1, m = length-1; k < m; k++, m--) {
        temp = perm[k];
        perm[k] = perm[m];
        perm[m] = temp;
    }
}
