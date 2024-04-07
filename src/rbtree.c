#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

static void delete (rbtree *t, node_t *n);
static void left_rotate(rbtree *t, node_t *n);
static void right_rotate(rbtree *t, node_t *n);
static void insert_fixup(rbtree *t, node_t *z);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  p->root = nil;
  p->nil = nil;
  nil->color = RBTREE_BLACK;
  p->root->left = nil;
  p->root->right = nil;
  p->root->parent = nil;
  return p;
}

static void delete (rbtree *t, node_t *n) {
  if (n == t->nil)
    return;
  delete (t, n->left);
  delete (t, n->right);
  free(n);
}

void delete_rbtree(rbtree *t) {
  delete (t, t->root);
  free(t->nil);
  free(t);
}

static void left_rotate(rbtree *t, node_t *n) {
  node_t *temp = n->right;
  n->right = temp->left;
  if (temp->left != t->nil)
    temp->left->parent = n;
  temp->parent = n->parent;
  if (n->parent == t->nil)
    t->root = temp;
  else if (n == n->parent->left)
    n->parent->left = temp;
  else
    n->parent->right = temp;
  temp->left = n;
  n->parent = temp;
}

static void right_rotate(rbtree *t, node_t *n) {
  node_t *temp = n->left;
  n->left = temp->right;
  if (temp->right != t->nil)
    temp->right->parent = n;
  temp->parent = n->parent;
  if (n->parent == t->nil)
    t->root = temp;
  else if (n == n->parent->left)
    n->parent->left = temp;
  else
    n->parent->right = temp;
  temp->right = n;
  n->parent = temp;
}

static void insert_fixup(rbtree *t, node_t *z) {
  while (z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left) {
      node_t *y = z->parent->parent->right; // uncle node
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    } else {
      node_t *y = z->parent->parent->left; // uncle node
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *x = t->root;
  node_t *y = t->nil;
  node_t *z = (node_t *)malloc(sizeof(node_t));
  z->key = key;
  while (x != t->nil) {
    y = x;
    if (key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  z->parent = y;
  if (y == t->nil)
    t->root = z;
  else if (key < y->key)
    y->left = z;
  else
    y->right = z;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  insert_fixup(t, z);
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
