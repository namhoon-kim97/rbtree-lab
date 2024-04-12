#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

static void delete (rbtree *t, node_t *n);
static void left_rotate(rbtree *t, node_t *n);
static void right_rotate(rbtree *t, node_t *n);
static void insert_fixup(rbtree *t, node_t *z);
static node_t *_min(const rbtree *t, node_t *nxt);
static void erase_fixup(rbtree *t, node_t *x);
static void rb_transplant(rbtree *t, node_t *x, node_t *y);
static void inorder_traversal(const rbtree *t, node_t *p, key_t *arr,
                              const size_t n, int *idx);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (p == NULL) {
    fprintf(stderr, "Memory allocation failed for rbtree pointer\n");
    exit(EXIT_FAILURE);
  }
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  if (nil == NULL) {
    fprintf(stderr, "Memory allocation failed for nil pointer\n");
    exit(EXIT_FAILURE);
  }
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
  if (z == NULL) {
    fprintf(stderr, "Memory allocation failed for new_node pointer\n");
    exit(EXIT_FAILURE);
  }
  while (x != t->nil) {
    y = x; // z의 부모노드를 변수 y에 저장.
    if (key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  z->parent = y;
  z->key = key;
  if (y == t->nil)
    t->root = z;
  else if (z->key < y->key)
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
  node_t *nxt = t->root;
  while (nxt != t->nil) {
    if (nxt->key == key)
      return nxt;
    else if (nxt->key < key)
      nxt = nxt->right;
    else
      nxt = nxt->left;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *nxt = t->root;
  if (nxt == t->nil)
    return nxt;
  while (nxt->left != t->nil) {
    nxt = nxt->left;
  }
  return nxt;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *nxt = t->root;
  if (nxt == t->nil)
    return nxt;
  while (nxt->right != t->nil) {
    nxt = nxt->right;
  }
  return nxt;
}

// doubly black을 없애는 과정
static void erase_fixup(rbtree *t, node_t *x) {
  node_t *w;
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) { // x가 부모 노드의 왼쪽 자식이면
      w = x->parent->right;     // w는 x의 형제노드
      if (w->color == RBTREE_RED) {
        // case1 x의 형제노드를 black으로 만들어주고 이 후 case2,3,4 중에서 해결
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }

      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        // case2 x의 doubly black과 w의 black을 부모에게 전달 후 위임
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          // case3 w->right->color 를 red로 만들어주고 case4로 해결
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        // case4 w->right->color가 red
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root; // rbtree 속성 만족 후 while문 탈출조건 설정
      }
    } else {               // x가 부모 노드의 오른쪽 자식이면
      w = x->parent->left; // w는 x의 형제노드
      if (w->color == RBTREE_RED) {
        // case1
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        // case2
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) {
          // case3
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        // case4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root; // rbtree 속성 만족 후 while문 탈출조건 설정
      }
    }
  }
  x->color = RBTREE_BLACK;
}

static void rb_transplant(rbtree *t, node_t *x, node_t *y) {
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->parent = x->parent;
}

// 노드 nxt밑으로 가장 작은 값을 가진 노드 반환
static node_t *_min(const rbtree *t, node_t *nxt) {
  while (nxt->left != t->nil) {
    nxt = nxt->left;
  }
  return nxt;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *x;
  node_t *y = p;
  color_t y_original_color = y->color;

  if (p->left == t->nil) {
    x = p->right;
    rb_transplant(t, p, p->right);
  } else if (p->right == t->nil) {
    x = p->left;
    rb_transplant(t, p, p->left);
  } else {
    y = _min(t, p->right);
    y_original_color = y->color;
    x = y->right;
    if (y != p->right) {
      rb_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    } else {
      x->parent = y; // x가 nil노드일 경우 x->parent값이 없으므로 명시해줘야 함.
    }

    rb_transplant(t, p, y);

    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if (y_original_color == RBTREE_BLACK)
    erase_fixup(t, x);
  free(p);
  return 0;
}

static void inorder_traversal(const rbtree *t, node_t *p, key_t *arr,
                              const size_t n, int *idx) {
  if (p == t->nil || *idx > n) {
    return;
  }
  inorder_traversal(t, p->left, arr, n, idx);
  arr[*idx] = p->key;
  (*idx)++;
  inorder_traversal(t, p->right, arr, n, idx);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int idx = 0;
  inorder_traversal(t, t->root, arr, n, &idx);
  return 0;
}