#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int key;
    char title[50];
    char author[30];
    int quantity;
} Book;

typedef struct node {
    struct node *left;
    struct node *right;
    Book *bk;
} node;


typedef enum {
    NONE,
    LEFT,
    RIGHT,
    BOTH
} numkids;


node* init();
node* find(node *, int);
void insert(node *, Book *);
node *del(node *, node *);
int kids(node *);
node* parent(node *, node *);
node* max(node *); 
void printTree(node *, FILE *);
void printSoldOutTree(node *, FILE *);

node* init() {
//Initialize a new binary search tree that can store Books
    node *newtree = malloc(sizeof(node));
    newtree->left = NULL;
    newtree->right = NULL;
    newtree->bk = NULL;

    return newtree;
}

node* find(node *current, int val) {
//find out if a book lives in the node given by current (or any of its
//subtrees)
//Or return NULL if the value does not exist in the tree
    if(current != NULL) {
        if(current->bk != NULL) {

            if(current->bk->key == val)
                return current;
            else if(val > current->bk->key)
                return find(current->right, val);
            else
                return find(current->left, val);
        }
    }

        else
            return NULL;
    
}

void insert(node *root, Book *book) {
//Insert book into tree rooted at root by generating a new node to house it in

    if(root->bk == NULL) {
        //root is empty: Give it this book and return
        root->bk = book;
        return;
    }

    else {
        if((book->key > root->bk->key) && root->right == NULL) {
            //The book's key is greater than the key of the book contained at
            //this node and no right child exists, so make one
            root->right = (node *)malloc(sizeof(node));
            root->right->left = NULL;
            root->right->right = NULL;
            root->right->bk = book;
            return;
        }
        else if(book->key > root->bk->key && !(root->right == NULL)) {
            //Book is greater than this node, and a right child already exists:
            //Recursively call insert() on the right node
            insert(root->right, book);
        }



        else if(book->key < root->bk->key && root->left == NULL) {
            //The book's key is less than the key of the book contained at this
            //node and no left child exists, so make one
            root->left = (node *)malloc(sizeof(node));
            root->left->left = NULL;
            root->left->right = NULL;
            root->left->bk = book;
            return;
        }
        else if(book->key < root->bk->key && !(root->left == NULL)) {
            //Book is less than this node, and a left child already exists:
            //Recursively call inser() on the left node
            insert(root->left, book);
        }
    }
}

node* del(node *root, node *remove) {
//Delete the specified node from root, and return a pointer to the node at the
//root of the resulting root
//
//As it turns out this function is not exactly needed but I thought I'd
//implement it anyway

    if(remove == NULL)
        return;

    int k = kids(remove);
    node *par = parent(root, remove);

    //get the max in the left subtree in case we need it later
    node *maxInLeft = max(remove->left);

    if(par == NULL) {
        //Dealing with root node
        //Make exceptions to operations performed on parents

        switch(k) {
            case NONE:
            //Root has no kids and is to be deleted
            //This root will die sad and alone
               free(remove);
                return NULL;
            
            case LEFT:
            //Free the root by returning its only remaining survivor, its left
            //subtree
                free(remove);
                return remove->left;

            case RIGHT:
            //Free root and return the right subtree
                free(remove);
                return remove->right;

            case BOTH:
                break;
                //Do nothing! This case will take care of itself later on

        }
    }

    switch(k) {
        
        case NONE:
            //remove has no kids, so find its parent and erase it with ease
            if(par->left == (node *)remove)
                par->left = NULL;
            else
                par->right = NULL;

            free(remove);
            break;

        case LEFT:
            //remove has only a left child, so to erase remove, set remove's par's
            //appropriate direction pointer to remove's left child
            if(par->left == (node *)remove)
                par->left = remove->left;
            else
                par->right = remove->left;
            free(remove);
            break;

        case RIGHT:
            //remove has only a right child, so mirror the above scenario
            if(par->left == (node *)remove)
                par->left = remove->right;
            else
                par->right = remove->right;
            free(remove);
            break;

        case BOTH:
            //remove has both kids, so replace the Book in remove with the Book with
            //the highest key in remove's left subtree, then free that leaf rather
            //than removing remove explicitly
            remove->bk = maxInLeft->bk;
            del(root, maxInLeft);
            break;

    }

    //If we got here, then we did not have to remove the root of the tree and
    //can safely return the original root as the new root of the resulting tree
    return root;
}

int kids(node *parent) {
//Find out how many kids this parent has

    if(parent->left == NULL && parent->right == NULL)
        return NONE;
    else if(parent->left != NULL)
        return LEFT;
    else if(parent->right != NULL)
        return RIGHT;
    else
        return BOTH;

}

node* parent(node *current, node *kid) {
//Returns the parent of node *kid in the tree rooted at *current


    if(current == kid) {
        //base-case: must be root, we have no parent
        return NULL;
    }

    if(kid->bk->key > current->bk->key) {
        //find parents to the right of this node, since kid's book val is
        //greater than current's book val

        if(current->right == kid)
            return current;
        else
            return parent(current->right, kid);

    }
    else if(kid->bk->key < current->bk->key) {
        //find parents to the left of this node, since kid's book val is
        //less than current's book val

        if(current->left == kid)
            return current;
        else
            return parent(current->left, kid);

    }

}

node* max(node *root) {
//Find the max, i.e. rightmost node in a subtree

    node *current = root;

    while(current->right != NULL)
        current = current->right;
    

    return current;
}

void printTree(node *current, FILE *fp) {
//Print the tree using inorder traversal

    if(current->left != NULL)
        printTree(current->left, fp);

    if(current->bk->quantity > 0)
        fprintf(fp, "Element #%d. %s %d\n", current->bk->key,
                current->bk->title,
                current->bk->quantity);

    if(current->right != NULL)
        printTree(current->right, fp);

}

void printSoldOutTree(node *current, FILE *fp) {
//Print the sold out elements of the tree using inorder traversal 

    if(current->left != NULL)
        printSoldOutTree(current->left, fp);

    if(current->bk->quantity == 0)
        fprintf(fp, "The book %s by %s sold out.\n", current->bk->title,
                current->bk->author);

    if(current->right != NULL)
        printSoldOutTree(current->right, fp);

}
