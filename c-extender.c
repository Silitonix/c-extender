#ifndef __HELPER
#define __HELPER

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define return_value_if(value, condition)                                      \
  if (condition)                                                               \
    return value;

#define return_if(condition)                                                   \
  if (condition)                                                               \
    return;

#define tif(condition) return_value_if(true, condition)
#define fif(condition) return_value_if(false, condition)
#define nif(condition) return_value_if(NULL, condition)

#define new(class, name, ...)                                                  \
  s_##class *name = (s_##class *)malloc(sizeof(s_##class));                    \
  s_##class##_new(name, __VA_ARGS__);                                          \
  trash_can_register(name)

#define new_static(class, name, ...)                                           \
  s_##class name;                                                              \
  s_##class##_new(&name, __VA_ARGS__);

#define new_static_np(class, name)                                             \
  s_##class name;                                                              \
  s_##class##_new(&name);

#define class(name, property, parameter, constructor)                          \
  typedef struct s_##name {                                                    \
    property                                                                   \
  } s_##name;                                                                  \
  void s_##name##_new parameter constructor

#define loop while (true)

void trash_can_register(void *data);

class(list, struct s_list_item *start; struct s_list_item * end; int count;
      , (s_list * list), // constructor
      { list->count = 0; });

class(list_item,

      struct s_list_item *prev;
      struct s_list_item * next; void *value;

      , (s_list_item * list, s_list_item *prev), {
        return_if(prev == NULL);
        list->prev = prev;
        prev->next = list;
      });

void s_list_push(s_list *list, void *new) {
  new (list_item, new_item, list->end);
  if (list->start == NULL) {
    list->start = new_item;
  }

  list->end = new_item;
  list->count++;
}

s_list trash_can = {NULL, NULL, 0};

void trash_can_empty() {
  return_if(trash_can.start == NULL);
  s_list_item *item_start = trash_can.start;
  s_list_item *item_temp;

  while (true) {
    free(item_start->value);

    if (item_start->next == NULL) {
      free(item_start);
      break;
    }

    item_temp = item_start->next;
    free(item_start);
    item_start = item_temp;
  }
}
void trash_can_register(void *data) { s_list_push(&trash_can, data); }

int main(int argc, char **argv) {

#include "main.c"

  trash_can_empty();
  signal(SIGINT, trash_can_empty);
  return 0;
}

#endif
