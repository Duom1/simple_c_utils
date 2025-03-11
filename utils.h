#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* string settings */
#define STRING_NAME String
#define STRING_ALLOC_SIZE uint32_t

/* status settings */
#define STATUS_NAME Status
#define STATUS_CODE_SIZE uint8_t
#define STATUS_ERR_SIZE STATUS_CODE_SIZE
#define STATUS_FUNCP_SIZE uint16_t
enum STATUS_STATUS {
  STATUS_OK,
  STATUS_ERR,
};
enum STATUS_CODE {
  STATUS_CODE_NONE,
  STATUS_CODE_FAIL_ALLOC,
  STATUS_CODE_FAIL_REALLOC,
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
  STATUS_ERR_SIZE err;
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

#ifdef UTILS_H_IMPLEMENTATION

/* function number 1
 * Creates a new string and allocates x bytes for it */
STRING_NAME *STRING_NAME_new(STRING_ALLOC_SIZE x) {
  STRING_NAME *s = malloc(sizeof(STRING_NAME));
  s->content = malloc(sizeof(char) * x);
  s->used = 0;
  s->content[0] = '\0';
}

#endif /* ifdef UTILS_H_IMPLEMENTATION */
#endif /* ifndef __UTILS_H__ */
