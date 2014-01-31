#include <stdio.h>
#include <stdlib.h>
#include "tag.h"
#include "csv_reader.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Argment required. %s <csv_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct CsvReader *csv_reader = CsvReader__create();
    CsvReader_open(csv_reader, argv[1]);

    struct CsvColumn *csv_column;
    struct Tag *tag;
    while((csv_column = CsvReader_gets(csv_reader)) != NULL) {
        tag = Tag__create_from_CsvColumn(csv_column);
        printf("%s\n", tag->name);

        Tag_delete(tag);
    }

    CsvReader_delete(csv_reader);

    return 0;
}
