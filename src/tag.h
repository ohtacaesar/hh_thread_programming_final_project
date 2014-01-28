#ifndef TAG_H
#define TAG_H

struct Tag {
    char *name;
    int view_count;
    int comment_count;
    int mylist_count;
};

struct Tag* Tag__create(char *name, int view_count, int comment_count, int mylist_count);

#endif
