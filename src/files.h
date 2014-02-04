#ifndef FILES_H
#define FILES_H

struct Files {
    char **file_paths;
    int  size;
};

// クラス的メソッド
struct Files* Files__create(char *path);

// インスタンス的メソッド
int Files_delete(struct Files *files);

#endif
