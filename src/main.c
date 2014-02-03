#include <stdio.h>
#include <stdlib.h>
#include "files.h"
#include "tag.h"
#include "tag_array.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "Argment required. %s <csv_directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("%s\n", argv[1]);
    struct Files *files = Files__create(argv[1]);
    if(files == NULL) {
        perror("Files__create");
        exit(EXIT_FAILURE);
    }

    int i;
    for(i = 0; i < files->size; i++) {
        printf("%s\n", files->file_paths[i]);
    }

    Files_delete(files);

    return 0;
}
