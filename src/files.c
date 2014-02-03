#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "files.h"

#define DEFAULT_FILES_SIZE 512

struct Files* Files__create(char *dir_path) {
    // remote tail / if exists
    if(dir_path[strlen(dir_path) - 1] == '/') {
        dir_path[strlen(dir_path) - 1] = '\0';
    }

    struct Files *files = (struct Files *) malloc(sizeof(struct Files));
    DIR *dp;

    if((dp = opendir(dir_path)) == NULL) {
        perror("opendir");
        return NULL;
    }

    char **file_paths_buf   = (char **) malloc(sizeof(char *) * DEFAULT_FILES_SIZE);
    int file_paths_buf_size = DEFAULT_FILES_SIZE;
    int file_paths_size     = 0;

    struct dirent *entry;
    struct stat   statbuf;
    while((entry = readdir(dp)) != NULL) {
        // ignore
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        int path_size = strlen(dir_path) * strlen(entry->d_name) + 2; // '/' と '\0' で + 2
        char *file_path = (char *) malloc(sizeof(char) * path_size);
        char *file_path_pos = file_path;

        strncat(file_path, dir_path, strlen(dir_path));
        strncat(file_path, "/", strlen("/"));
        strncat(file_path, entry->d_name, strlen(entry->d_name));

        if(stat(file_path, &statbuf) == -1) {
            fprintf(stderr, "stat: (%s)\n", file_path, strerror(errno));
            Files_delete(files);
            free(file_path);
            free(file_paths_buf);

            return NULL;
        }

        if(S_ISDIR(statbuf.st_mode)) {
            free(file_path);

            continue;
        }

        if(file_paths_size > file_paths_buf_size) {
            file_paths_buf_size *= 2;

            char **temp;
            if((temp = (char **) realloc(file_paths_buf, sizeof(char *) * file_paths_buf_size)) == NULL) {
                perror("realloc");
                int i;
                for(i = 0; i < file_paths_size; i++) {
                    free(file_paths_buf[i]);
                }
                free(file_paths_buf);

                return NULL;
            } else {
                file_paths_buf = temp;
            }

        }

        file_paths_buf[file_paths_size++] = file_path;
    }
    closedir(dp);

    files->file_paths = file_paths_buf;
    files->size       = file_paths_size;

    return files;
}

int Files_delete(struct Files *files) {
    if(files == NULL) {
        return 0;
    }

    free(files->file_paths);
    files->file_paths = NULL;

    free(files);

    return 0;
}
