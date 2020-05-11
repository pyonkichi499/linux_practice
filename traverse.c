#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

static void do_ls(char *path);

int main(int argc, char *argv[]){
    int i;

    if (argc < 2) {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }
    for (i = 1; i < argc; i++) {
        do_ls(argv[i]);
    }
    exit(0);
}

static void do_ls(char *path){
    DIR *d;
    struct dirent *ent;
    char corrent_dir[256];

    d = opendir(path);
    if (!d) {
        perror(path);
        exit(1);
    }
    while (ent = readdir(d)) { 
        if (!strcmp(".", ent->d_name)) continue;
        if (!strcmp("..", ent->d_name)) continue;
        //exclude "." and ".."

        printf("%s\n", ent->d_name);
        if (ent->d_type == DT_DIR){// if d_type is a directory, call do_ls()
            sprintf(corrent_dir, "%s/%s", path, ent->d_name);
            do_ls(corrent_dir);
        }

    }
    closedir(d);
}