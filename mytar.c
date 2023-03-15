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

typedef struct tarinfo *tarinfoptr;
typedef struct tarinfo {
    char** files; /*list of files*/
    int numFiles; /*number of files*/
    int headers; /*number of headers in tar*/
    char *tarName;
} TarInfo;

tarinfoptr handle_args(int, char **, char **);

int main(int argc,char *argv[]){
    char *options;
    tarinfoptr ti;
    int i;

    ti = handle_args(argc, argv, &options);
    printf("options: %s\n", options);
    printf("tarName:\n");
    for (i = 0; i < sizeof(ti->files)/sizeof(char *); i++)
        printf("\n", ti->files[i]);
    printf("tarName: %d\n", ti->numFiles);
    printf("tarName: %s\n", ti->tarName);

    free(ti);
    return 0;
}

/*strchr for f & check that there's enough valid args, exit otherwise; 
init tarinfoptr*/
tarinfoptr handle_args(int argc, char **argv, char **opt) {
    char *cin, *tin, *xin;
    tarinfoptr ti = (tarinfoptr) malloc(sizeof(TarInfo));

    if ((argc < 3)) {
        printf("Usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
        exit(-1);
    }
    
    *opt = argv[1];
    /*Check if f is included*/
    if (strchr(argv[1], (int) 'f') == NULL) {
        printf("Usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
        exit(-1);
    }
    else {
        cin = strchr(argv[1], (int) 'c');
        tin = strchr(argv[1], (int) 't');
        xin = strchr(argv[1], (int) 'x');

        /* check if opt contains c,t,or x */
        if ((cin == NULL) && (tin == NULL) && (xin == NULL)) { 
            printf("Usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
            printf("[ctxvS]f must include c, t, or x\n");
            exit(-1);    
        }

        ti->files = list_files(argc, argv);
        ti->numFiles = 1;
        ti->tarName = argv[2];
    }
    
    return ti;
}

char **list_files(int argc, char **argv) {
    int a = 0;
}

void create(tarinfoptr ti){
    int nums = ti->numFiles;
    int i = 0;
    int comp;
    char buff[500];
    /*Open every directory passed through args*/
    for(i = 0; i < nums; i++){
        /*if dir not null*/
        if(ti->files[i]){
            comp = strcmp(*(ti->files[i]), "/");
            if(comp == 0){
                /*is a slash*/
                /*copy into buffer*/
                /*create a directory*/
            }
            else{
                /*store file name in buffer*/
                /*add slash*/

            }
        }
        else{
            /*create a file, dir is null*/
        }
    }

}

void createFile(){
    /*open file*/
    /*create header*/
    /*write header to file*/
    /*incr num headers*/
    /*write the contents of file*/
    /*close file*/
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
