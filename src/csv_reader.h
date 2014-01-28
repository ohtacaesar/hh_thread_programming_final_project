#ifndef CSV_READER_H
#define CSV_READER_H

#include "csv_column.h"

/**
 * CsvReader
 *
 * 今回利用するCSVファイル以外は対応していません
 */

// クラス的メソッド
struct CsvReader* CsvReader__create();

// インスタンス的メソッド
int CsvReader_delete(struct CsvReader *csv_reader);

int CsvReader_open(struct CsvReader *csv_reader, char *file_name);

struct CsvColumn* CsvReader_gets(struct CsvReader *csv_reader);

// プライベート的インスタンス的メソッド
// 書かないとテストできなかった
struct CsvColumn* split_line_to_column_list(char *line);

#endif
