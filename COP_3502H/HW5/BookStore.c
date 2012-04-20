#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BinarySearchTree.h"

int main() {

    FILE *booksIn = fopen("books.in", "r");
    FILE *booksOut = fopen("books.out", "w");

    if(booksIn == NULL || booksOut == NULL) {
        printf("could not open files\n");
        return -1;
    }

    int numCases;
    fscanf(booksIn, "%d", &numCases);

    int i;
    for(i = 0; i < numCases; i++) {

        fprintf(booksOut, "Test Case #%d:\n", i+1);
        
        int numOperations;
        fscanf(booksIn, "%d", &numOperations);

        node *head = init();

        int j;
        for(j = 0; j < numOperations; j++) {

            char currentOp[20];
            fscanf(booksIn, "%s", currentOp);

            if(strcmp(currentOp, "POPULATE") == 0) {


                int key;
                fscanf(booksIn, "%d", &key);

                if(find(head, key) == NULL) {
                    //This book does not yet exist in the tree
                    Book *new = (Book *)malloc(sizeof(Book));
                    fscanf(booksIn, "%d", &(new->quantity));
                    fscanf(booksIn, "%s", new->author);
                    fscanf(booksIn, "%s", new->title);
                    new->key = key;

                    insert(head, new);

                } else {

                    int qty;
                    fscanf(booksIn, "%d", &qty);
                    (find(head, key))->bk->quantity += qty;

                    //Get the author and title but don't process them
                    fscanf(booksIn, "%s%s", NULL);

                }

            }

            else if(strcmp(currentOp, "SOLD") == 0) {

                int key;
                fscanf(booksIn, "%d", &key);
                int qty;
                fscanf(booksIn, "%d", &qty);

                node *bestseller = find(head, key);

                bestseller->bk->quantity -= qty;

            }

        }

        printTree(head, booksOut);
        printSoldOutTree(head, booksOut);
        fprintf(booksOut, "\n");

    }
    fclose(booksOut);


    return 0;
}
