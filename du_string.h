#ifndef DU_STRING_H_
#define DU_STRING_H_

#include "du_portable_types.h"
#include <stdbool.h>
#include <stdlib.h>

#define DU_MALLOC malloc
#define DU_REALOC realloc
#define DU_FREE free

#define NULL_TERM_SIZE 1

enum
{
  DU_OK,
  DU_ERR,
  DU_INV_INPUT
};

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

#define RETURN_OK(G, V)                                                        \
  G.stat = DU_OK;                                                              \
  G.ret.val = V;                                                               \
  return G;

#define RETURN_ERR(G, E, V)                                                    \
  G.stat = E;                                                                  \
  G.ret.err_msg = V;                                                           \
  return G;

typedef struct
{
  char* cont;
  du_u32 len;
  du_u32 cap;
} du_str;

typedef struct
{
  du_u8 stat;
  union
  {
    du_str* val;
    char* err_msg;
  } ret;
} du_str_res;

typedef struct
{
  du_u32* cont;
  du_u32 len;
  du_u32 cap;
} du_u32_list;

typedef struct
{
  bool stat;
  du_u32 val;
} du_u32_opt;

typedef struct
{
  bool stat;
  du_u32_list val;
} du_u32_list_opt;

typedef struct
{
  du_u8 stat;
  union
  {
    du_u32_opt val;
    char* err_msg;
  } ret;
} du_u32_opt_res;

du_str_res
du_str_new(du_u32 size);

du_u32
du_strlen(char* in);

void
du_memcpy(char* dest, const char* src, du_u32 n);

du_str_res
du_str_from(char* in);

void
du_str_free(du_str* a);

du_str_res
du_str_ext_to(du_str* str, du_u32 len);

du_str_res
du_str_cat(du_str* str, char* t, du_u32 t_len);

du_u8
du_str_trim_trail(du_str* s, du_u32 n);

du_u8
du_str_trim_lead(du_str* s, du_u32 n);

bool
du_memcmpb(const char* s1, const char* s2, du_u32 n);

du_u32_opt_res
du_str_find(du_str* a, du_u32 start, char* f, du_u32 f_len);

#endif /* DU_STRING_H_ */
