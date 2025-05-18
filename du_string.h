#ifndef DU_STRING_H_
#define DU_STRING_H_

#include "portable_types.h"
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
  u32 len;
  u32 cap;
} du_str;

typedef struct
{
  u8 stat;
  union
  {
    du_str* val;
    char* err_msg;
  } ret;
} du_str_res;



du_str_res
du_str_new(u32 size);

u32
du_strlen(char* in);

void
du_memcpy(char* dest, const char* src, u32 n);

du_str_res
du_str_from(char* in);

void
du_str_free(du_str* a);

du_str_res
du_str_ext_to(du_str* str, u32 len);

du_str_res
du_str_cat(du_str* str, char* t, u32 t_len);

u8
du_str_trim_trail(du_str* s, u32 n);

u8
du_str_trim_lead(du_str* s, u32 n);

#endif /* DU_STRING_H_ */
