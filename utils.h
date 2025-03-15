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

/* status settings */
#define STATUS_NAME Status
#define STATUS_FUNC(SUF) Status##_##SUF
#define STATUS_CODE_SIZE uint8_t
#define STATUS_ERR_SIZE STATUS_CODE_SIZE
#define STATUS_FUNCP_SIZE uint16_t
enum STATUS_STATUS { STATUS_OK, STATUS_ERR };
enum STATUS_CODE {
  STATUS_CODE_NONE,
  STATUS_CODE_FAIL_ALLOC,
  STATUS_CODE_FAIL_REALLOC,
  STATUS_CODE_INVALID_INPUT,
  STATUS_CODE_INVALID_NULL,
  STATUS_CODE_BROKEN_STRUCT,
  STATUS_CODE_FAIL_STRCPY,
  STATUS_CODE_MAX
};
const char *err_expl_lookup[STATUS_CODE_MAX] = {"no error",
                                                "failed to allocate memory",
                                                "failed to reallocate memory",
                                                "invalid input given",
                                                "invalid null pointer given",
                                                "struct seems to be broken",
                                                "call to strcpy failed"};

/* function numbers and names */
const char *func_name_lookup[] = {
    "NONE",          "String_new()",   "Status_pcheck()",
    "String_from()", "Status_check()", "String_free()"};
enum Function_numbers {
  FUNC_NONE = 0,
  FUNC_STRING_NEW = 1,
  FUNC_STATUS_PCHECK,
  FUNC_STRING_FROM,
  FUNC_STATUS_CHECK,
  FUNC_STRING_FREE
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

#ifdef UTILS_H_IMPLEMENTATION

/* Function number 1.
 * Creates a new string and allocates x bytes for it.
 *
 * pseudo code (no error checking):
 * def String_new(int x):
 *   string s = malloc(sizeof(String))
 *   s.content = malloc(sizeof(char) * x)
 *   s.used = 0
 *   s.content[0] = "\0"
 *   return s
 */
STRING_NAME *STRING_FUNC(new)(STRING_ALLOC_SIZE a, STATUS_NAME *status) {

  STRING_NAME *s = NULL;

  if (status == NULL) {
    return NULL;
  }

  status->func = FUNC_STRING_NEW;

  if (a == 0) {
    status->status = STATUS_ERR;
    status->code = STATUS_CODE_INVALID_INPUT;
    return NULL;
  }

  s = malloc(sizeof(STRING_NAME));
  if (s == NULL) {
    status->status = STATUS_ERR;
    status->code = STATUS_CODE_FAIL_ALLOC;
    return NULL;
  }

  s->content = malloc(sizeof(char) * a);
  if (s->content == NULL) {
    free(s);
    status->status = STATUS_ERR;
    status->code = STATUS_CODE_FAIL_ALLOC;
    return NULL;
  }

  s->allocated = a;
  s->used = 1;
  s->content[0] = '\0';

  status->status = STATUS_OK;
  status->code = STATUS_CODE_NONE;

  return s;
}

/* Function number 2.
 * Check for an error in status and print it.
 * Returns true if an error is detected otherwise false.
 *
 * pseudo code:
 * def Status_pcheck(Status a, file output):
 *   if (a.status == STATUS_ERR):
 *     fprintf(output, "%s in function %s\n", status_lookup[a.err],
 *             func_lookup[a.func])
 *     return True
 *   elif (a.status == STATUS_OK):
 *     return False;
 *   else:
 *     fprintf(output, "unknown error code: %i", a.err)
 *     return True
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
 *
 * pseudo code:
 * def Status_from(char *a):
 *   STRING_ALLOC_SIZE size = strlen(a) # strlen includes the null terminator
 *   String s = String_new(size)
 *   strncpy(s->content, a, size);
 *   s->used = size - 1;
 */
STRING_NAME *STRING_FUNC(from)(const char *a, STATUS_NAME *status) {
  char *ret = NULL;
  String *s = NULL;
  STRING_ALLOC_SIZE size = 0;

  if (status == NULL) {
    return NULL;
  }

  if (a == NULL) {
    status->func = FUNC_STRING_FROM;
    status->status = STATUS_ERR;
    status->code = STATUS_CODE_INVALID_NULL;
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
    status->status = STATUS_ERR;
    status->func = FUNC_STRING_FROM;
    status->code = STATUS_CODE_BROKEN_STRUCT;
    return NULL;
  }

  ret = strcpy(s->content, a);
  if (ret != s->content) {
    status->func = FUNC_STRING_FROM;
    status->status = STATUS_ERR;
    status->code = STATUS_CODE_FAIL_STRCPY;
    STRING_FUNC(free)(s, status);
    return NULL;
  }

  s->used = size;

  status->func = FUNC_STRING_FROM;
  status->status = STATUS_OK;
  status->code = STATUS_CODE_NONE;

  return s;
}

/* Function number 4.
 * Check for an error in status *BUT DON'T print it*.
 *
 * pseudo code:
 * def Status_check(Status a):
 *   if (a.status == STATUS_ERR):
 *     return True
 *   elif (a.status == STATUS_OK):
 *     return False;
 *   else:
 *     fprintf(output, "unknown error code: %i", a.err)
 *     return True
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
 *
 * pseudo code:
 * def Status_check(Status a):
 *   free(a.content)
 *   free(a)
 */
void STRING_FUNC(free)(STRING_NAME *a, STATUS_NAME *status) {
  if (a == NULL) {
    status->func = FUNC_STRING_FREE;
    status->status = STATUS_ERR;
    status->code = STATUS_CODE_INVALID_NULL;
    return;
  }
  if (a->content == NULL) {
    status->func = FUNC_STRING_FREE;
    status->status = STATUS_ERR;
    status->code = STATUS_CODE_BROKEN_STRUCT;
    return;
  }
  free(a->content);
  free(a);
  status->func = FUNC_STRING_FREE;
  status->status = STATUS_OK;
  status->code = STATUS_CODE_NONE;
}

#endif /* ifdef UTILS_H_IMPLEMENTATION */
#endif /* ifndef __UTILS_H__ */
