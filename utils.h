#ifndef __UTILS_H__
#define __UTILS_H__

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* string settings */
#define STRING_NAME String
#define STRING_FUNC(SUF) String##_##SUF
#define STRING_ALLOC_SIZE uint32_t
#define STRING_ALLOC_SIZE_MAX UINT32_MAX

/* status settings */
#define STATUS_NAME Status
#define STATUS_FUNC(SUF) Status##_##SUF
#define STATUS_CODE_SIZE uint8_t
#define STATUS_ERR_SIZE STATUS_CODE_SIZE
#define STATUS_FUNCP_SIZE uint16_t
#define STATUS_SET_ERR(T, U, V)                                                \
  T->status = STATUS_ERR;                                                      \
  T->code = U;                                                                 \
  T->func = V
#define STATUS_SET_OK(T, V)                                                    \
  T->status = STATUS_OK;                                                       \
  T->code = STATUS_CODE_NONE;                                                  \
  T->func = V
enum STATUS_STATUS { STATUS_OK, STATUS_ERR };
enum STATUS_CODE {
  STATUS_CODE_NONE,
  STATUS_CODE_FAIL_ALLOC,
  STATUS_CODE_FAIL_REALLOC,
  STATUS_CODE_INVALID_INPUT,
  STATUS_CODE_INVALID_NULL,
  STATUS_CODE_BROKEN_STRUCT,
  STATUS_CODE_FAIL_STRCPY,
  STATUS_CODE_FAIL_MEMMOVE,
  STATUS_CODE_MAX
};
static const char *err_expl_lookup[STATUS_CODE_MAX] = {
    "no error",
    "failed to allocate memory",
    "failed to reallocate memory",
    "invalid input given",
    "invalid null pointer given",
    "struct seems to be broken",
    "call to strcpy failed",
    "call to memmove failed"};

/* function numbers and names */
static const char *func_name_lookup[] = {"NONE",
                                         "String_new()",
                                         "Status_pcheck()",
                                         "String_from()",
                                         "Status_check()",
                                         "String_free()",
                                         "String_expand_by()",
                                         "String_lead_trunc()",
                                         "String_trunc()"};
enum Function_numbers {
  FUNC_NONE = 0,
  FUNC_STRING_NEW = 1,
  FUNC_STATUS_PCHECK,
  FUNC_STRING_FROM,
  FUNC_STATUS_CHECK,
  FUNC_STRING_FREE,
  FUNC_STRING_EXPAND_BY,
  FUNC_STRING_LEAD_TRUNC,
  FUNC_STRING_TRUNC
};

/* array settings */
#define ARRAY_NAME Array
#define ARRAY_ALLOC_SIZE uint32_t
#define ARRAY_ELEM_SIZE size_t

typedef struct ARRAY_NAME {
  /* the size allocated for the content void pointer */
  ARRAY_ALLOC_SIZE allocated;
  /* the amount of space elements in content */
  ARRAY_ALLOC_SIZE used;
  /* the size of an individual element */
  ARRAY_ELEM_SIZE elem_size;
  /* pointer to the content it self */
  void *content;
} ARRAY_NAME;

typedef struct STATUS_NAME {
  /* status error or ok */
  STATUS_CODE_SIZE status;
  /* the error code */
  STATUS_ERR_SIZE code;
  /* number of the error function */
  STATUS_FUNCP_SIZE func;
} STATUS_NAME;

typedef struct STRING_NAME {
  /* the size allocated for the content char pointer */
  STRING_ALLOC_SIZE allocated;
  /* the size of content including the null terminator */
  STRING_ALLOC_SIZE used;
  /* pointer to the content it self */
  char *content;
} STRING_NAME;

STRING_NAME *STRING_FUNC(new)(STRING_ALLOC_SIZE a, STATUS_NAME *status);
bool STATUS_FUNC(pcheck)(const STATUS_NAME *a, FILE *output);
STRING_NAME *STRING_FUNC(from)(const char *a, STATUS_NAME *status);
bool STATUS_FUNC(check)(const STATUS_NAME *a);
void STRING_FUNC(free)(STRING_NAME *a, STATUS_NAME *status);
STRING_NAME *STRING_FUNC(expand_by)(STRING_NAME *s, STRING_ALLOC_SIZE x,
                                    STATUS_NAME *status);
STRING_NAME *STRING_FUNC(lead_trunc)(STRING_NAME *s, STRING_ALLOC_SIZE n,
                                     STATUS_NAME *status);
STRING_NAME *STRING_FUNC(trunc)(STRING_NAME *s, STRING_ALLOC_SIZE n,
                                STATUS_NAME *status);

#ifdef UTILS_H_IMPLEMENTATION

/* Function number 1.
 * Creates a new string and allocates x bytes for it.
 */
STRING_NAME *STRING_FUNC(new)(STRING_ALLOC_SIZE a, STATUS_NAME *status) {

  STRING_NAME *s = NULL;

  if (a == 0) {
    STATUS_SET_ERR(status, STATUS_CODE_INVALID_INPUT, FUNC_STRING_NEW);
    return NULL;
  }

  s = malloc(sizeof(STRING_NAME));
  if (s == NULL) {
    STATUS_SET_ERR(status, STATUS_CODE_FAIL_ALLOC, FUNC_STRING_NEW);
    return NULL;
  }

  s->content = malloc(sizeof(char) * a);
  if (s->content == NULL) {
    free(s);
    STATUS_SET_ERR(status, STATUS_CODE_FAIL_ALLOC, FUNC_STRING_NEW);
    return NULL;
  }

  s->allocated = a;
  s->used = 1;
  s->content[0] = '\0';

  STATUS_SET_OK(status, FUNC_STRING_NEW);

  return s;
}

/* Function number 2.
 * Check for an error in status and print it.
 * Returns true if an error is detected otherwise false.
 */
bool STATUS_FUNC(pcheck)(const STATUS_NAME *a, FILE *output) {
  if (a == NULL) {
    fprintf(output, "status pointer is null\n");
    return true;
  }
  if (output == NULL) {
    fprintf(stderr,
            "Null pointer give as output file. Using stderr as fallback");
    output = stderr;
  }
  if (a->status == STATUS_ERR) {
    fprintf(output, "%s in function %s\n", err_expl_lookup[a->code],
            func_name_lookup[a->func]);
    return true;
  } else if (a->status == STATUS_OK) {
    return false;
  } else {
    fprintf(output, "unknown error code: %i\n", a->code);
    return true;
  }
}

/* Function number 3.
 * Create a string from c string.
 */
STRING_NAME *STRING_FUNC(from)(const char *a, STATUS_NAME *status) {

  String *s = NULL;
  STRING_ALLOC_SIZE size = 0;

  if (a == NULL) {
    STATUS_SET_ERR(status, STATUS_CODE_INVALID_NULL, FUNC_STRING_FROM);
    return NULL;
  }

  size = strlen(a) + 1; /* plus one for the null terminator */

  s = STRING_FUNC(new)(size, status);
  if (STATUS_FUNC(check)(status)) {
    /* We just return null because the status was already set by the previous
     * function. */
    return NULL;
  }
  if (s == NULL) {
    STATUS_SET_ERR(status, STATUS_CODE_BROKEN_STRUCT, FUNC_STRING_FROM);
    return NULL;
  }

  /* plus one for the null terminator */
  memcpy(s->content, a, strlen(a) + 1);

  s->used = size;

  STATUS_SET_OK(status, FUNC_STRING_FROM);

  return s;
}

/* Function number 4.
 * Check for an error in status but *DON'T* print it.
 */
bool STATUS_FUNC(check)(const STATUS_NAME *a) {
  if (a == NULL) {
    return true;
  }
  if (a->status == STATUS_ERR) {
    return true;
  } else if (a->status == STATUS_OK) {
    return false;
  } else {
    return true;
  }
}

/* Function number 5.
 * Free a string.
 */
void STRING_FUNC(free)(STRING_NAME *a, STATUS_NAME *status) {
  if (a == NULL) {
    STATUS_SET_ERR(status, STATUS_CODE_INVALID_NULL, FUNC_STRING_FREE);
    return;
  }
  if (a->content != NULL) {
    free(a->content);
  }
  STATUS_SET_OK(status, FUNC_STRING_FREE);
  free(a);
}

/* function number 6
 * Expands s->allocated by x
 */
STRING_NAME *STRING_FUNC(expand_by)(STRING_NAME *s, STRING_ALLOC_SIZE x,
                                    STATUS_NAME *status) {
  char *new_content = NULL;
  STRING_ALLOC_SIZE new_size = 0;

  if (s == NULL) {
    STATUS_SET_ERR(status, STATUS_CODE_INVALID_NULL, FUNC_STRING_EXPAND_BY);
    return s;
  }

  new_size = (s->allocated + x);

  if (s->content == NULL) {
    STATUS_SET_ERR(status, STATUS_CODE_BROKEN_STRUCT, FUNC_STRING_EXPAND_BY);
    return s;
  }
  if (x == 0) {
    STATUS_SET_ERR(status, STATUS_CODE_INVALID_INPUT, FUNC_STRING_EXPAND_BY);
    return s;
  }
  if (x > STRING_ALLOC_SIZE_MAX - s->allocated) {
    STATUS_SET_ERR(status, STATUS_CODE_INVALID_INPUT, FUNC_STRING_EXPAND_BY);
    return s;
  }

  new_content = (char *)realloc((STRING_NAME *)s->content, new_size);
  if (new_content == NULL) {
    STATUS_SET_ERR(status, STATUS_CODE_FAIL_REALLOC, FUNC_STRING_EXPAND_BY);
    return s;
  }

  s->content = new_content;
  s->allocated = new_size;

  STATUS_SET_OK(status, FUNC_STRING_EXPAND_BY);

  return s;
}

/* Fucntion number 7
 * Remove x amount of characters from the beginning.
 */
STRING_NAME *STRING_FUNC(lead_trunc)(STRING_NAME *s, STRING_ALLOC_SIZE n,
                                     STATUS_NAME *status) {
  if (s == NULL) {
    STATUS_SET_ERR(status, STATUS_CODE_INVALID_NULL, FUNC_STRING_LEAD_TRUNC);
    return s;
  }
  if (s->content == NULL) {
    STATUS_SET_ERR(status, STATUS_CODE_BROKEN_STRUCT, FUNC_STRING_LEAD_TRUNC);
    return s;
  }

  if (n == 0 || n > s->used) {
    STATUS_SET_ERR(status, STATUS_CODE_INVALID_INPUT, FUNC_STRING_LEAD_TRUNC);
    return s;
  }

  memmove(s->content, s->content + n, s->used - n);
  s->used -= n;

  STATUS_SET_OK(status, FUNC_STRING_LEAD_TRUNC);

  return s;
}

/* Fucntion number 7
 * Remove x amount of characters from the end.
 */
STRING_NAME *STRING_FUNC(trunc)(STRING_NAME *s, STRING_ALLOC_SIZE n,
                                STATUS_NAME *status) {
  if (s == NULL) {
    STATUS_SET_ERR(status, STATUS_CODE_INVALID_INPUT, FUNC_STRING_TRUNC);
    return s;
  }
  if (s->content == NULL) {
    STATUS_SET_ERR(status, STATUS_CODE_BROKEN_STRUCT, FUNC_STRING_TRUNC);
    return s;
  }

  if (n == 0) {
    STATUS_SET_ERR(status, STATUS_CODE_INVALID_INPUT, FUNC_STRING_TRUNC);
    return s;
  }

  /* minus one for the null terminator */
  s->content[s->used - 1 - n] = '\0';
  s->used -= n;

  STATUS_SET_OK(status, FUNC_STRING_TRUNC);

  return s;
}

#endif /* ifdef UTILS_H_IMPLEMENTATION */
#endif /* ifndef __UTILS_H__ */
