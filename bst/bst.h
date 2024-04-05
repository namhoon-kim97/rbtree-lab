#ifndef BST_H
#define BST_H

#include <stddef.h>

typedef int key_t;

typedef struct node_t {
    key_t key;
    struct node_t *left, *right;
} node_t;

typedef struct {
    node_t *root;
} bst;

bst *new_bst(void);
void delete_bst(bst *);

node_t *bst_insert(bst *, const key_t);
node_t *bst_find(const bst *, const key_t);
node_t *bst_min(const bst *);
node_t *bst_max(const bst *);
int bst_erase(bst *, node_t *);

int bst_to_array(const bst *, key_t *, const size_t);

#endif
