#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"

#include "../src/tag.h"
#include "../src/tag_list.h"
#include "../src/tag_array.h"
#include "../src/csv_reader.h"
#include "../src/files.h"

int  tests_run = 0;
char *bin_dir;
char *test_data_path;
char *dir_for_test_path;

/**
 * Tag
 */
static char *test_Tag__create_and_Tag_delete() {
    struct Tag *tag = Tag__create("test_tag", 1, 2, 3);

    mu_assert("ERROR, tag->name != \"test_tag\"", 
            strcmp(tag->name, "test_tag") == 0);
    mu_assert("ERROR, tag->view_count != 1",    tag->view_count    == 1); 
    mu_assert("ERROR, tag->comment_count != 2", tag->comment_count == 2); 
    mu_assert("ERROR, tag->mylist_count != 3",  tag->mylist_count  == 3); 

    // char *tag_name = $tag->name;
    // char **tag_name_address = &tag->name;
    Tag_delete(tag);

    // TODO: Tag_deleteでNULLを代入してるけど通らない、調べる
    // 結論: これやりたいならポインタのアドレス渡せるようにしないとだめかも
    // int Tag_delete(struct Tag **tag_address);
    // Tag_delete(&tag);
    // なんかしっくりこないかも？
    //  
    // mu_assert("ERROR, tag_name != NULL", tag_name ==  NULL);
    // 下ならいけた (たまたまいけてるっぽい）
    // mu_assert("ERROR, tag_name != NULL", *tag_name_address == NULL);

    return 0;
}

static char *test_Tag__create_from_CsvColumn() {
    struct CsvColumn *csv_column = 
        split_line_to_column_list("\"３部OVA_MAD\",\"19602\",\"357\",\"118\"");
    struct Tag *tag = Tag__create_from_CsvColumn(csv_column);

    mu_assert("ERROR, tag->name != \"３部OVA_MAD\"", 
            strcmp(tag->name, "３部OVA_MAD") == 0);
    mu_assert("ERROR, tag->view_count != 19602",  tag->view_count    == 19602);
    mu_assert("ERROR, tag->comment_count != 357", tag->comment_count == 357);
    mu_assert("ERROR, tag->mylist_count != 118",  tag->mylist_count  == 118);

    CsvColumn_delete(csv_column);
    Tag_delete(tag);

    // return NULL if csv_column is wrong
    csv_column = CsvColumn__create("test");
    tag = Tag__create_from_CsvColumn(csv_column);
    mu_assert("ERROR, tag != NULL", tag == NULL);

    CsvColumn_delete(csv_column);
    Tag_delete(tag);

    return 0;
}

/**
 * TagList
 */
static char *test_TagList__create() {
    struct Tag *tag = Tag__create("test", 1, 2, 3);

    struct TagList *tag_list = TagList__create(tag);
    mu_assert("ERROR, tag_list == NULL",     tag_list != NULL);
    mu_assert("ERROR, tag_list->tag != tag", tag_list->tag == tag);

    mu_assert("ERROR, TagList_delete(tag_list) != 0", 
            TagList_delete(tag_list) == 0);

    return 0;
}

static char *test_TagList_delete() {
    struct Tag *tag_1 = Tag__create("test_1", 1, 1, 1);
    struct Tag *tag_2 = Tag__create("test_2", 2, 2, 2);
    struct Tag *tag_3 = Tag__create("test_3", 3, 3, 3);

    struct TagList *tag_list_1 = TagList__create(tag_1);
    struct TagList *tag_list_2 = TagList__create(tag_2);
    struct TagList *tag_list_3 = TagList__create(tag_3);

    tag_list_1->next = tag_list_2;
    tag_list_2->next = tag_list_3;

    // 本当に再帰的にfreeできてるか確認できていない
    mu_assert("ERROR, TagList_delete(tag_list_1) != 0", 
            TagList_delete(tag_list_1) == 0);
    mu_assert("ERROR, TagList_delete(NULL) != 0",
            TagList_delete(NULL) == 0);

    return 0;
}

static char *test_TagList_create_next() {
    struct Tag *tag_1 = Tag__create("test_1", 1, 1, 1);
    struct Tag *tag_2 = Tag__create("test_2", 2, 2, 2);

    struct TagList *tag_list      = TagList__create(tag_1);
    struct TagList *next_tag_list = TagList_create_next(tag_list, tag_2);

    mu_assert("ERROR, tag_list->next == next_tag_list", 
            tag_list->next == next_tag_list);
    mu_assert("ERROR, tag_list->next->tag != tag2",  
            next_tag_list->tag == tag_2);       
    mu_assert("ERROR, tag_list->next->next != NULL", 
            tag_list->next->next == NULL);

    TagList_delete(tag_list);

    return 0;
}

static char *test_TagList__create_from_csv() {
    struct TagList *tag_list = TagList__create_from_csv(test_data_path);

    mu_assert("ERROR, tag_list->tag->name != \"k-pop\"",         
            strcmp(tag_list->tag->name, "k-pop") == 0);
    mu_assert("ERROR, tag_list->next->tag->name != \"→sm1014\"", 
            strcmp(tag_list->next->tag->name, "→sm1014") == 0);

    TagList_delete(tag_list);

    return 0;
}

/**
 * TagArray
 */
static char *test_TagArray__create_and_TagArray_delete() {
    struct TagArray *tag_array = TagArray__create();
    mu_assert("ERROR, TagArray_delete(tag_array) != 0", 
            TagArray_delete(tag_array) == 0);

    return 0;
}

static char *test_TagArray__create_with_capacity() {
    struct TagArray *tag_array = TagArray__create_with_capacity(32);
    mu_assert("ERROR, tag_array->capacity != 32", tag_array->capacity == 32);

    return 0;
}

static char *test_TagArray_add() {
    struct TagArray *tag_array = TagArray__create();

    struct Tag *tag_1 = Tag__create("test_1", 1, 1, 1);
    struct Tag *tag_2 = Tag__create("test_2", 2, 2, 2);

    mu_assert("ERROR, TagArray_add(tag_array, tag_1) != 0",
            TagArray_add(tag_array, tag_1) == 0);
    mu_assert("ERROR, tag_array->size != 1", tag_array->size == 1);

    mu_assert("ERROR, TagArray_add(tag_array, tag_2) != 0",
            TagArray_add(tag_array, tag_2) == 0);
    mu_assert("ERROR, tag_array->size != 2", tag_array->size == 2);

    mu_assert("ERROR, tag_array->array[0] != tag_1",
            tag_array->array[0] == tag_1);
    mu_assert("ERROR, tag_array->array[1] != tag_2",
            tag_array->array[1] == tag_2);

    TagArray_delete(tag_array);

    return  0;
}

static char *test_TagArray_add_when_size_over_capacity() {
    struct TagArray *tag_array = TagArray__create_with_capacity(2);

    struct Tag *tag_1 = Tag__create("test_1", 1, 1, 1);
    struct Tag *tag_2 = Tag__create("test_2", 2, 2, 2);
    struct Tag *tag_3 = Tag__create("test_3", 3, 3, 3);

    mu_assert("ERROR, TagArray_add(tag_array, tag_1) != 0",
            TagArray_add(tag_array, tag_1) == 0);
    mu_assert("ERROR, tag_array->capacity != 2", tag_array->capacity == 2);

    mu_assert("ERROR, TagArray_add(tag_array, tag_2) != 0",
            TagArray_add(tag_array, tag_2) == 0);
    mu_assert("ERROR, tag_array->capacity != 2", tag_array->capacity == 2);

    mu_assert("ERROR, TagArray_add(tag_array, tag_3) != 0",
            TagArray_add(tag_array, tag_3) == 0);
    mu_assert("ERROR, tag_array->capacity != 4", tag_array->capacity == 4);

    mu_assert("ERROR, tag_array->array[0] != tag_1",
            tag_array->array[0] == tag_1);
    mu_assert("ERROR, tag_array->array[1] != tag_2",
            tag_array->array[1] == tag_2);
    mu_assert("ERROR, tag_array->array[2] != tag_3",
            tag_array->array[2] == tag_3);


    TagArray_delete(tag_array);

    return 0;
}

static char *test_TagArray_update_capacity() {
    struct TagArray *tag_array = TagArray__create();
    mu_assert("ERROR, TagArray_update_capacity(tag_array, 4) != 0", 
            TagArray_update_capacity(tag_array, 4) == 0);
    mu_assert("ERROR, tag_array->capacity != 4", tag_array->capacity = 4);

    struct Tag *tag_1 = Tag__create("test_1", 1, 1, 1);
    struct Tag *tag_2 = Tag__create("test_2", 2, 2, 2);
    TagArray_add(tag_array, tag_1);
    TagArray_add(tag_array, tag_2);

    mu_assert("ERROR, TagArray_update_capacity(tag_array, 2) != 0", 
            TagArray_update_capacity(tag_array, 2) == 0);
    mu_assert("ERROR, tag_array->array[0] != tag_1",
            tag_array->array[0] == tag_1);
    mu_assert("ERROR, tag_array->array[1] != tag_2",
            tag_array->array[1] == tag_2);
    
    mu_assert("ERROR, TagArray_update_capacity(tag_array, 1) != 1", 
            TagArray_update_capacity(tag_array, 1) == 1);
    // 失敗したら、中身は変わらない
    mu_assert("ERROR, tag_array->array[0] != tag_1",
            tag_array->array[0] == tag_1);
    mu_assert("ERROR, tag_array->array[1] != tag_2",
            tag_array->array[1] == tag_2);

    return 0;
}

static char *test_TagArray_sort() {
    struct TagArray *tag_array = TagArray__create();
    TagArray_add(tag_array, Tag__create("c", 4, 4, 4));
    TagArray_add(tag_array, Tag__create("b", 4, 4, 4));
    TagArray_add(tag_array, Tag__create("a", 4, 4, 4));

    mu_assert("ERROR, TagArray_sort(tag_array) != 0", 
            TagArray_sort(tag_array) == 0);

    mu_assert("tag_array->array[0]->name != \"a\"",
            strcmp(tag_array->array[0]->name, "a") == 0);
    mu_assert("tag_array->array[1]->name != names[i]",
            strcmp(tag_array->array[1]->name, "b") == 0);
    mu_assert("tag_array->array[2]->name != names[i]",
            strcmp(tag_array->array[2]->name, "c") == 0);

    return 0;
}

static char *test_TagArray__create_from_csv() {
    struct TagArray *tag_array = TagArray__create_from_csv(test_data_path);

    // 本当はNULLじゃないかからチェックした方がよさげ？
    mu_assert("ERROR, tag_array->array[0]->name != \"k-pop\"",         
            strcmp(tag_array->array[0]->name, "k-pop") == 0);
    mu_assert("ERROR, tag_array->array[1]->name != \"→sm1014\"", 
            strcmp(tag_array->array[1]->name, "→sm1014") == 0);

    TagArray_delete(tag_array);

    return 0;
}


/**
 * CsvReader
 */
static char *test_CsvReader__create_and_CsvReader_delete() {
    struct CsvReader *csv_reader = CsvReader__create();
    mu_assert("ERROR, CsvReader_delete(csv_reader) != 0", 
            CsvReader_delete(csv_reader) == 0);
    
    return 0;
}

static char *test_CsvReader_open() {
    struct CsvReader *csv_reader = CsvReader__create();
    mu_assert("ERROR, CsvReader_open(csv_reader, test_data_path) != 0", 
            CsvReader_open(csv_reader, test_data_path) == 0);

    CsvReader_delete(csv_reader);

    return 0;
}

static char *test_split_line_to_column_list() {
    char *line = "\"３部OVA_MAD\",\"19602\",\"357\",\"118\"";

    struct CsvColumn *head_csv_column = split_line_to_column_list(line);
    struct CsvColumn *csv_column      = head_csv_column;

    mu_assert("ERROR, csv_column->value != \"３部OVA_MAD\"",
            strcmp(csv_column->value, "３部OVA_MAD") == 0);

    csv_column = csv_column->next;
    mu_assert("ERROR, csv_column->value != \"19602\"",
            strcmp(csv_column->value, "19602") == 0);

    csv_column = csv_column->next;
    mu_assert("ERROR, csv_column->value != \"357\"",
            strcmp(csv_column->value, "357") == 0);

    csv_column = csv_column->next;
    mu_assert("ERROR, csv_column->value != \"118\"", 
            strcmp(csv_column->value, "118") == 0);

    mu_assert("ERROR, csv_column->next != NULL", csv_column->next == NULL);

    CsvColumn_delete(head_csv_column);

    return 0;
}

static char *test_CsvReader_gets() {
    struct CsvReader *csv_reader = CsvReader__create();
    if(CsvReader_open(csv_reader, test_data_path) < 0) {
        mu_assert("ERROR, CsvReader_open is failed", NULL);
    }

    struct CsvColumn *csv_column = NULL;

    csv_column = CsvReader_gets(csv_reader);
    mu_assert("ERROR, csv_column->value != \"k-pop\"",   
            strcmp(csv_column->value, "k-pop") == 0);

    csv_column = CsvReader_gets(csv_reader);
    mu_assert("ERROR, csv_column->value != \"→sm1014\"", 
            strcmp(csv_column->value, "→sm1014") == 0);

    // csv_readerの開放によって、csv_columnが開放されている事も確認したい
    CsvReader_delete(csv_reader);

    return 0;
}

/**
 * CsvColumn
 */
static char *test_CsvColumn__create() {
    struct CsvColumn *csv_column = CsvColumn__create("test_column");
    mu_assert("ERROR, csv_column->value != \"test_column\"", 
            strcmp(csv_column->value, "test_column") == 0);

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

    return 0;
}

static char *test_CsvColumn_create_next() {
    struct CsvColumn *csv_column      = CsvColumn__create("1");
    struct CsvColumn *next_csv_column = CsvColumn_create_next(csv_column, "2");

    mu_assert("ERROR, csv_column->next != next_csv_column",
            csv_column->next == next_csv_column);

    CsvColumn_delete(csv_column);

    return 0;
}

/**
 * Files
 */
static char *test_Files__create_and_Files_delete() {
    // with dir
    struct Files *files = Files__create(dir_for_test_path);
    int i;
    for(i = 0; i < files->size; i++) {
        files->file_paths[i];
    }
    mu_assert("ERROR, files->size != 3", files->size == 3);

    Files_delete(files);

    // with file
    files = Files__create(test_data_path);
    mu_assert("ERROR, files->size != 3", files->size == 1);

    Files_delete(files);

    return 0;
}

/**
 * Others
 */
static char *all_tests() {
    // Tag
    mu_run_test(test_Tag__create_and_Tag_delete); 
    mu_run_test(test_Tag__create_from_CsvColumn);

    // TagList
    mu_run_test(test_TagList__create);
    mu_run_test(test_TagList_delete);
    mu_run_test(test_TagList_create_next);

    mu_run_test(test_TagList__create_from_csv);

    // TagArray
    mu_run_test(test_TagArray__create_and_TagArray_delete);
    mu_run_test(test_TagArray__create_with_capacity);
    mu_run_test(test_TagArray_add);
    mu_run_test(test_TagArray_add_when_size_over_capacity);
    mu_run_test(test_TagArray_update_capacity);
    mu_run_test(test_TagArray_sort);

    mu_run_test(test_TagArray__create_from_csv);

    // CsvReader
    mu_run_test(test_CsvReader__create_and_CsvReader_delete);
    mu_run_test(test_CsvReader_open);
    mu_run_test(test_split_line_to_column_list);
    mu_run_test(test_CsvReader_gets);

    // CsvColumn
    mu_run_test(test_CsvColumn__create);
    mu_run_test(test_CsvColumn_delete);
    mu_run_test(test_CsvColumn_create_next);

    // Files
    mu_run_test(test_Files__create_and_Files_delete);

    return 0;
}

char* create_relative_path_from_bin(char *target_path) {
    int relative_path_size = strlen(bin_dir) + strlen(target_path) + 1;

    char *relative_path = (char *) malloc(sizeof(char) * relative_path_size);
    strncpy(relative_path, bin_dir,     strlen(bin_dir));
    strncat(relative_path, target_path, strlen(target_path));

    return relative_path;
}

int main(int argc, char** argv) {
    bin_dir = (char *) malloc(sizeof(char) * (strlen(argv[0]) + 1));
    strncpy(bin_dir, argv[0], strlen(argv[0]) + 1);
    char *pos = strrchr(bin_dir, '/');
    pos++;
    *pos = '\0';
    // printf("%s\n", bin_dir);
    // bin dir 完成
    
    // Get path to test/test_dataa.csv
    // C mendoi
    test_data_path = create_relative_path_from_bin("../test/test_data.csv");
    // printf("%s\n", test_data_path);

    dir_for_test_path = create_relative_path_from_bin("../test/dir_for_test");
    // printf("%s\n", dir_for_test_path);

    // Execute Tests
    char *result = all_tests();

    if(result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }

    printf("Tests run: %d\n", tests_run);

    free(test_data_path);
    free(dir_for_test_path);

    return result != 0;
}
