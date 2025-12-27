#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define QP_MALLOC malloc
#define QP_FREE free
#define QP_REALLOC realloc
#define QP_STR_DEF_ALLOC 32


typedef uintptr_t qp_uint;
typedef intptr_t qp_int;
typedef char qp_char;


typedef struct qp_str
{
  qp_char* data;     /* the actual string data */
  qp_uint used;      /* this includes the null terminator */
  qp_uint allocated; /* represents the amount of items allocated not bytes */
} qp_str_t;


qp_uint
qp_strlen(qp_char* a)
{
  qp_uint len = 0;
  qp_char cur = a[len];
  while (true) {
    if (cur == '\0') {
      break;
    }
    ++len;
    cur = a[len];
  }
  return len;
}


void
qp_strcpy(qp_char* dest, qp_char* src)
{
  qp_uint i;
  qp_uint src_len = qp_strlen(src);
  for (i = 0; i < src_len; ++i) {
    dest[i] = src[i];
  }
  dest[src_len] = '\0';
}


bool
qp_strequ(qp_char* a, qp_char* b)
{
  qp_uint a_len = qp_strlen(a);
  qp_uint b_len = qp_strlen(b);
  qp_uint i;
  if (b_len != a_len) {
    return false;
  }
  for (i = 0; i <= a_len; ++i) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}


qp_str_t*
qp_str_new(qp_uint s)
{
  qp_str_t* a;

  if (s == 0) {
    return NULL;
  }

  a = QP_MALLOC(sizeof(qp_str_t));
  if (a == NULL) {
    return NULL;
  }

  a->data = QP_MALLOC(sizeof(qp_char) * s);
  if (a->data == NULL) {
    QP_FREE(a);
    return NULL;
  }

  a->data[0] = '\0';
  a->used = 1;
  a->allocated = s;

  return a;
}


qp_str_t*
qp_str_resize(qp_str_t* a, qp_uint s)
{
  qp_char* data;

  if (s == 0) {
    return NULL;
  }
  if (a == NULL) {
    return NULL;
  }
  if (a->data == NULL) {
    return NULL;
  }

  data = QP_REALLOC(a->data, sizeof(qp_char) * s);
  if (data == NULL) {
    return NULL;
  }

  a->data = data;

  if (s < a->allocated) {
    a->data[s - 1] = '\0';
    a->used = s;
  }

  a->allocated = s;

  return a;
}


qp_str_t*
qp_str_del(qp_str_t* a, qp_uint index)
{
  qp_uint i;
  if (index >
      a->used -
        2) { /* minus two is used for null terminator and zero indexing */
    return NULL;
  }

  for (i = index; i < a->used - 1;
       ++i) { /* minux one for the null terminator */
    a->data[i] = a->data[i + 1];
  }

  --a->used;

  return a;
}


qp_str_t*
qp_str_del_n(qp_str_t* a, qp_uint index, qp_uint n)
{
  qp_str_t* tmp;
  for (; n > 0; --n) {
    tmp = qp_str_del(a, index);
    if (tmp == NULL) {
      return NULL;
    }
  }
  return a;
}


qp_str_t*
qp_str_ins(qp_str_t* a, qp_uint index, qp_char b)
{
  qp_str_t* tmp;
  qp_uint i;

  if (index > a->used - 1) { /* minus one is used for zero indexing */
    return NULL;
  }

  if (!(a->allocated >= a->used + 1)) {
    tmp = qp_str_resize(a, a->used + 1);
    if (tmp == NULL) {
      return NULL;
    }
    a = tmp;
  }

  for (i = a->used; i > index; --i) {
    a->data[i] = a->data[i - 1];
  }

  a->data[index] = b;
  ++a->used;

  return a;
}


qp_str_t*
qp_str_ins_str(qp_str_t* a, qp_uint b, qp_char* c)
{
  qp_uint i, c_len = qp_strlen(c), og_used = a->used;
  qp_str_t* tmp;
  qp_char d;

  if (b + 1 > og_used) {
    return NULL;
  }

  for (i = c_len; i > 0; --i) {
    /* FIX: the side effect of adding characters even when insertion fails */
    d = c[i - 1];
    tmp = qp_str_ins(a, b, d);
    if (tmp == NULL) {
      return NULL;
    }
  }

  if (b + 1 == og_used) {                /* plus one for null terminator */
    a->data[og_used + c_len - 1] = '\0'; /* minus one for zero indexing */
  }

  return a;
}


qp_str_t*
qp_str_cat(qp_str_t* a, qp_char* b)
{
  return qp_str_ins_str(a, a->used - 1, b); /* minus one for zero indexing*/
}


qp_str_t*
qp_str_from(qp_char* a)
{
  qp_uint len = qp_strlen(a) + 1; /* plus one for the null terminator */
  qp_str_t* str = qp_str_new(len);
  if (str == NULL) {
    return NULL;
  }
  qp_strcpy(str->data, a);
  str->data[len - 1] = '\0'; /* minus one for the zero indexing */
  str->used = len;
  return str;
}


void
qp_str_free(qp_str_t* a)
{
  if (a != NULL) {
    if (a->data != NULL) {
      QP_FREE(a->data);
    }
    QP_FREE(a);
  }
}


void
qp_str_free_generic(void* a)
{
  qp_str_free(a);
}


int
main(void)
{
  qp_str_t* str = qp_str_new(1);
  qp_str_t* str2;
  assert(str != NULL);
  assert(str->allocated == 1);
  assert(str->used == 1);
  assert(str->data[0] == '\0');
  qp_str_free(str);

  assert(qp_strlen("jarkko") == 6);
  assert(qp_strlen("") == 0);
  assert(qp_strlen("a") == 1);

  str = qp_str_new(2);
  assert(str != NULL);
  assert(str->allocated == 2);
  assert(str->used == 1);
  assert(str->data[0] == '\0');
  qp_str_free(str);

  str = qp_str_new(0);
  assert(str == NULL);

  str = qp_str_new(2);
  assert(str != NULL);
  str->data[0] = 'a';
  str->data[1] = '\0';
  str->used = 2;
  str2 = qp_str_resize(str, 1);
  assert(str2 != NULL);
  str = str2;
  str2 = NULL;
  assert(str->allocated == 1);
  assert(str->used == 1);
  assert(str->data[0] == '\0');
  qp_str_free(str);


  str = qp_str_new(2);
  assert(str != NULL);
  str->data[0] = 'a';
  str->data[1] = '\0';
  str->used = 2;
  str2 = qp_str_resize(str, 32);
  assert(str2 != NULL);
  str = str2;
  str2 = NULL;
  assert(str->allocated == 32);
  assert(str->used == 2);
  assert(str->data[0] == 'a');
  assert(str->data[1] == '\0');
  qp_str_free(str);

  str = qp_str_new(2);
  assert(str != NULL);
  str->data[0] = 'a';
  str->data[1] = '\0';
  str->used = 2;
  str2 = qp_str_del(str, 0);
  assert(str2 != NULL);
  str = str2;
  str2 = NULL;
  assert(str->allocated == 2);
  assert(str->used == 1);
  assert(str->data[0] == '\0');
  qp_str_free(str);


  str = qp_str_new(2);
  assert(str != NULL);
  str->data[0] = 'a';
  str->data[1] = '\0';
  str->used = 2;
  str2 = qp_str_ins(str, 1, 'b');
  assert(str2 != NULL);
  str = str2;
  str2 = NULL;
  assert(str->allocated == 3);
  assert(str->used == 3);
  assert(str->data[0] == 'a');
  assert(str->data[1] == 'b');
  assert(str->data[2] == '\0');
  qp_str_free(str);

  str = qp_str_from("hello world!\n");
  assert(str->data[4] == 'o');
  assert(str->data[12] == '\n');
  assert(str->data[13] == '\0');
  qp_str_free(str);

  str = qp_str_from("hello ");
  str2 = qp_str_ins_str(str, 6, "world!\n");
  assert(str2 != NULL);
  str = str2;
  str2 = NULL;
  assert(str->data[4] == 'o');
  assert(str->data[12] == '\n');
  assert(str->data[13] == '\0');
  qp_str_free(str);

  str = qp_str_from("hello ");
  str2 = qp_str_cat(str, "world!\n");
  assert(str2 != NULL);
  str = str2;
  str2 = NULL;
  assert(str->data[4] == 'o');
  assert(str->data[12] == '\n');
  assert(str->data[13] == '\0');
  qp_str_free(str);

  str = qp_str_from("hello world!\n");
  str2 = qp_str_del_n(str, 6, 7);
  assert(str2 != NULL);
  str = str2;
  str2 = NULL;
  assert(str->data[0] == 'h');
  assert(str->data[4] == 'o');
  assert(str->data[6] == '\0');
  qp_str_free(str);

  return 0;
}
