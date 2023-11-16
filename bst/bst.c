#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // ssize_t


/* - API - */

typedef int T;

typedef struct _bnode bnode_t;
struct _bnode {
    T value;
    bnode_t* left;
    bnode_t* right;
};

typedef struct _bst {
    bnode_t* root;
    size_t length;
} bst_t;

typedef bnode_t** bsst_t;

static bnode_t* bnode_create(T value);
static ssize_t bst_insertleaf(bsst_t subtree, T value);
static ssize_t bst_insertsubtree(bsst_t subtree, T value);
static bnode_t* bst_maxsubtree(bsst_t subtree);
static ssize_t bst_delsubtree(bsst_t subtree, T value);

ssize_t bst_insert(bst_t* tree, T value);
ssize_t bst_del(bst_t* tree, T value);


/* - Implementation - */


/*
    Creates a initialized BinaryNode on the heap

    @value: the value that eventually resides in the node

    Returns: ptr to the allocated node on the heap
*/
static bnode_t* bnode_create(T value)
{
    bnode_t* bn = malloc(sizeof(*bn));
    if (bn) {
        bn->value = value;
        bn->left = NULL;
        bn->right = NULL;
    }
    return bn;
}

/*
    Inserts a node into an empty (leaf) subtree

    @subtree: pointer to the subtree (pointer to a pointer to a node)
    @value: value of the node to possibly be inserted

    Returns: 1 on success, 0 on failure
*/
static ssize_t bst_insertleaf(bsst_t subtree, T value)
{
    assert(*subtree == NULL);
    *subtree = bnode_create(value);
    return 1;
}

/*
    Inserts a node to one of the left or right subtrees (recursively).
    It recurses until it reaches a subtree that is NULL (a leaf).

    @subtree: pointer to the subtree (pointer to a pointer to a node)
        - note that we have to use double pointer, otherwise the address of the
        node the subtree points to will be passed. However, we need the address
        to the subtree node itself.
    @value: value of the node to possibly be inserted

    Returns: 1 on success, 0 on failure
*/
static ssize_t bst_insertsubtree(bsst_t subtree, T value)
{
    if (*subtree == NULL) {
        return bst_insertleaf(subtree, value);
    }

    if (value == (*subtree)->value) {
        return 0L;
    }

    if (value <= (*subtree)->value) {
        return bst_insertsubtree(&((*subtree)->left), value);
    } else {
        return bst_insertsubtree(&((*subtree)->right), value);
    }
}

/*
    Insert a node with a specific value into the tree

    @tree: the tree into which we insert a new node
    @value: value of the node to be inserted

    Returns: 1 on success, 0 on failure
*/
ssize_t bst_insert(bst_t* tree, T value)
{
    // No node in the tree
    if (!tree->root) {
        tree->root = bnode_create(value);
        tree->length++;
        return 1L;
    }

    if (value == tree->root->value) {
        return 0L;
    }

    ssize_t ok = 0L;
    if (value <= tree->root->value) {
        ok = bst_insertsubtree(&(tree->root->left), value);
    } else {
        ok = bst_insertsubtree(&(tree->root->right), value);
    }

    if (ok) {
        tree->length++;
    }

    return ok;
}

/*
    Finds the maximum node in the given subtree

    @subtree: the subtree node to which the max node reside (which is the right
    most node)

    Returns: ptr to the max node
*/
static bnode_t* bst_maxsubtree(bsst_t subtree)
{
    assert(subtree != NULL && "NULL subtree has been passed.");

    bsst_t tmp = subtree;
    while (*tmp != NULL && (*tmp)->right != NULL) {
        tmp = &((*tmp)->right);
    }

    return *tmp;
}

/*
    Removes a node in a subtree that has a certain value

    @subtree: the subtree node in which the value to be removed may reside
    @value: the node with this value will be removed in the subtree (if exists)

    Returns: 1 on success, 0 on failure
*/
static ssize_t bst_delsubtree(bsst_t subtree, T value)
{
    assert(subtree != NULL && "NULL subtree passed.");

    // whoops didn't found it
    if (*subtree == NULL) {
        return 0L;
    }

    /// TODO probably more checks

    if (value > (*subtree)->value) {
        return bst_delsubtree(&((*subtree)->right), value);
    } else if (value < (*subtree)->value) {
        return bst_delsubtree(&((*subtree)->left), value);
    } else {
        // If you found it on a leaf
        if ((*subtree)->left == NULL && (*subtree)->right == NULL) {
            free(*subtree);
            *subtree = NULL;
            return 1L;
        }

        // If left node is empty, then steal right branch' root (move)
        if ((*subtree)->left == NULL) {
            bnode_t* rst = (*subtree)->right;
            (*subtree)->value = rst->value;
            (*subtree)->left = rst->left;
            (*subtree)->right = rst->right;
            free(rst);
            return 1L;
        }

        /// TODO eliminate code duplication -> move function()

        // If right node is empty, then steal left branch's root (move)
        if ((*subtree)->right == NULL) {
            bnode_t* lst = (*subtree)->left;
            (*subtree)->value = lst->value;
            (*subtree)->left = lst->left;
            (*subtree)->right = lst->right;
            free(lst);
            return 1L;
        }

        // Replace to-be-deleted with the max node in left subtree
        bnode_t* max_left = bst_maxsubtree(&((*subtree)->left));
        (*subtree)->value = max_left->value;
        (void)bst_delsubtree(&((*subtree)->left), max_left->value);

        return 1L;
    }
}


/*
    Removes a node in a tree

    @tree: the address of the tree
    @value: the value of the node to be removed

    Returns: 1 on success, 0 on failure
*/
ssize_t bst_del(bst_t* tree, T value)
{
    // No node in the tree
    if (!tree->root) {
        return 0L;
    }

    ssize_t ok = bst_delsubtree(&(tree->root), value);

    if (ok) {
        tree->length--;
    }

    return ok;
}

/* - Test - */


int main(void)
{
    bst_t bst = { 0 };

    /* Insertion */
    bst_insert(&bst, 20);
        bst_insert(&bst, 10);
            bst_insert(&bst, 5);
                bst_insert(&bst, 2);
                bst_insert(&bst, 7);
            bst_insert(&bst, 15);
                bst_insert(&bst, 13);
                bst_insert(&bst, 17);
        bst_insert(&bst, 30);
            bst_insert(&bst, 25);
                bst_insert(&bst, 23);
                bst_insert(&bst, 28);
            bst_insert(&bst, 35);
                bst_insert(&bst, 33);
                bst_insert(&bst, 38);


    /* Test printf */
    printf("After insertion:\n");
    printf("bst.root->value: %d\n", bst.root->value);
    printf("bst.root->left->value: %d\n", bst.root->left->value);
    printf("bst.root->right->value: %d\n", bst.root->right->value);
    printf("size: %zu\n", bst.length);

    /* Deletion */
    bst_del(&bst, 20);
        bst_del(&bst, 10);
            bst_del(&bst, 5);
                bst_del(&bst, 2);
                bst_del(&bst, 7);
            bst_del(&bst, 15);
                bst_del(&bst, 13);
                bst_del(&bst, 17);
        bst_del(&bst, 30);
            bst_del(&bst, 25);
                bst_del(&bst, 23);
                bst_del(&bst, 28);
            bst_del(&bst, 35);
                bst_del(&bst, 33);
                bst_del(&bst, 38);


    // printf("%d\n", bst.root->value);
    printf("\nAfter deletion:\n");
    printf("size: %zu\n", bst.length);

    return 0;
}


// vim: fdm=indent fdn=1
