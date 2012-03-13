// Arup Guha
// 1/15/09
// Solution to 2009 Spring Recitation Assignment #1:
// Finding whether or not there exists a number in one array, when added to
// some number from a second array equals a designated target value.

/* Edited on 2/23/2010 as an exercise to show how to time an algorithm and
   make predictions about run-times on different input sizes
*/
   
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NOT_FOUND -1

#define SIZE 400000

int arraySumBF(int vals1[], int len1, int vals2[], int len2, int target);
int arraySumBinSearch(int vals1[], int len1, int vals2[], int len2, int target);
int BinarySearch(int A[], int n, int TARGET);
int arraySumLinear(int vals1[], int len1, int vals2[], int len2, int target);
int* makeRandSortArray(int size);

int main() {
    
    srand(time(0));
    
    // A couple testing arrays.
    int* arr1 = makeRandSortArray(SIZE);
    int* arr2 = makeRandSortArray(SIZE);
    
    // Set to whatever value you want to search for (as the sum of one
    // element in each array.
    int total = 3;
    
    /* Note: In order to accurately time the other two, you need to run them
             many times (say 100) and divide the total time by how many times
             you ran them.
    */
    
    int start1 = time(0);
    int ansbf = arraySumBF(arr1,SIZE,arr2,SIZE,total);
    int start2 = time(0);
	int i = 0;
	int ansbs;
	for (i = 0; i < 1000; i++){    
		ansbs = arraySumBinSearch(arr1,SIZE,arr2,SIZE,total);  // Since this is so fast we had to run it 1000 times for the time to show up
	}
    int start3 = time(0);
	int anslin;
	for (i = 0; i < 1000; i++){    
		anslin = arraySumLinear(arr1,SIZE,arr2,SIZE,total);   // Since this is so fast we had to run it 1000 times for the time to show up
	}
    
    int end = time(0);
        
    printf("Brute force took %d seconds.\n", start2 - start1);
    printf("Bin search took %d seconds.\n", start3 - start2); /	/// NOTE - You must divide this by 1000
    printf("Linear took %d seconds.\n", end - start3);			 // NOTE - You must divide this by 1000

    free(arr1);
    free(arr2);
    
    system("PAUSE");
    return 0;
}

int* makeRandSortArray(int size) {

    // Allocate space and fill in the first slot.
    int* array = (int*)malloc(sizeof(int)*size);
    array[0] = rand()%100;
    
    // Fill in each number to be the previous number plus a random offset.
    int i;
    for (i=1; i<size; i++)
        array[i] = array[i-1] + rand()%100;     
     
    return array; 
}

// Pre-condition: vals1 is sorted in non-descending order
//                and is of length len1, vals2 is sorted
//                similarly and is of length len2.
// Post-condition: 1 is returned if there exists a value
//                 in vals1 when added to some value in
//                 vals2 equals target. If no pair of
//                 values exist, 0 is returned.
int arraySumBF(int vals1[], int len1, int vals2[], int len2, int target) {
    
    int i,j;
    
    // Loop through all len1*len2 pairs of numbers (x,y) where x is from
    // the first list and y is from the second and see if x+y is equal
    // to the target!
    for (i=0; i<len1; i++)
        for (j=0; j<len2; j++)
            if (vals1[i]+vals2[j] == target)
                return 1;
       
    // None of our options worked so there's no such ordered pair.         
    return 0;
}

// Pre-condition: vals1 is sorted in non-descending order
//                and is of length len1, vals2 is sorted
//                similarly and is of length len2.
// Post-condition: 1 is returned if there exists a value
//                 in vals1 when added to some value in
//                 vals2 equals target. If no pair of
//                 values exist, 0 is returned.
int arraySumBinSearch(int vals1[], int len1, int vals2[], int len2, int target) {
    
    int i;
    
    // Go through each value in vals1.
    for (i=0; i<len1; i++)
    
        // The number that "matches" vals1[i] is target-vals1[i], so
        // look for it!
        if (BinarySearch(vals2, len2, target-vals1[i]))
            return 1;
                
    return 0;
}

// Pre-condition: A is an array sorted in ascending order of size greater than
//                or equal to n.
// Post-condition: If TARGET is stored in A in an index in between 0 and n-1,
//                 then 1 is returned. Otherwise, 0 is returned.
int BinarySearch(int A[], int n, int TARGET) {
 
    // Set the left and rightmost boundaries in which to search.   
    int L = 0, Mid, R = n-1;
    
    // Keep on looking so long as there is a search space.
    while (L <= R) {
    
        // Find the element in the middle of the search space.
        Mid = (L+R)/2;
            
        // We got a direct hit.
        if (TARGET == A[Mid]) 
            return 1;
        
        // We know that if TARGET is in A, then it will be located
        // to the right of A[Mid].
        else if (TARGET > A[Mid]) 
            L = Mid + 1;
        
        // We know that if TARGET is in A, then it will be located
        // to the left of A[Mid].
        else 
            R = Mid - 1;
    }
    
    return 0;
}

// Pre-condition: vals1 is sorted in non-descending order
//                and is of length len1, vals2 is sorted
//                similarly and is of length len2.
// Post-condition: 1 is returned if there exists a value
//                 in vals1 when added to some value in
//                 vals2 equals target. If no pair of
//                 values exist, 0 is returned.
int arraySumLinear(int vals1[], int len1, int vals2[], int len2, int target) {
    
    int vals1index = 0;
    int vals2index = len2-1;
    
    while (vals1index < len1 && vals2index >= 0) {
    
        // Sum of the two current values.
        int curSum = vals1[vals1index] + vals2[vals2index];
        
        if (curSum == target)
            return 1;      
            
        // Sum is too small, we must try a larger number from the first list.
        else if (curSum < target)
            vals1index++;
            
        // Sum is too big, we must try a smaller number from the second list.
        else
            vals2index--;
    }   
    
    // If either array index goes all the way to the end of the array, then
    // there's no way a pair exists that adds up to the target.
    return 0;
}
