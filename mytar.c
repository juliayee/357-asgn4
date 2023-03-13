#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define BUFFER 2048

/*My functions*/
int main(int argc,char *argv[])
{
    /*Usage: mytar [ctxvS]f tarfile [ path [ ... ] ]*/
    char command = argv[1];

    switch(command) {
        case 'c' :
            /*c: create an archive*/
            printf("Excellent!\n" );
            break;
        case 't' :
            /*t: print the table of contents of an archive*/
            break;
        case 'x' :
            /*x: extract the contents of an archive*/
            break;
        case 'v' :
            /*v: increases verbosity*/
            break;
        case 'f' :
            /*f: specifies archive filename*/
            break;
        case 'S' :
            /*S:*/
            break;
        default :
            perror("Usage: mytar [ctxvS]f tarfile [ path [ ... ] ]");
            exit(-1);
   }

    return 0;
}

void createArchive(){

}