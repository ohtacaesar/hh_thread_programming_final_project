#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_column.h"

struct CsvColumn* CsvColumn__create(char *value) {
    struct CsvColumn *csv_column = (struct CsvColumn*) malloc(sizeof(struct CsvColumn));

    csv_column->value = (char *) malloc(sizeof(char) * strlen(value));
    strncpy(csv_column->value, value, strlen(value));
    csv_column->next = NULL;

    return csv_column;
}

int CsvColumn_delete(struct CsvColumn *csv_column) {
    while(csv_column != NULL) {
        struct CsvColumn *prev = csv_column;
        csv_column = csv_column->next;

        free(prev->value);
        prev->value = NULL;
        prev->next  = NULL;

        free(prev);
        prev = NULL;
    }

    return 0;
}

struct CsvColumn* CsvColumn_create_next(struct CsvColumn *csv_column, char* value) {
    struct CsvColumn *next_csv_column = CsvColumn__create(value);
    csv_column->next = next_csv_column;

    return next_csv_column;
}

