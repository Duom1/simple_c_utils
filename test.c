#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UTILS_H_IMPLEMENTATION
#include "utils.h"

/* colors from:
 * https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a */

#define ESC "\x1B"
#define GRN ESC "[0;32m"
#define RED ESC "[0;31m"
#define CRESET ESC "[0m"

const char *lorem_ipsum =
    "Lorem ipsum odor amet, consectetuer adipiscing elit. Platea dapibus nunc "
    "viverra mi feugiat, dui et neque. Dictum taciti penatibus porttitor "
    "imperdiet nullam himenaeos quis sem eget. Quisque varius turpis est "
    "habitasse consequat. Inceptos tellus aliquet parturient libero sapien "
    "orci tempor sapien. Vitae dolor ultrices nunc euismod venenatis metus "
    "per. Quam commodo feugiat nam aptent ut mauris. Eros a suspendisse "
    "sagittis fermentum metus varius egestas odio. In rutrum ligula dapibus ";

bool test_string(STRING_NAME *s, int64_t allocated, int64_t used,
                 const char *content) {
  if (allocated != -1) {
    if (s->allocated != allocated) {
      fprintf(stderr, "allocation sizes do not match\n");
      return false;
    }
  }
  if (used != -1) {
    if (s->used != used) {
      fprintf(stderr, "used sizes do not match\n");
      return false;
    }
  }
  if (content != NULL) {
    if (strcmp(s->content, content) != 0) {
      fprintf(stderr, "contents do not match\n");
      return false;
    }
  }
  return true;
}

int main(void) {
  unsigned test = 1;
  bool test_status = false;
  STRING_NAME *s = NULL;
  STATUS_NAME status;
  status.status = STATUS_ERR;
  status.code = STATUS_CODE_NONE;
  status.func = FUNC_NONE;

  /* ======================================= */
  /* = String_new() tests                  = */
  /* ======================================= */

  s = STRING_FUNC(new)(10, &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status)
    test_status = test_string(s, 10, 1, NULL);
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = STRING_FUNC(new)(0, &status);
  test_status = STATUS_FUNC(check)(&status); /* this should fail */
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = STRING_FUNC(new)(1, &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status)
    test_status = test_string(s, 1, 1, NULL);
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = STRING_FUNC(new)(999999, &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status)
    test_status = test_string(s, 999999, 1, NULL);
  STRING_FUNC(free)(s, &status);
  s = NULL;

  if (test_status)
    fprintf(stdout, GRN "%u: Passed String_new() tests\n" CRESET, test);
  else
    fprintf(stdout, "%u: Failed String_new() tests\n", test);
  fflush(stdout);

  ++test;

  /* ======================================= */
  /* ======================================= */

  /* ======================================= */
  /* = String_from() tests                  = */
  /* ======================================= */

  s = STRING_FUNC(from)("hello world", &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status)
    test_status = test_string(s, 12, 12, "hello world");
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = STRING_FUNC(from)(" ", &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status)
    test_status = test_string(s, 2, 2, " ");
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = STRING_FUNC(from)("", &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status)
    test_status = test_string(s, 1, 1, "");
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = STRING_FUNC(from)(lorem_ipsum, &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status)
    test_status = test_string(s, -1, -1, lorem_ipsum);
  STRING_FUNC(free)(s, &status);
  s = NULL;

  if (test_status)
    fprintf(stdout, GRN "%u: Passed String_from() tests\n" CRESET, test);
  else
    fprintf(stdout, "%u: Failed String_from() tests\n", test);
  fflush(stdout);

  ++test;

  /* ======================================= */
  /* ======================================= */

  /* ======================================= */
  /* = String_expand_by() tests            = */
  /* ======================================= */

  s = STRING_FUNC(from)("hello world", &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status) {
    s = STRING_FUNC(expand_by)(s, 10, &status);
    test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
    if (test_status)
      test_status = test_string(s, 22, 12, "hello world");
  }
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = STRING_FUNC(from)("hello world", &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status) {
    s = STRING_FUNC(expand_by)(s, 0, &status);
    test_status = STATUS_FUNC(check)(&status); /* this is expected to fail */
    if (test_status)
      test_status = test_string(s, 12, 12, "hello world");
  }
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = STRING_FUNC(from)("hello world", &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status) {
    s = STRING_FUNC(expand_by)(s, 999999, &status);
    test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
    if (test_status)
      test_status = test_string(s, 1000011, 12, "hello world");
  }
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = NULL;
  s = STRING_FUNC(expand_by)(s, 999999, &status);
  test_status = STATUS_FUNC(check)(&status);
  s = NULL;

  if (test_status)
    fprintf(stdout, GRN "%u: Passed String_expand_by() tests\n" CRESET, test);
  else
    fprintf(stdout, "%u: Failed String_expand_by() tests\n", test);
  fflush(stdout);

  ++test;

  /* ======================================= */
  /* ======================================= */

  /* ======================================= */
  /* = String_lead_trunc() tests           = */
  /* ======================================= */

  s = STRING_FUNC(from)("hello world", &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status) {
    s = STRING_FUNC(lead_trunc)(s, 6, &status);
    test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
    if (test_status)
      test_status = test_string(s, 12, 6, "world");
  }
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = STRING_FUNC(from)("ab", &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status) {
    s = STRING_FUNC(lead_trunc)(s, 1, &status);
    test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
    if (test_status)
      test_status = test_string(s, 3, 2, "b");
  }
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = STRING_FUNC(from)("asdcco", &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status) {
    s = STRING_FUNC(lead_trunc)(s, 0, &status);
    test_status = STATUS_FUNC(check)(&status);
  }
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = STRING_FUNC(lead_trunc)(NULL, 0, &status);
  test_status = STATUS_FUNC(check)(&status);

  s = STRING_FUNC(from)(lorem_ipsum, &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status) {
    s = STRING_FUNC(lead_trunc)(s, 40, &status);
    test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
    if (test_status)
      /* plus one for the null tyerminator */
      test_status = test_string(s, -1, strlen(lorem_ipsum) + 1 - 40, NULL);
  }
  STRING_FUNC(free)(s, &status);
  s = NULL;

  if (test_status)
    fprintf(stdout, GRN "%u: Passed String_lead_trunc() tests\n" CRESET, test);
  else
    fprintf(stdout, "%u: Failed String_lead_trunc() tests\n", test);
  fflush(stdout);

  ++test;

  /* ======================================= */
  /* ======================================= */

  /* ======================================= */
  /* = String_trunc() tests                = */
  /* ======================================= */

  s = STRING_FUNC(from)("hello world", &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status) {
    s = STRING_FUNC(trunc)(s, 6, &status);
    test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
    if (test_status)
      test_status = test_string(s, 12, 6, "hello");
  }
  STRING_FUNC(free)(s, &status);
  s = NULL;

  s = STRING_FUNC(from)(lorem_ipsum, &status);
  test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
  if (test_status) {
    s = STRING_FUNC(trunc)(s, 55, &status);
    test_status = !(STATUS_FUNC(pcheck)(&status, stderr));
    if (test_status)
      test_status = test_string(s, -1, strlen(lorem_ipsum) + 1 - 55, NULL);
  }
  STRING_FUNC(free)(s, &status);
  s = NULL;

  if (test_status)
    fprintf(stdout, GRN "%u: Passed String_trunc() tests\n" CRESET, test);
  else
    fprintf(stdout, "%u: Failed String_trunc() tests\n", test);
  fflush(stdout);

  ++test;

  /* ======================================= */
  /* ======================================= */

  return EXIT_SUCCESS;
}
