#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashmap.h"

const char key_prompt[5] = "key:";

void read_line(const char *prompt, char *buffer, int psize) {
  printf("%s ", prompt);
  fgets(buffer, psize, stdin);
  buffer[strlen(buffer) - 1] = '\0';
}

void insert(hashmap hmap) {
  char *key = calloc(32, sizeof(char));
  char *value = calloc(32, sizeof(char));

  const char vp[7] = "value:";

  HashMapObj *element;

  puts("-- Insert new element");
  read_line(key_prompt, key, 32);
  read_line(vp, value, 32);

  element = new_hashmap_obj(key, value, "string type");
  hashmap_insert(hmap, element);
}

void search(hashmap hmap) {
  char key[32];
  char value[32];

  HashMapObj *element;
  puts("-- Search element");
  read_line(key_prompt, key, 32);
  element = get_hashmap_element(hmap, key);

  if (element == NULL) {
      puts("");
      printf("Key <%s> not found", key);
      puts("");
      return;
  }

  puts("");
  printf("Value is: %s\n", (char *)element->data);
  puts("");
}

void delete(hashmap hmap) {
  char key[32];
  bool found;

  HashMapObj *element;

  puts("-- Delete element");
  read_line(key_prompt, key, 32);
  remove_hashmap_element(hmap, key, &found);
}

int main(void) {
  hashmap hmap = hashmap_create();
  char command[32];
  char prompt[3] = "> ";

  puts("");
  printf("Hashmap created, %d slots available\n", MAX_ARRAY_SIZE);

  do {
    puts("");
    puts("Available commands: insert, delete, search, quit");
    puts("");

    read_line(prompt, command, 32);

    if (strcmp("insert", command) == 0) {
      insert(hmap);
    }

    if (strcmp("search", command) == 0) {
      search(hmap);
    }

    if (strcmp("delete", command) == 0) {
      delete(hmap);
    }

  } while (strcmp(command, "quit") != 0);
  puts("Terminated");
}
