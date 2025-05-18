#include "du_string.h"
#include <stdio.h>

du_str_res
du_str_new(u32 size)
{
  du_str_res a = { 0 };
  du_str* b = NULL;

  if (size == 0) {
    RETURN_ERR(
      a, DU_ERR, "string size cannot be 0 in " __FILE__ " line " LINE_STRING);
  }

  b = (du_str*)DU_MALLOC(sizeof(du_str));
  if (b == NULL) {
    RETURN_ERR(a,
               DU_ERR,
               "unable to allocate memory for string in " __FILE__
               " line " LINE_STRING);
  }

  b->cont = (char*)DU_MALLOC(size + NULL_TERM_SIZE);
  if (b->cont == NULL) {
    DU_FREE(b);
    RETURN_ERR(a,
               DU_ERR,
               "unable to allocate memory for string content in " __FILE__
               " line " LINE_STRING);
  }

  b->cont[0] = '\0';
  b->cap = size + NULL_TERM_SIZE;
  b->len = 0;

  RETURN_OK(a, b);
}

u32
du_strlen(char* in)
{
  size_t len = 0;
  while (in[len]) {
    ++len;
  }
  return len;
}


void
du_memcpy(char* dest, const char* src, u32 n)
{
  u32 i = 0;
  for (; i < n; ++i) {
    dest[i] = src[i];
  }
}


du_str_res
du_str_from(char* in)
{
  du_str_res a = { 0 };
  du_str* b = NULL;
  u32 len = du_strlen(in);

  a = du_str_new(len);
  if (a.stat != DU_OK) {
    return a;
  }
  b = a.ret.val;

  du_memcpy(b->cont, in, len);

  b->cont[len] = 0;
  b->len = len;

  RETURN_OK(a, b);
}

void
du_str_free(du_str* a)
{
  if (a != NULL) {
    if (a->cont != NULL) {
      DU_FREE(a->cont);
    }
    DU_FREE(a);
  }
}

du_str_res
du_str_ext_to(du_str* str, u32 len)
{
  du_str_res a = { 0 };
  char* tmp = NULL;

  if (len <= str->cap) {
    RETURN_ERR(a,
               DU_INV_INPUT,
               "this function is for extending only. in" __FILE__
               " line " LINE_STRING);
  }

  tmp = (char*)DU_REALOC(str->cont, len + NULL_TERM_SIZE);
  if (tmp == NULL) {
    RETURN_ERR(a,
               DU_ERR,
               "unable to realloc str content in " __FILE__
               " line " LINE_STRING);
  }
  str->cont = tmp;

  str->cap = len + NULL_TERM_SIZE;

  RETURN_OK(a, str);
}

du_str_res
du_str_cat(du_str* str, char* t, u32 t_len)
{
  du_str_res a = { 0 };
  u32 c = str->len + t_len;

  if (!(str->cap >= c + NULL_TERM_SIZE)) {
    a = du_str_ext_to(str, c);
    if (a.stat != DU_OK) {
      return a;
    }
    str = a.ret.val;
  }

  du_memcpy(str->cont + str->len, t, t_len);

  str->cont[c] = '\0';
  str->len = c;

  RETURN_OK(a, str);
}

u8
du_str_trim_lead(du_str* s, u32 n)
{
  u32 i = 0;
  if (n > s->len) {
    return DU_INV_INPUT;
  }
  for (; i <= s->len - n; ++i) {
    s->cont[i] = s->cont[i + n];
  }
  return DU_OK;
}

u8
du_str_trim_trail(du_str* s, u32 n)
{
  if (n > s->len) {
    return DU_INV_INPUT;
  }
  s->cont[s->len - n] = '\0';
  s->len -= n;
  return DU_OK;
}
