#include <stdlib.h>
#include <string.h>
#include "tag.h"

struct Tag* Tag__create(char *name, int view_count, int comment_count, int mylist_count) {
    struct Tag *tag = (struct Tag*) malloc(sizeof(struct Tag));

    tag->name = (char*) malloc(sizeof(char) * strlen(name));
    strncpy(tag->name, name, strlen(name));
    tag->view_count    = view_count;
    tag->comment_count = comment_count;
    tag->mylist_count  = mylist_count;

    return tag;
}

void Tag_delete(struct Tag *tag) {
    free(tag->name);
    tag->name = NULL;
    free(tag);
    tag = NULL;
}

