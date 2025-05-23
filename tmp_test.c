#include "du_portable_types.h"
#include "du_string.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int
main(void)
{
  du_str_res a = { 0 };
  du_str* b = NULL;

  a = du_str_new(10);
  if (a.stat != DU_OK) {
    printf("%s\n", a.ret.err_msg);
  } else {
    b = a.ret.val;

    assert(b->cap == 11);
    assert(b->len == 0);
    assert(b->cont[0] == '\0');

    du_str_free(b);
  }


  a = du_str_from("Hello, world!");
  if (a.stat != DU_OK) {
    printf("%s\n", a.ret.err_msg);
  } else {
    b = a.ret.val;

    assert(b->cap == 14);
    assert(b->len == 13);
    assert(b->cont[0] == 'H');

    a = du_str_ext_to(b, 20);

    if (a.stat != DU_OK) {
      printf("%s\n", a.ret.err_msg);
    } else {
      b = a.ret.val;

      assert(b->cap == 21);
      assert(b->len == 13);
      assert(b->cont[0] == 'H');
    }

    du_str_free(b);
  }

  a = du_str_from("Hello, ");
  if (a.stat != DU_OK) {
    printf("%s\n", a.ret.err_msg);
  } else {
    b = a.ret.val;

    a = du_str_cat(b, "world!", 6);
    if (a.stat != DU_OK) {
      printf("%s\n", a.ret.err_msg);
    } else {
      b = a.ret.val;

      assert(b->cap == 14);
      assert(b->len == 13);
      assert(b->cont[0] == 'H');
      assert(strcmp(b->cont, "Hello, world!") == 0);
    }

    du_str_free(b);
  }


  a = du_str_from("Hello, world!");
  if (a.stat != DU_OK) {
    printf("%s\n", a.ret.err_msg);
  } else {
    b = a.ret.val;

    du_str_trim_trail(b, 1);
    assert(strcmp(b->cont, "Hello, world") == 0);

    du_str_trim_trail(b, 6);
    assert(strcmp(b->cont, "Hello,") == 0);

    du_str_free(b);
  }

  a = du_str_from("Hello, world!");
  if (a.stat != DU_OK) {
    printf("%s\n", a.ret.err_msg);
  } else {
    b = a.ret.val;

    du_str_trim_lead(b, 1);
    assert(strcmp(b->cont, "ello, world!") == 0);

    du_str_trim_lead(b, 6);
    assert(strcmp(b->cont, "world!") == 0);

    du_str_free(b);
  }

  a = du_str_from("Hello, world!");
  if (a.stat != DU_OK) {
    printf("%s\n", a.ret.err_msg);
  } else {
    du_u32_opt_res k = { 0 };
    du_u32_opt l = { false, 0 };
    du_u32 p = 0;
    b = a.ret.val;

    k = du_str_find(b, 0, "world", 5);
    if (k.stat != DU_OK) {
      printf("%s\n", k.ret.err_msg);
      return 0;
    }
    l = k.ret.val;
    if (l.stat) {
      p = l.val;
      printf("word found at position %d (good)\n", p);
      assert(p == 7);
    } else {
      printf("word not found! (bad)\n");
    }

    k = du_str_find(b, 0, "wasd", 4);
    if (k.stat != DU_OK) {
      printf("%s\n", k.ret.err_msg);
      return 0;
    }
    l = k.ret.val;
    if (l.stat) {
      p = l.val;
      printf("word found at position %d (bad)\n", p);
    } else {
      printf("word not found! (good)\n");
    }

    du_str_free(b);
  }


  return 0;
}
