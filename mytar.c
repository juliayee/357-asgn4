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

typedef struct tarinfo {
    char* files; /*list of files*/
    int numFiles; /*number of files*/
    char tarName[];
} TarInfo;

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
        exit(-1);    
    }
}

/*-------------------------------Given functions for 
inserting and removing binary integers 
from non-conforming headers.----------------------*/
uint32_t extract_special_int(char *where, int len) {
    /* For interoperability with GNU tar. GNU seems to
    * set the high–order bit of the first byte, then
    * treat the rest of the field as a binary integer
    * in network byte order.
    * I don’t know for sure if it’s a 32 or 64–bit int, but for
    * this version, we’ll only support 32. (well, 31)
    * returns the integer on success, –1 on failure.
    * In spite of the name of htonl(), it converts int32 t
    */
    int32_t val= −1;
    if ( (len >= sizeof(val)) && (where[0] & 0x80)) {
        /* the top bit is set and we have space
        * extract the last four bytes */
        val = *(int32_t *)(where+len−sizeof(val));
        val = ntohl(val); /* convert to host byte order */
    }
    return val;
}

int insert_special_int(char *where, size_t size, int32_t val) {
    /* For interoperability with GNU tar. GNU seems to
    * set the high–order bit of the first byte, then
    * treat the rest of the field as a binary integer
    * in network byte order.
    * Insert the given integer into the given field
    * using this technique. Returns 0 on success, nonzero
    * otherwise
    */
    int err=0;
    if ( val < 0 || ( size < sizeof(val)) ) {
        /* if it’s negative, bit 31 is set and we can’t use the flag
        * if len is too small, we can’t write it. Either way, we’re
        * done.
        */
        err++;
    } else {
        /* game on....*/
        memset(where, 0, size); /* Clear out the buffer */
        *(int32_t *)(where+size−sizeof(val)) = htonl(val); /* place the int */
        *where |= 0x80; /* set that high–order bit */
    }
    return err;
}
