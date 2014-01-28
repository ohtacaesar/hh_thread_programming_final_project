#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"

#include "../src/tag.h"
#include "../src/csv_reader.h"

int   tests_run = 0;
char* test_data_path;
char* wrong_test_data_path; 


static char *test_Tag__create_and_delete() {
    struct Tag *tag = Tag__create("test_tag", 1, 2, 3);

    mu_assert("ERROR, tag->name != \"test_tag\"", strcmp(tag->name, "test_tag") == 0);
    mu_assert("ERROR, tag->view_count != 1",      tag->view_count    == 1); 
    mu_assert("ERROR, tag->comment_count != 2",   tag->comment_count == 2); 
    mu_assert("ERROR, tag->mylist_count != 3",    tag->mylist_count  == 3); 

    // char *tag_name = $tag->name;
    char **tag_name_address = &tag->name;
    Tag_delete(tag);

    // TODO: Tag_deleteでNULLを代入してるけど通らない、調べる
    // 結論: これやりたいならポインタのアドレス渡せるようにしないとだめかも
    // int Tag_delete(struct Tag **tag_address);
    // Tag_delete(&tag);
    // なんかしっくりこないかも？
    //  
    // mu_assert("ERROR, tag_name != NULL", tag_name ==  NULL);
    // 下ならいけた (たまたまいけてるっぽい）
    mu_assert("ERROR, tag_name != NULL", *tag_name_address == NULL);
    // mu_assert("ERROR, tag != NULL",      tag ==NULL);

    return 0;
}

static char *test_CsvReader__create_and_delete() {
    struct CsvReader *csv_reader = CsvReader__create();
    CsvReader_delete(csv_reader);
    
    return 0;
}

static char *test_CsvReader_open() {
    struct CsvReader *csv_reader = CsvReader__create();
    int status = CsvReader_open(csv_reader, test_data_path);
    mu_assert("ERROR, test_data.csv does not exists", status == 0);

    CsvReader_delete(csv_reader);

    return 0;
}

static char *test_split_line_to_column_list() {
    char *line = "\"３部OVA_MAD\",\"19602\",\"357\",\"118\"";

    struct CsvColumn *csv_column = split_line_to_column_list(line);
    mu_assert("ERROR, csv_column->value != \"３部OVA_MAD\"", strcmp(csv_column->value, "３部OVA_MAD") == 0);

    csv_column = csv_column->next;
    mu_assert("ERROR, csv_column->value != \"19602\"", strcmp(csv_column->value, "19602") == 0);

    csv_column = csv_column->next;
    mu_assert("ERROR, csv_column->value != \"357\"", strcmp(csv_column->value, "357") == 0);

    csv_column = csv_column->next;
    mu_assert("ERROR, csv_column->value != \"118\"", strcmp(csv_column->value, "118") == 0);

    // mu_assert("ERROR, csv_column->next != NULL", (csv_column->next == NULL));

    return 0;
}

static char *test_CsvReader_gets() {
    struct CsvReader *csv_reader = CsvReader__create();
    if(CsvReader_open(csv_reader, test_data_path) < 0) {
        mu_assert("ERROR, CsvReader_open failed", NULL);
    }

    struct CsvColumn *csv_column = CsvReader_gets(csv_reader);
    mu_assert("ERROR, csv_column != \"k-pop\"", strcmp(csv_column->value, "k-pop") == 0);

    CsvReader_delete(csv_reader);

    return 0;
}

static char *test_CsvColumn__create() {
    struct CsvColumn *csv_column = CsvColumn__create("test_column");
    mu_assert("ERROR, csv_column->value != \"test_column\"", strcmp(csv_column->value, "test_column") == 0);

    CsvColumn_delete(csv_column);

    return 0;
}

static char *test_CsvColumn_delete() {
    struct CsvColumn *csv_column_1 = CsvColumn__create("1");
    struct CsvColumn *csv_column_2 = CsvColumn__create("2");
    struct CsvColumn *csv_column_3 = CsvColumn__create("3");

    csv_column_1->next = csv_column_2;
    csv_column_2->next = csv_column_3;

    CsvColumn_delete(csv_column_1);
    // TODO: どうやってfreeが完了していることを確認するか
    //
    // printf("%p (%s)\n", csv_column_1_value_address, __FILE__);
    // mu_assert("ERROR, csv_column_1->value != NULL", *csv_column_1_value_address == NULL);
    // mu_assert("ERROR, csv_column_2->value != NULL", *csv_column_2_value_address == NULL);
    // mu_assert("ERROR, csv_column_3->value != NULL", *csv_column_3_value_address == NULL);

    return 0;
}

static char *test_CsvColumn_create_next() {
    struct CsvColumn *csv_column      = CsvColumn__create("1");
    struct CsvColumn *next_csv_column = CsvColumn_create_next(csv_column, "2");

    mu_assert("ERROR, next tag is not created", csv_column->next != NULL);
    mu_assert("ERROR, csv_column->next->value != \"2\"", strcmp(next_csv_column->value, "2") == 0);
    mu_assert("ERROR, csv_column->next->value != \"2\"", strcmp(csv_column->next->value, "2") == 0);

    CsvColumn_delete(csv_column);

    return 0;
}

static char *all_tests() {
    // Tag
    mu_run_test(test_Tag__create_and_delete);

    // CsvReader
    mu_run_test(test_CsvReader__create_and_delete);
    mu_run_test(test_CsvReader_open);
    mu_run_test(test_split_line_to_column_list);
    mu_run_test(test_CsvReader_gets);

    // CsvColumn
    mu_run_test(test_CsvColumn__create);
    mu_run_test(test_CsvColumn_delete);
    mu_run_test(test_CsvColumn_create_next);

    return 0;
}

int main(int argc, char** argv) {

    // Get path to test/test_dataa.csv
    // C mendoi
    char *file_name = "/test_data.csv";
    int file_path_size = strlen(__FILE__) + strlen(file_name);

    test_data_path = (char *) malloc(sizeof(char) * file_path_size);
    strncpy(test_data_path, __FILE__, strlen(__FILE__));

    char *pos = strrchr(test_data_path, '/');
    *pos = '\0';

    strncat(test_data_path, file_name, file_path_size);
    // printf("Test data file is %s\n", test_data_path);

    // Execute Tests
    char *result = all_tests();

    if(result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }

    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
