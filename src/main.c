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
    while((csv_column = CsvReader_gets(csv_reader)) != NULL) {
        printf("%s\n", csv_column->value);

        // 現在メモリりーくちゅう!!
        // 1. CsvReaderにLastCsvColumnもたせて、getsが呼び出されるたびにdeleteするのがよし？
        // 2. それかここでCsvColumn_deleteをよばせる
    }

    CsvReader_delete(csv_reader);

    return 0;
}
