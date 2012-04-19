#include <stdio.h>


typedef struct {
    int key;
    char title[50];
    char author[30];
    int quantity;
} Book;

typedef struct {
    node *left;
    node *right;
    Book *bk;
} node;


typedef enum _numkids {
    NONE,
    LEFT,
    RIGHT,
    BOTH
} numkids;

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
    if(current != NULL) {
        if(current->bk->key == val)
            return current;
        else if(val > current->bk->key)
            return find(current->right, val);
        else
            return find(current->left, val);
    }

    else
        return NULL;
}

void insert(node *list, Book *book) {
    if(list->bk == NULL) {
        //List is empty: Give it this book and return
        list->bk = book;
        return;
    }

    else {
        if((book->key > list->bk->key) && list->right == NULL) {
            //The book's key is greater than the key of the book contained at
            //this node and no right child exists, so make one
            list->right = (node *)malloc(sizeof(node));
            list->right->left = NULL;
            list->right->right = NULL;
            list->right->bk = book;
            return;
        }
        else if(book->key > list->bk->key) {
            //Book is greater than this node, and a right child already exists:
            //Recursively call insert() on the right node
            insert(list->right, book);
        }

        else if(book->key < list->bk->key && list->left == NULL) {
            //The book's key is less than the key of the book contained at this
            //node and no left child exists, so make one
            list->left = (node *)malloc(sizeof(node));
            list->left->left = NULL;
            list->left->right = NULL;
            list->left->bk = book;
            return;
        }
        else if(book->key < list->bk->key) {
            //Book is less than this node, and a left child already exists:
            //Recursively call inser() on the left node
            insert(list->left, book);
        }
    }
}

node* del(node *list, node *remove) {
//Delete the specified node from list, and return a pointer to the node at the
//root of the resulting list

    if(remove == NULL)
        return;

    char k = kids(remove);
    node *par = parent(list, remove);

    if(par == NULL) {
        //Dealing with root node
        //Make exceptions to operations performed on parents

        switch(k) {
            case NONE:
            //Root has no kids and is to be deleted
            //This list will die sad and alone
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
                //Do nothing! This case will take care of itself later on

        }
    }

    switch(k) {
        
        case NONE:
            //remove has no kids, so find its parent and erase it with ease
            if(par->left == remove)
                par->left = NULL;
            else
                par->right = NULL;

            free(remove);
            break;

        case LEFT:
            //remove has only a left child, so to erase remove, set remove's par's
            //appropriate direction pointer to remove's left child
            if(par->left == remove)
                par->left = remove->left;
            else
                par->right = remove->left;
            free(remove);
            break;

        case RIGHT:
            //remove has only a right child, so mirror the above scenario
            if(par->left == remove)
                par->left = remove->right;
            else
                par->right = remove->right;
            free(remove);
            break;

        case BOTH:
            //remove has both kids, so replace the Book in remove with the Book with
            //the highest key in remove's left subtree, then free that leaf rather
            //than removing remove explicitly
            node *maxval = max(remove->left);
            remove->bk = maxval->bk;
            del(maxval);
            break;

    }

    //If we got here, then we did not have to remove the root of the tree and
    //can safely return the original root as the new root of the resulting tree
    return list;

numkids kids(node *parent) {
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
    if(current == find) {
        //base-case: must be root, we have no parent
        return NULL;
    }

    else {
        if(find->bk->val > current->bk->val) {
            //Find parents to the right of this node, since kid's book val is
            //greater than current's book val

            if(current->right == kid)
                return current;
            else
                return parent(current->right, kid);

        }
        else {
            //Find parents to the left of this node, since kid's book val is
            //less than current's book val

            if(current->left == kid)
                return current;
            else
                return parent(current->left, kid);

        }
    }

}

node* max(node *root) {

    node *current = root;
    int maxBookVal = current->bk->val;

    while(current->right != NULL) {
        current = current->right;
        maxBookVal = current->bk->val;
    }

    return maxBookVal;
}
