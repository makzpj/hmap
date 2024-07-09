#include <string.h>

unsigned int jenkins_hash(char *data) {
    size_t length = strlen(data);
    unsigned int hash = 0;
    size_t i;

    for (i = 0; i < length; ++i) {
        hash += data[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}
