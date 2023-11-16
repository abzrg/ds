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

ssize_t bst_insert(bst_t* tree, T value);


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


/* - Test - */


int main(void)
{
    bst_t bst = { 0 };
    bst_insert(&bst, 20);
    bst_insert(&bst, 10);
    bst_insert(&bst, 30);

    printf("%d\n", bst.root->value);
    printf("%d\n", bst.root->left->value);
    printf("%d\n", bst.root->right->value);
    return 0;
}


// vim: fdm=indent fdn=1
