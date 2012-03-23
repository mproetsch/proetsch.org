#include <stdio.h>

int partition(int *, int, int);
void quicksort(int *, int, int);
void swap(int *, int *);

int main( void ) {
    FILE *nutsBoltsIn = fopen("NutsBolts.in", "r");
    FILE *nutsBoltsOut = fopen("NutsBolts.out", "w");

    int cases;
    fscanf(nutsBoltsIn, "%d", &cases);

    int i;
    for(i = 0; i < cases; i++) {

        int size;
        fscanf(nutsBoltsIn, "%d", &size);

        int q;
        int nuts[size];
        int bolts[size];

        for(q = 0; q < size; q++) 
            fscanf(nutsBoltsIn, "%d", &nuts[q]);

        for(q = 0; q < size; q++)
            fscanf(nutsBoltsIn, "%d", &bolts[q]);

        fprintf(nutsBoltsOut, "Before:\n");
        fprintf(nutsBoltsOut, "\tNuts: ");
        
        for(q = 0; q < size; q++)
            fprintf(nutsBoltsOut, "%d ", nuts[q]);
        fprintf("\n\tBolts: ");
        for(q = 0; q < size; q++)
            fprintf(nutsBoltsOut, "%d", bolts[q]);

                

        quicksort(
    }
}

int partition(int *vals, int low, int high, int bolt) {

    int temp;
    int i, lowpos;

    if (low == high) return low; //A base case that should never really occur

    //Pick a random partition element and swap it into index low.
    /*i = low + rand() % (high-low+1);*/

    temp = bolt;
    bolt = vals[low];
    vals[low] = temp;

    lowpos = low; //Store the index of the parittion element.

    low++; //Update our low pointer

    while(low <= high) {
        //Move the low pointer until we find a value too large for this side
        while(vals[low] < bolt) low++;
        
        /*Now move until we find a value too large for this side*/
        while(vals[high] > bolt) high--;

        if(low < high) // Swap the two values that were on the wrong side
            swap(&vals[low], &vals[high]);
    }
    swap(&vals[lowPos], &vals[high]); //Swap our actual partition into the right spot

    return high; //This is the index of our partition element
}

void quicksort(int *numbers, int low, int high) {

    //Only have to sort if we are sorting more than one number
    if(low < high) {
        int split = partition(numbers, low, high);

        quicksort(vals, low, split-1, );
        quicksort(vals, split+1, high);
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

