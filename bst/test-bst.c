#include "bst.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void test_init(void) {
  bst *t = new_bst();
  assert(t != NULL);

  assert(t->root == NULL);
  delete_bst(t);
}

void test_insert_single(const key_t key) {
  bst *t = new_bst();
  node_t *p = bst_insert(t, key);
  assert(p != NULL);
  assert(t->root == p);
  assert(p->key == key);

  assert(p->left == NULL);
  assert(p->right == NULL);

  delete_bst(t);
}

void test_find_single(const key_t key, const key_t wrong_key) {
  bst *t = new_bst();
  node_t *p = bst_insert(t, key);

  node_t *q = bst_find(t, key);
  assert(q != NULL);
  assert(q->key == key);
  assert(q == p);

  q = bst_find(t, wrong_key);
  assert(q == NULL);

  delete_bst(t);
}

static void insert_arr(bst *t, const key_t *arr, const size_t n) {
  for (size_t i = 0; i < n; i++) {
    bst_insert(t, arr[i]);
  }
}

static int comp(const void *p1, const void *p2) {
  const key_t *e1 = (const key_t *)p1;
  const key_t *e2 = (const key_t *)p2;
  if (*e1 < *e2) {
    return -1;
  } else if (*e1 > *e2) {
    return 1;
  } else {
    return 0;
  }
};

// min/max should return the min/max value of the tree
void test_minmax(key_t *arr, const size_t n) {
  // null array is not allowed
  assert(n > 0 && arr != NULL);

  bst *t = new_bst();
  assert(t != NULL);

  insert_arr(t, arr, n);
  assert(t->root != NULL);

  qsort((void *)arr, n, sizeof(key_t), comp);
  node_t *p = bst_min(t);
  assert(p != NULL);
  assert(p->key == arr[0]);

  node_t *q = bst_max(t);
  assert(q != NULL);
  assert(q->key == arr[n - 1]);

  bst_erase(t, p);
  p = bst_min(t);
  assert(p != NULL);
  assert(p->key == arr[1]);

  if (n >= 2) {
    bst_erase(t, q);
    q = bst_max(t);
    assert(q != NULL);
    assert(q->key == arr[n - 2]);
  }

  delete_bst(t);
}

void test_minmax_suite() {
  key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_minmax(entries, n);
}

int main(int argc, char *argv[]) {
    test_init();
    test_insert_single(123);
    test_find_single(123, 234);
    test_minmax_suite();
    printf("test success!\n");
}