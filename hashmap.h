#include <stdbool.h>

#define MAX_ARRAY_SIZE 33554432
//#define MAX_ARRAY_SIZE 3
#define LAST_INDEX MAX_ARRAY_SIZE - 1
#define HMAP_DEBUG

typedef unsigned short int type;

typedef struct HMO {
  const char *key;
  void *data;
  char *metadata;
} HashMapObj;

typedef HashMapObj **hashmap;

hashmap hashmap_create(void);

unsigned int hashmap_insert(hashmap, HashMapObj *);

unsigned int get_key_index(hashmap, char *, bool *);

HashMapObj *new_hashmap_obj(const char *, void *, char *);

HashMapObj *get_hashmap_element(hashmap h, const char *key);

bool remove_hashmap_element(hashmap, char *, bool *);
