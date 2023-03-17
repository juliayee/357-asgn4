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
#define BUFFSIZE 10

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

typedef struct tarHeader{
    char name[LEN_NAME + 1];
    mode_t mode;
    uid_t uid;
    gid_t gid;
    size_t size;
    time_t mtime;
    int chksum;
    char typeflag;
    char linkname[LEN_LINKNAME + 1];
    char magic[LEN_MAGIC];
    char version[LEN_VERSION + 1];
    char uname[LEN_UNAME];
    char gname[LEN_GNAME];
    int devmajor;
    int devminor;
    char prefix[LEN_PREFIX + 1];
};

tarinfoptr handle_args(int, char **, char **, int[]);
char **list_files(int, char **);
void listA(tarinfo);

int main(int argc,char *argv[]){
    char *options;
    int vs[1] = {0, 0};
    tarinfoptr ti;
    int i;

    ti = handle_args(argc, argv, &options, vs);

    #ifdef TEST
        printf("options: %s\n", options);
        printf("files:\n");
        for (i = 0; i < ti->numFiles; i++)
            printf("\t%s\n", ti->files[i]);
        printf("numFiles: %d\n", ti->numFiles);
        printf("tar Name: %s\n", ti->tarName);
    #endif

    for (i = 0; i < strlen(options); i++) {
        switch((options)[i]) {
            case 'c':
                /*call creat funcs*/
                printf("c\n");
                break;
            
            case 't':
                /*list funcs*/
                printf("t\n");                
                break;

            case 'x':
                /*extract funcs*/
                printf("x\n");
                break;

            default:
                break;
        }
    }

    free(ti->files);
    free(ti);
    return 0;
}

tarinfoptr handle_args(int argc, char **argv, char **opt, int vs[]) {
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

        /* check if v or S are in opt */
        if (strchr(argv[1], (int) 'v') != NULL)
            vs[0] = 1;
        if (strchr(argv[1], (int) 'S') != NULL)
            vs[1] = 1;
        
        ti->files = list_files(argc, argv);
        ti->numFiles = argc - 3;
        ti->tarName = argv[2];
    }
    
    return ti;
}

char **list_files(int argc, char **argv) {
    int i;
    size_t count = 0, size = 0;
    char **files = NULL;
    for (i = 3; i < (argc); i++) {
        count += 1;
        if (count >= size) {
            size += BUFFSIZE;
            files = realloc(files, size*sizeof(char *));
            if (files == NULL) {
                perror("list_files");
                exit(-1);
            }
        }
        *(files + count - 1) = argv[i];
    }

    return files;
}

void create(tarinfoptr ti){
    int nums = ti->numFiles;
    int i = 0;
    int comp;
    char buff[500];
    int ftd = -1;
    /*Open every directory passed through args*/
    for(i = 0; i < nums; i++){
        /*if dir not null*/
        if(ti->files[i]){
            comp = strcmp(*(ti->files[i]), "/");
            if(comp == 0){
                /*is a slash*/
                /*copy into buffer*/
                strcpy(buff, ti->files[i]);
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

/* ----------------------------- CREATE START ----------------------------- */
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
/* ------------------------------ CREATE END ------------------------------ */

void listA(tarinfoptr *tar) {
    
}