#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // ssize_t

#define TEST_MIN_MAX_HEIGHT

/* - API - */

/// @brief type of the data (integer types or floats).
typedef int T;

/// @brief binary node containing a value and pointer to left and right subtrees.
typedef struct _bnode bnode_t;
struct _bnode {
    T value;
    bnode_t* left;
    bnode_t* right;
};

/// @brief binary search tree containing a length and a pointer to root.
typedef struct _bst {
    bnode_t* root;
    size_t length;
} bst_t;

/// @brief pointer to a node as a subtree (binary-search-sub-tree-type).
/// @note we have to use double pointer to be able to not only change the underlying subtree but the pointer (member of parent) itself.
/// @note in the recursive functions, a pointer to a subtree (bsst_t*) will be passed.
typedef bnode_t* bsst_t;

/// @brief create a new zero initialized node (mallocs one node).
/// @param value value of the node
/// @return pointer to node
static bnode_t* bnode_create(T value);

/// @brief insert a node into a leaf node/subtree.
/// @param subtree pointer to subtree
/// @param value value of the node
/// @return 1 on success, 0 on failure
static ssize_t bst_insertleaf(bsst_t* subtree, T value);

/// @brief insert a node into a subtree (recursive).
/// @param subtree pointer to subtree
/// @param value value of the node
/// @return 1 on success, 0 on failure
static ssize_t bst_insertsubtree(bsst_t* subtree, T value);

/// @brief find maximum node in a subtree.
/// @param subtree address of a subtree
/// @return pointer to the maximum node
/// @note the passed subtree pointer should not be NULL
static bnode_t* bst_maxsubtree(bsst_t* subtree);

/// @brief remove a node from a subtree (recursive).
/// @param subtree address of subtree
/// @param value value of node
/// @return 1 on success, 0 on failure
static ssize_t bst_delsubtree(bsst_t* subtree, T value);

/// @brief find the node with the value `value` in the subtree.
/// @param subtree address of subtree
/// @param value query value
/// @return pointer to node or NULL if not found
static bnode_t* bst_findsubtree(bsst_t* subtree, T value);

/// @brief find height of subtree.
/// @param subtree address of subtree
/// @return height of subtree, 0 if *subtree is NULL
static ssize_t bst_findsubtreeheight(bsst_t* subtree);

/// @brief insert a node into tree.
/// @param tree address of tree
/// @param value value of node
/// @return 1 on success, 0 on failure
ssize_t bst_insert(bst_t* tree, T value);

/// @brief remove a node from tree.
/// @param tree address of tree
/// @param value value of node
/// @return 1 on success, 0 on failure
ssize_t bst_del(bst_t* tree, T value);

/// @brief find a node with the value of `value`.
/// @param tree address of tree
/// @param value query value
/// @return pointer to node or NULL if not found
bnode_t* bst_find(bst_t* tree, T value);

/// @brief find minimum node in tree.
/// @param tree address of tree
/// @return pointer to minimum node, NULL if tree is empty
bnode_t* bst_findmin(bst_t* tree);

/// @brief find maximum node in tree
/// @param tree address of tree
/// @return pointer to maximum node, NULL if tree is empty
bnode_t* bst_findmax(bst_t* tree);

/// @brief find the height of tree.
/// @param tree addreess of tree
/// @return height of tree, -1 if tree is empty
ssize_t bst_findheight(bst_t* tree);


/* - Implementation - */


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

static ssize_t bst_insertleaf(bsst_t* subtree, T value)
{
    assert(*subtree == NULL);
    *subtree = bnode_create(value);
    return 1;
}

static ssize_t bst_insertsubtree(bsst_t* subtree, T value)
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

static bnode_t* bst_maxsubtree(bsst_t* subtree)
{
    assert(subtree != NULL && "NULL subtree has been passed.");

    bsst_t* tmp = subtree;
    while (*tmp != NULL && (*tmp)->right != NULL) {
        tmp = &((*tmp)->right);
    }

    return *tmp;
}

static ssize_t bst_delsubtree(bsst_t* subtree, T value)
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

static bnode_t* bst_findsubtree(bsst_t* subtree, T value)
{
    // Did not found it
    if (*subtree == NULL) {
        return NULL;
    }

    if (value > (*subtree)->value) {
        return bst_findsubtree(&((*subtree)->right), value);
    }

    if (value < (*subtree)->value) {
        return bst_findsubtree(&((*subtree)->left), value);
    }

    // Found it
    return *subtree;
}

bnode_t* bst_find(bst_t* tree, T value)
{
    if (tree->root == NULL) {
        return NULL;
    }

    if (value > tree->root->value) {
        return bst_findsubtree(&(tree->root->right), value);
    }

    if (value < tree->root->value) {
        return bst_findsubtree(&(tree->root->left), value);
    }

    return tree->root;
}

bnode_t* bst_findmin(bst_t* tree)
{
    bnode_t* root = tree->root;

    while (root) {
        if (root->left == NULL) {
            return root;
        }

        root = root->left;
    }

    return root;
}

bnode_t* bst_findmax(bst_t* tree)
{
    bnode_t* root = tree->root;

    while (root) {
        if (root->right == NULL) {
            return root;
        }

        root = root->right;
    }

    return root;
}

static ssize_t bst_findsubtreeheight(bsst_t* subtree)
{
    if (*subtree == NULL) {
        return 0;
    }

    ssize_t left_height = 1;
    ssize_t right_height = 1;

    left_height += bst_findsubtreeheight(&(*subtree)->left);
    right_height += bst_findsubtreeheight(&(*subtree)->right);

    return (left_height > right_height) ? left_height : right_height;
}

ssize_t bst_findheight(bst_t* tree)
{
    bnode_t* root = tree->root;

    if (root == NULL) {
        return -1;
    }

    ssize_t left_height = 1;
    ssize_t right_height = 1;

    left_height += bst_findsubtreeheight(&root->left);
    right_height += bst_findsubtreeheight(&root->right);

    return (left_height > right_height) ? left_height : right_height;
}


/* - Test - */


int main(void)
{
    bst_t bst = { 0 };


#ifdef TEST_MIN_MAX_HEIGHT
    printf("\n<------------------------------------------------------\n");
    printf("[ TEST ]: min, max, height (before insertion)\n\n");

    printf("\n[height]: %zd\n", bst_findheight(&bst));

    bnode_t* minnode = NULL;
    bnode_t* maxnode = NULL;

    printf("[min]: ");
    if ((minnode = bst_findmin(&bst))) {
        printf("%d\n", minnode->value);
    } else {
        printf("tree is empty\n");
    }

    printf("[max]: ");
    if ((maxnode = bst_findmax(&bst))) {
        printf("%d\n", maxnode->value);
    } else {
        printf("tree is empty\n");
    }

    printf(">------------------------------------------------------\n\n");
#endif

    /* Insertion */
#define VALUES_SIZE 15
    T values[VALUES_SIZE]
        = { 20, 10, 5, 2, 7, 15, 13, 17, 30, 25, 23, 28, 35, 33, 38 };
    for (size_t i = 0; i < VALUES_SIZE; i++) {
        bst_insert(&bst, values[i]);
    }


#ifdef TEST_MIN_MAX_HEIGHT
    printf("\n<------------------------------------------------------\n");
    printf("[ TEST ]: min, max, height (after insertion)\n\n");

    printf("\n[height]: %zd\n", bst_findheight(&bst));

    printf("[min]: ");
    if ((minnode = bst_findmin(&bst))) {
        printf("%d\n", minnode->value);
    } else {
        printf("tree is empty\n");
    }

    printf("[max]: ");
    if ((maxnode = bst_findmax(&bst))) {
        printf("%d\n", maxnode->value);
    } else {
        printf("tree is empty\n");
    }

    printf(">------------------------------------------------------\n\n");
#endif


#if 0
    /* Test printf */
    printf("After insertion:\n");
    printf("bst.root->value: %d\n", bst.root->value);
    printf("bst.root->left->value: %d\n", bst.root->left->value);
    printf("bst.root->right->value: %d\n", bst.root->right->value);
    printf("size: %zu\n", bst.length);
#endif


    /* Test find */
    bnode_t* foundval = NULL;

    for (size_t i = 0; i < VALUES_SIZE; i++) {
        if ((foundval = bst_find(&bst, values[i]))) {
            if (values[i] == foundval->value) {
                printf("%02zu: finding %2d -> found %2d, (OK)\n", i, values[i], foundval->value);
            }
        } else {
            printf("Not found!\n");
        }
    }

    if (!(foundval = bst_find(&bst, 0))) {
        printf("Not found!\n");
    }


#if 1
    /* Deletion */
    for (size_t i = 0; i < VALUES_SIZE; i++) {
        bst_del(&bst, values[i]);
    }

    // printf("%d\n", bst.root->value);
    printf("\nAfter deletion:\n");
    printf("size: %zu\n", bst.length);
#endif


#ifdef TEST_MIN_MAX_HEIGHT
    printf("\n<------------------------------------------------------\n");
    printf("[ TEST ]: min, max, height (after deletion)\n\n");

    printf("\n[height]: %zd\n", bst_findheight(&bst));

    printf("[min]: ");
    if ((minnode = bst_findmin(&bst))) {
        printf("%d\n", minnode->value);
    } else {
        printf("tree is empty\n");
    }

    printf("[max]: ");
    if ((maxnode = bst_findmax(&bst))) {
        printf("%d\n", maxnode->value);
    } else {
        printf("tree is empty\n");
    }

    printf(">------------------------------------------------------\n\n");
#endif


    return 0;
}


// vim: fdm=indent fdn=1
