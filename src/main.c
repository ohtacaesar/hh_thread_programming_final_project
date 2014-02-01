#include <stdio.h>
#include <stdlib.h>
#include "tag.h"
#include "tag_array.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "Argment required. %s <csv_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct TagArray *tag_array = TagArray__create_from_csv(argv[1]);
    if(tag_array == NULL) {
        perror("TagArray__create_from_csv");
        exit(EXIT_FAILURE);
    }

    int i;
    for(i = 0; i < tag_array->size; i++) {
        printf("%06d: %s\n", i, tag_array->array[i]->name);
    }

    TagArray_delete(tag_array);

    return 0;
}
