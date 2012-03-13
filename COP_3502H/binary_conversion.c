// Stephen Fulwider
// 1 March 2012
// Recursive and Iterative functions to convert a decimal integer to binary and print the resulting binary number.

#include <stdio.h>

// A recursive function that takes in a decimal number and prints out that decimal number in binary.
// Takes in an additional parameter isFirstCall to see if this is the first time this function has been
// called, to handle printing 0 if this is the number passed to the function.
void convertToBinaryRec(int decimalNumber, int isFirstCall) {

  // Base case, nothing left to do (except print 0 if this is the first call to the function).
  if (decimalNumber == 0) {
    if (isFirstCall) {
      printf("0");
    }
    return;
  }

  // Computer the remainder and result of division.
  int remainder = decimalNumber % 2;
  int division = decimalNumber / 2;

  // Recursively solve the smaller version of the problem on the result of division. Notice we now pass 0
  // as the isFirstCall parameter, since this call is definitely not the first.
  convertToBinaryRec(division,0);

  // Print the remainder (notice this is after the recursive call, since in the process we read off
  // the remainders in the reverse order that we computed them--this simulates that).
  printf("%d",remainder);

}

// Convert a decimal integer to binary via an interative process.
void convertToBinaryIter(int decimalNumber) {

  // An array to store the remainders (need to store them since we compute them in reverse). The most digits we
  // should need to store a number will be 31 (assuming we are dealing with non-negative ints), so make the array
  // just a little larger to be on the safe side.
  int array[35];

  // The current index we are at in the array. This is essentially how many elements we have filled in so far.
  int arrayIndex=0;
  int i;

  // Continue processing as long as the number hasn't reached 0.
  while (decimalNumber != 0) {
    
    // Computer the remainder.
    int remainder = decimalNumber%2;

    // Store that number at the end of the array.
    array[arrayIndex] = remainder;

    // Increase the arrayIndex to tell us that the array has grown by one.
    ++arrayIndex;

    // Make the problem smaller.
    decimalNumber /= 2;
  }

  // Special case if the input number was 0 so "0" gets printed.
  if (arrayIndex == 0) {
    array[0]=0;
    ++arrayIndex;
  }

  // Loop through the array backwards, printing each remainder we previously computed.
  for (i=arrayIndex-1; i>=0; --i)
    printf("%d",array[i]);
  printf("\n");

}


int main() {

  // Read in a number from the user to convert.
  printf("What number do you want to convert? ");
  int n;
  scanf("%d",&n);

  // Convert the number using the recursive function (notice we pass 1 as the parameter, since this is the first call).
  convertToBinaryRec(n,1);
  printf("\n");

  // Convert the number using the iterative function.
  convertToBinaryIter(n);

  return 0;

}
