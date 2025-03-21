#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hashmap.h"
#include "jenkins.h"

#ifdef HMAP_DEBUG
  #define DPRINTF(x)  do { printf x; } while (0)
#else
  #define DPRINTF(x)  do {} while (0)
#endif

unsigned int free_slots = MAX_ARRAY_SIZE;
unsigned int insert_collisions = 0;
bool key_found = false;

unsigned int get_key_index(hashmap hmap, char *key, bool *found) {
  unsigned int index = jenkins_hash(key) % MAX_ARRAY_SIZE;

  HashMapObj *element = hmap[index];

  key_found = false;

  if (element != NULL) {

    DPRINTF(("Something found at index %d\n", index));
    DPRINTF(("Testing for key match: %s = %s\n", element->key, key));

    if (strcmp(element->key, key) == 0) {
      *found = true;
      return index;
    } else {
      DPRINTF(("Collision, searching for match..."));
      for (int i=index+1;; i++) {
        element = hmap[i];
        if (strcmp(element->key, key) == 0) {
          *found = true;
          return index;
        }
        if (i == index) {
          *found = false;
          return MAX_ARRAY_SIZE + 1;
        }
      }
    }
  }

  *found = false;
  return MAX_ARRAY_SIZE + 1;
}

hashmap hashmap_create(void) {
  return calloc(MAX_ARRAY_SIZE, sizeof(HashMapObj *));
}

HashMapObj *new_hashmap_obj(const char *key, void *value, char *metadata) {
  HashMapObj *hashmapobj = (HashMapObj *)malloc(sizeof(HashMapObj));

  hashmapobj->key = key;
  hashmapobj->data = value;
  hashmapobj->metadata = metadata;

  return hashmapobj;
}

unsigned int hashmap_insert(hashmap h, HashMapObj *element) {
  unsigned int index = jenkins_hash(element->key) % MAX_ARRAY_SIZE;

  int i = -1;
  bool collision = false;
  bool same_key = false;

  if (free_slots == 0) {
    fprintf(stderr, "Cannot insert element with key %s, no free slots available.", element->key);
    return 2;
  }

  if (h[index] != NULL) {
    DPRINTF(("Collision while inserting element with key %s at %d\n", element->key, index));
    collision = true;
  }

  if (collision) {
    DPRINTF(("Testing key match\n"));

    if (strcmp(element->key, h[index]->key) == 0) {
      same_key = true;
    }
  }

  if ((collision) && !(same_key)) {
  DPRINTF(("Searching for available slot..."));
    for (i = index + 1 ;; i++) {
      insert_collisions++;
      if (i > LAST_INDEX) {
        i = 0;
      }

      if (i == index) {
        fprintf(stderr, "Error: could not insert element");
        return 1;
      }

      if (h[i] == NULL) {
        DPRINTF(("Found a slot at %d, was %d\n", i, index));
        index = i;
        break;
      }
    }
  }

  h[index] = element;

  DPRINTF(("Inserted element at index %d with key %s\n", index, h[index]->key));

  free_slots--;

  return 0;
}

HashMapObj *get_hashmap_element(hashmap h, const char *key) {
  unsigned int index = jenkins_hash(key) % MAX_ARRAY_SIZE;

  HashMapObj *element = h[index];

  if (element != NULL) {
    DPRINTF(("Something found at index %d\n", index));
    DPRINTF(("Testing for key match: %s = %s\n", element->key, key));

    if (strcmp(element->key, key) == 0)
      return element;
    else {
      DPRINTF(("Collision, searching for match...\n"));

      for (unsigned int i = index+1 ;; i++) {
        element = h[i];
        if (strcmp(element->key, key) == 0) {
          return element;
        }
        if ((element == NULL) || (i == index)) {
          return NULL;
        }
      }
    }
  }

  return NULL;
}

bool remove_hashmap_element(hashmap hmap, char *key, bool *found) {
  unsigned int index = get_key_index(hmap, key, found);

  if (! *found) {
    DPRINTF(("Element with key %s not found\n", key));

    return false;
  }

  free(hmap[index]);
  hmap[index] = NULL;
  free_slots++;
  DPRINTF(("Element removed"));
  return true;
}
