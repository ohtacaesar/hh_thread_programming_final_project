#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"

#define FGETS_BUF_SIZE 256
#define COLUMN_BUF_SIZE 64

struct CsvReader {
    FILE *file_pointer;
    char *file_name;
};

struct CsvReader* CsvReader__create() {
    struct CsvReader *csv_reader = (struct CsvReader*) malloc(sizeof(struct CsvReader));
    csv_reader->file_pointer = NULL;
    csv_reader->file_name    = NULL;

    return csv_reader;
}

int CsvReader_delete(struct CsvReader *csv_reader) {
    if(csv_reader->file_pointer != NULL) {
        fclose(csv_reader->file_pointer);
        csv_reader->file_pointer = NULL;
    }

    if(csv_reader->file_name != NULL) {
        free(csv_reader->file_name);
        csv_reader->file_name = NULL;
    }

    free(csv_reader);
    csv_reader = NULL;

    return 0;
}

int CsvReader_open(struct CsvReader *csv_reader, char *file_name) {
    // file open failed
    if((csv_reader->file_pointer = fopen(file_name, "r")) == NULL) {
        return -1;
    }

    csv_reader->file_name = (char *) malloc(sizeof(char) * strlen(file_name));
    strcpy(csv_reader->file_name, file_name);

    return 0;
}

struct CsvColumn* CsvReader_gets(struct CsvReader *csv_reader) {
    char line[FGETS_BUF_SIZE];

    if(fgets(line, FGETS_BUF_SIZE, csv_reader->file_pointer) == NULL) {
        return NULL;
    }

    struct CsvColumn *csv_column = split_line_to_column_list(line);

    return csv_column;
}

struct CsvColumn* split_line_to_column_list(char *line) {
    struct CsvColumn *csv_column, *current_csv_column = NULL;

    // CSVのカラムのダブルクオートの中にいるか外にいるか
    int in_double_quote = 0;

    char buf[COLUMN_BUF_SIZE];
    int buf_pos = 0;

    int i;
    int line_size = strlen(line);
    for(i = 0; i < line_size; i++) {
        char c = line[i];

        if(c == '"') {
            if(in_double_quote == 0) {
                in_double_quote = 1;
            } else {
                buf[buf_pos++] = '\0';
                // printf("%s: %d\n", buf, strlen(buf));

                if(csv_column == NULL) {
                    csv_column = CsvColumn__create(buf);
                    current_csv_column = csv_column;
                } else {
                    current_csv_column = CsvColumn_create_next(current_csv_column, buf);
                }

                in_double_quote = 0;
                buf_pos = 0;
            }
        } else {
            if(in_double_quote == 1) {
                buf[buf_pos++] = c;
            }
        }
    }

    return csv_column;
}

