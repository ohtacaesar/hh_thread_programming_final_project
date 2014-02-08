#include <stdio.h>
#include <stdlib.h>
#include "files.h"
#include "tag.h"
#include "tag_array.h"

int main(int argc, char** argv) {
    // 引数の確認
    if(argc < 2) {
        fprintf(stderr, "Argment required. %s <csv_or_directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // ファイルの取得
    struct Files *files = Files__create(argv[1]);
    if(files == NULL) {
        perror("Files__create");
        exit(EXIT_FAILURE);
    }

    int i;
    for(i = 0; i < files->size; i++) {
        printf("%s\n", files->file_paths[i]);

        struct TagArray *tag_array = TagArray__create_from_csv(files->file_paths[i]);
        if(tag_array == NULL) {
            fprintf(stderr, "failed to create tag_array\n");
            break;
        }

        int j;
        for(j = 0; j < tag_array->size; j++) {
            // printf("%s\n", tag_->array[j]->name);
        }
        TagArray_delete(tag_array);
    }

    Files_delete(files);

    return 0;
}
