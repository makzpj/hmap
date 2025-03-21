#include <stdio.h>
#include <stdint.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <time.h>
#include "hashmap.h"

#define KEY_LENGTH 8
#define VALUE_LENGTH KEY_LENGTH * 2

extern unsigned int free_slots;
extern unsigned int insert_collisions;

void generateRandomString(char *str, int length) {
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < length; i++) {
        int randomIndex = rand() % (sizeof(charset) - 1);
        str[i] = charset[randomIndex];
    }
    str[length] = '\0';
}

void insert_random_keys_and_values(hashmap hmap, int how_many) {
  HashMapObj *element;

  char *randomKey;
  char *randomValue;

  // Seed the random number generator
  srand(time(NULL));
    for (int i=0; i < how_many; i++) {
      randomKey = calloc(KEY_LENGTH + 1, sizeof(char));
      randomValue = calloc(KEY_LENGTH * 2, sizeof(char));
      // Generate the random string
      generateRandomString(randomKey, KEY_LENGTH);
      generateRandomString(randomValue, KEY_LENGTH * 2);

      element = new_hashmap_obj(randomKey, randomValue, "string type");
      hashmap_insert(hmap, element);
    }

    printf("Inserted %d elements\n", how_many);
    printf("Free slots: %d\n", free_slots);
}

void get_keys(hashmap hmap) {
  HashMapObj *element;
  unsigned int elements_found = 0;

  for (int i=0; i < MAX_ARRAY_SIZE; i++) {
    element = hmap[i];
    if (element != NULL) {
      elements_found++;
      #ifdef DEBUG
      printf("Found key %s at %d\n", (char *)element->key, i);

      if (strcmp(element->metadata, "string type") == 0) {
        puts("Type is string");
        printf("Value is %s\n", (char *)element->data);
      }
      #endif
    }
  }

  printf("Found %d elements \n", elements_found);
}

int main(int argc, char **argv) {
  const char *k = "date";
  int v = 2024;
  int v2 = 2025;
  long int how_many;

  const char *k2 = "name";
  const char *s = "Hari Seldon";

  char *endptr;

  bool key_found = false;

  if (argc == 2) {
    how_many = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0') {
      puts("Invalid number");
      return 1;
    }
  }
  else {
    how_many = 10;
  }

  printf("Array size is: %d\n", MAX_ARRAY_SIZE);

  puts("--- Single insert test ---");

  hashmap hmap = hashmap_create();

  HashMapObj *element = new_hashmap_obj(k, &v, "int type");
  HashMapObj *p_element;

  hashmap_insert(hmap, element);

  puts("");
  puts("--- Single get test ---");

  puts("Get element with key date...");

  p_element = get_hashmap_element(hmap, "date");

  if (p_element == NULL) {
    puts("Not found");
  } else {
    printf("Value is %d\n", *(int *)p_element->data);
  }

  puts("Get element with key name...");

  p_element = get_hashmap_element(hmap, "name");

  if (p_element == NULL) {
    puts("Not found");
  } else {
    printf("Value is %s\n", (char *)p_element->data);
  }

  puts("");
  puts("--- Insert random keys and values test ---");

  insert_random_keys_and_values(hmap, how_many);

  puts("");
  puts("--- Sequential get keys test ---");
  get_keys(hmap);

  printf("Free slots: %d\n", free_slots);
  printf("Collisions: %d\n", insert_collisions);

  printf("Collision rate: %.2f%%\n", ((float)insert_collisions/(float)MAX_ARRAY_SIZE)*100);

  puts("");
  puts("--- Replace Test ---");
  element = new_hashmap_obj("date", &v2, "replaced date");
  hashmap_insert(hmap, element);
  element = get_hashmap_element(hmap, "date");

  if (element != NULL) {
    printf("Key %s found, value is %d\n", "date", *(int *)element->data);
  }

  puts("");
  puts("--- Remove test ---");

  if(!remove_hashmap_element(hmap, "date", &key_found)) {
    puts("Key not found");
  } else {
    puts("Key found during removal");
  }

  puts("Get element with key date...");

  element = get_hashmap_element(hmap, "date");

  if (p_element == NULL) {
    puts("Not found");
  } else {
    printf("Value is %d\n", *(int *)p_element->data);
  }

  printf("Free slots: %d\n", free_slots);

}
