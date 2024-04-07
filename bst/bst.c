#include "bst.h"

#include <stdlib.h>

bst *new_bst(void) {
  bst *p = (bst *)calloc(1, sizeof(bst));
  if (p == NULL) {
    // 메모리 할당 실패 처리
    return NULL;
  }
  // TODO: initialize struct if needed
  return p;
}

static void delete_bst_all_node(node_t *n) {
  if (n == NULL)
    return;
  delete_bst_all_node(n->left);
  delete_bst_all_node(n->right);
  free(n);
}

void delete_bst(bst *t) {
  if (t == NULL)
    return;
  delete_bst_all_node(t->root);
  t->root = NULL;
  free(t);
}

static node_t *bst_insert_node(node_t *n, const key_t key) {
  if (n == NULL) {
    n = (node_t *)malloc(sizeof(node_t)); // 새 노드 메모리 할당
    if (n == NULL)
      return NULL; // 메모리 할당 실패 처리
    n->right = NULL;
    n->left = NULL;
    n->key = key;
    return n;
  }
  if (n->key > key)
    n->left = bst_insert_node(n->left, key);
  else if (n->key < key)
    n->right = bst_insert_node(n->right, key);
  return n;
}

node_t *bst_insert(bst *t, const key_t key) {
  // TODO: implement insert
  if (t == NULL)
    return NULL;
  return t->root = bst_insert_node(t->root, key);
}

static node_t *bst_find_node(node_t *n, const key_t key) {
  if (!n)
    return NULL;
  if (key < n->key)
    return bst_find_node(n->left, key);
  else if (key > n->key)
    return bst_find_node(n->right, key);
  return n;
}

node_t *bst_find(const bst *t, const key_t key) {
  // TODO: implement find
  if (!t)
    return NULL;
  return bst_find_node(t->root, key);
}

node_t *bst_min(const bst *t) {
  // TODO: implement find
  node_t *nxt = t->root;
  if (nxt == NULL)
    return NULL;
  while (nxt->left != NULL) {
    nxt = nxt->left;
  }
  return nxt;
}

node_t *bst_max(const bst *t) {
  node_t *nxt = t->root;
  if (nxt == NULL)
    return NULL;
  while (nxt->right != NULL) {
    nxt = nxt->right;
  }
  return nxt;
}

static node_t *min_key_node(node_t *n) {
  if (n == NULL)
    return NULL;
  while (n->left != NULL) {
    n = n->left;
  }
  return n;
}

static node_t *bst_erase_node(node_t *n, key_t key) {
  if (!n)
    return n;

  if (key < n->key)
    n->left = bst_erase_node(n->left, key);
  else if (key > n->key)
    n->right = bst_erase_node(n->right, key);
  else {
    if (!n->left) {
      node_t *temp = n->right;
      free(n);
      return temp;
    } else if (!n->right) {
      node_t *temp = n->left;
      free(n);
      return temp;
    }
    node_t *temp = min_key_node(n->right);
    n->key = temp->key;
    n->right = bst_erase_node(n->right, temp->key);
  }
  return n;
}

int bst_erase(bst *t, node_t *p) {
  // TODO: implement erase
  t->root = bst_erase_node(t->root, p->key);
  return 0;
}

int rbtree_to_array(const bst *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
