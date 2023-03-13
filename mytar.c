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

void handle_args(int, char **, int *);

int main(int argc,char *argv[]){
    int opt = 1;
    
    handle_args(argc, argv, &opt);
    printf("%d\n", opt);

    return 0;
}

    /* strchr to determine which options are input
     * strchr for f, exit otherwise
     * if */
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
        if (strchr(argv[1], (int) 'c') == NULL)
            *opt |= setc;
        if (strchr(argv[1], (int) 't') == NULL)
            *opt |= sett;
        if (strchr(argv[1], (int) 'x') == NULL)
            *opt |= setx;
        if (strchr(argv[1], (int) 'v') == NULL)
            *opt |= setv;
        if (strchr(argv[1], (int) 'S') == NULL)
            *opt |= setS;
    }
}
