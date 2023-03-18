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
#define FILES 100
#define BUFFER 500
#define V 0
#define S 1
#define TDIR 5
#define TFILE 0
#define PATHMAX 256
#define MASK 0x1FF

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

typedef struct Tar *TarPtr;
typedef struct tar {
    char* files[FILES]; /*list of files*/
    TarHeaderPtr headers[FILES]; 
    int numFiles; /*number of files*/
    int numHeaders; /*number of headers in tar*/
    char *tarName;
} Tar;

typedef struct TarHeader *TarHeaderPtr;
typedef struct tarHeader{
    char name[LEN_NAME];
    char mode[LEN_MODE];
    char uid[LEN_UID];
    char gid[LEN_GID];
    char size[LEN_SIZE];
    char mtime[LEN_MTIME];
    char chksum[LEN_CHKSUM];
    char typeflag[LEN_TYPEFLAG];
    char linkname[LEN_LINKNAME];
    char magic[LEN_MAGIC];
    char version[LEN_VERSION];
    char uname[LEN_UNAME];
    char gname[LEN_GNAME];
    char devmajor[LEN_DEVMAJOR];
    char devminor[LEN_DEVMINOR];
} TarHeader; 

/*---MY FUNCTIONS---*/
TarPtr handle_args(int, char *[], char *[], int[]);
void fill_files(int, char **, TarPtr);
void listA(tar);

int main(int argc,char *argv[]){
    char *options;
    int vs[1] = {0, 0};
    /*{V, S}*/
    TarPtr ti;
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

    free(ti);
    return 0;
}

TarPtr handle_args(int argc, char **argv, char **opt, int vs[]) {
    char *cin, *tin, *xin;
    TarPtr ti = (TarPtr) malloc(sizeof(Tar));

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
            vs[V] = 1;
        if (strchr(argv[1], (int) 'S') != NULL)
            vs[S] = 1;
        
        fill_files(argc, argv, ti);
        ti->numFiles = argc - 3;
        ti->tarName = argv[2];
    }
    
    return ti;
}

void fill_files(int argc, char **argv, TarPtr ti) {
    int i;
    for (i = 0; i + 3 < (argc); i++) {
        ti->files[i] = argv[i + 3];
    }
}

void create(TarPtr ti, int *vs){
    int numFiles = ti->numFiles;
    int i, comp, pid = 0;
    char buff[BUFFER];
    int ftar = -1; /*fd for tar*/
    DIR *dir;

    /*Open tar file*/
    if((ftar = open(ti->tarName, O_CREAT | O_TRUNC | O_WRONLY, 0600)) != 0){
        perror("Cannot open tar file.");
        exit(-1);
    }
    /*Open directories*/
    for(i = 0; i < numFiles; i++);
    dir = opendir(ti->files[i]);
    /*if dir not null*/
    if(dir){
        comp = strcmp(*(ti->files[i]), "/");
        /*if '/'*/
        if(comp == 0){
            /*copy into buffer*/
            strcpy(buff, ti->files[i]);
        }
        /*not '/'*/
        else{
            /*copy file name into buff and add '/'*/
            sprintf(buff, "%s/", tar->files[i]);
        }
        /*Create directory*/
        createDirectory(ti, &buff, dir, ftar);
        closedir(dir);
    }
    else{
        /*create a file, dir is null*/
        createFile();
    }
    close(ftar);
}

void createFile(){
    /*open file*/
    /*create header*/
    /*write header to file*/
    /*incr num headers*/
    /*write the contents of file*/
    /*close file*/
}

void createDirectory(TarPtr t, char *filename[], DIR *dir, int ftar){
    DIR *new;
    struct dirent *pd;
    struct stat *sbuff;
    int fdir;
    char buff[BUFFER];

    sbuff = calloc(1, (sizeof(struct stat) + 1));
    fdir = dirfd();
    if(fstat(fdir, sbuff) != 0){
        perror("Stat: createDirectory.");
        exit(-1);
    }
    writeHeader(t, sbuff, filename, 5);
    t->numHeaders++;
    while((pd = readdir(dir)) != NULL){
        if((strcmp(pd->d_name, ".")!=0) && (strcmp(pd->d_name, "..")!=0)){
            if(strlen(pd->d_name) >= PATHMAX){
                perror("Path too long: createDirectory.");
                continue;
            }
            buff = calloc(PATHMAX, sizeof(char));
            sprintf(buff, "%s%s", filename, pd->d_name);
            new = opendir(buff);
            if(new){
                sprintf(buff, "%s%c", buff, "/");
                createDirectory(t, buff, new, ftar);
                closedir(new);
            }
            else{
                /*-------Put in params---------*/
                createFile();
            }
        }
    }
    free(sbuff);
}

void writeHeader(TarPtr t, struct stat *sbuff, char *filename, char t){
    TarHeaderPtr h = t->headers[t->numheaders] = calloc(1, sizeof(TarHeader));
    struct passwd *pwuid;
    struct group *g;
    strcpy(h->name, filename);
    sprintf(h->mode, "%06ho", (unsigned short)(sbuff->st_mode & MASK));
    sprintf(h->uid, "%06o", (int)sbuff->st_uid);
    sprintf(h->gid, "%06o", (int)sbuff->st_gid);
    sprintf(h->mtime, "%lo", (long) sbuff->st_mtime);
    sprintf(h->magic, "%d", LEN_MAGIC);
    strcpy(h->version, "00");
    if (t == TDIR) {
	    sprintf(h->size, "%011lo", (unsigned long)0);
        *(h->typeflag) = '5';
    }
    else if(t == TFILE){
        sprintf(h->size, "%011lo", (long)sbuff->st_size);
        *(h->typeflag) = '0';
    }
    pwuid = getpwuid(getuid());
    g = getgrgid(getgid());
    strcpy(h->uname, pwuid->pw_name);
    strcpy(h->gname, grp->gr_name);
    /*sprintf(h->chksum); */  
    /*linkname, devmajor, devminor*/    
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

void listA(TarPtr tarpt) {
    int tar, headSize;
    TarHeaderPtr header;
    char *buff;

    tar = open(tarpt->tarName, O_RDONLY);
    if (tar == -1) {
        perror("open");
        exit(-1);
    }
    while(read(tar, header, sizeof(TarHeader)) != 0) {
        headSize = strtol(header->size, &buff, 8);
        lseek(tar, headSize, SEEK_CUR);
        printf("%s\n", header->name);
    }
}