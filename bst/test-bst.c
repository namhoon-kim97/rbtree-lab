#include "bst.h"
#include <assert.h>
#include <stdio.h>
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

static void inorder(node_t *n) {
  if (!n)
    return;

  inorder(n->left);
  printf("%d ", n->key);
  inorder(n->right);
}

void test_find_erase(bst *t, const key_t *arr, const size_t n) {
  for (int i = 0; i < n; i++) {
    node_t *p = bst_insert(t, arr[i]);
    assert(p != NULL);
  }

  // 중위순회 트리 출력
  // inorder(t->root);

  for (int i = 0; i < n; i++) {
    node_t *p = bst_find(t, arr[i]);
    // printf("arr[%d] = %d\n", i, arr[i]);
    assert(p != NULL);
    assert(p->key == arr[i]);
    bst_erase(t, p);
  }

  for (int i = 0; i < n; i++) {
    node_t *p = bst_find(t, arr[i]);
    assert(p == NULL);
  }

  for (int i = 0; i < n; i++) {
    node_t *p = bst_insert(t, arr[i]);
    assert(p != NULL);
    node_t *q = bst_find(t, arr[i]);
    assert(q != NULL);
    assert(q->key == arr[i]);
    assert(p == q);
    bst_erase(t, p);
    q = bst_find(t, arr[i]);
    assert(q == NULL);
  }
}

void test_find_erase_fixed() {
  const key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 25, 36, 990, 26};
  const size_t n = sizeof(arr) / sizeof(arr[0]);
  bst *t = new_bst();
  assert(t != NULL);

  test_find_erase(t, arr, n);

  delete_bst(t);
}

void test_find_erase_rand(const size_t n, const unsigned int seed) {
  srand(seed);
  bst *t = new_bst();
  key_t *arr = calloc(n, sizeof(key_t));
  for (int i = 0; i < n; i++)
    arr[i] = rand();

  test_find_erase(t, arr, n);

  free(arr);
  delete_bst(t);
}

int main(int argc, char *argv[]) {
  test_init();
  test_insert_single(123);
  test_find_single(123, 234);
  // test_erase_root(128);
  test_find_erase_fixed();
  test_minmax_suite();
  test_find_erase_rand(10000, 17);
  printf("test success!\n");
}