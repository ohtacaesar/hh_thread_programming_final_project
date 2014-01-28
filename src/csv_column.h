#ifndef CSV_COLUMN_H
#define CSV_COLUMN_H

/**
 * CsvColumn
 * 
 * Csvのレコード1行を格納するLinkedList
 */
struct CsvColumn {
    char   *value;
    struct CsvColumn *next;
};

// クラス的メソッド
struct CsvColumn* CsvColumn__create(char *value);

// インスタンス的メソッド
int CsvColumn_delete(struct CsvColumn *csv_column);

struct CsvColumn* CsvColumn_create_next(struct CsvColumn *csv_column, char *value);

#endif
