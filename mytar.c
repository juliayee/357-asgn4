#include <stdio.h>
#include <stdlib.h>
/*#include <arpa/inet.h>*/
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
/*#include <tar.h>*/
#include <fcntl.h>
#include <dirent.h>

#define setc 0x10
#define sett 0x8
#define setx 0x4
#define setv 0x2
#define setS 0x1

#define BLOCKSIZE 512

/*OFFSETS*/
#define OFF_NAME 0
#define OFF_MODE 100
#define OFF_UID 108
#define OFF_GID 116
#define OFF_SIZE 124
#define OFF_MTIME 136
#define OFF_CHKSUM 148
#define OFF_TYPEFLAG 156
#define OFF_LINKNAME 157
#define OFF_MAGIC 257
#define OFF_VERSION 263
#define OFF_UNAME 265
#define OFF_GNAME 297
#define OFF_DEVMAJOR 329
#define OFF_DEVMINOR 337
#define OFF_PREFIX 345

/*LENGTHS*/
#define LEN_NAME 100 /*NUL terminated if it fits*/
#define LEN_MODE 8
#define LEN_UID 8
#define LEN_GID 8
#define LEN_SIZE 12
#define LEN_MTIME 12
#define LEN_CHKSUM 8
#define LEN_TYPEFLAG 1
#define LEN_LINKNAME 100 /*NUL terminated if it fits*/
#define LEN_MAGIC 6 /*must be "ustar", NUL-terminated */
#define LEN_VERSION 2 /*must be "00"*/
#define LEN_UNAME 32
#define LEN_GNAME 32
#define LEN_DEVMAJOR 8
#define LEN_DEVMINOR 8
#define LEN_PREFIX 155

void handle_args(int, char **, int *);

int main(int argc,char *argv[]){
    int opt = 0;
    
    handle_args(argc, argv, &opt);
    printf("%d\n", opt);

    return 0;
}

    /* strchr to determine which options are input
     * strchr for f and check that there's enough valid, exit otherwise */
void handle_args(int argc, char **argv, int* opt) {
    if ((argc < 3)) {
        printf("Usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
        exit(-1);
    }
    else if (strchr(argv[1], (int) 'f') == NULL) {
        printf("Usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
        exit(-1);
    }
    else {
        if (strchr(argv[1], (int) 'c') != NULL)
            *opt |= setc;
        if (strchr(argv[1], (int) 't') != NULL)
            *opt |= sett;
        if (strchr(argv[1], (int) 'x') != NULL)
            *opt |= setx;
        if (strchr(argv[1], (int) 'v') != NULL)
            *opt |= setv;
        if (strchr(argv[1], (int) 'S') != NULL)
            *opt |= setS;
    }
    if (*opt < setx) { /* doesn't include c, t, or x */
        printf("Usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
        printf("[ctxvS]f must include c, t, or x\n");
        exit(-1);    }
}
