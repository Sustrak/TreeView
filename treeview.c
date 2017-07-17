#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/param.h>

#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_RESET "\x1b[0m"

const int MAXFILENAME = 1024;
struct File {
        char *name;
        char type;
};

void printFiles(char *cwd, int *deep);

void usageSimplified();

void usage();

int main(int argc, char **argv) {
    char cwd[MAXPATHLEN];
    getwd(cwd);
    //OPTIONS
    if(argc > 3) usageSimplified();
    int i,j;
    for(i = 1; i < argc; i++){
        if(argv[i][0] == '-'){
            for(j = 1; argv[i][j] != '\0'; j++){
                char c = argv[i][j];
                switch(c) {
                    case 'h' :
                        usage();
                        break;
                    default:
                        usageSimplified();
                }
            }
        }
        else {
            strcpy(cwd, argv[i]);
        }
    }

    printf("%s\n\n", cwd);
    int deep = 1;
    printFiles(cwd, &deep);
}

void printFiles(char *cwd, int *deep){
    int i;
    char buff[MAXPATHLEN];
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(cwd)) != NULL){
        while((ent = readdir(dir)) != NULL){
            if(ent->d_name[0] != '.'){
                if(ent->d_type == 4){
                    for(i = 0; i < ((*deep)-1)*4; i++) printf(" ");
                    if(*deep > 1) for(i = 0; i < (*deep)-1; i++) printf(" ");
                    printf(" |----");
                    printf(COLOR_MAGENTA "%s\n" COLOR_RESET, ent->d_name);
                    (*deep)++;
                    sprintf(buff, "%s/%s",cwd, ent->d_name);
                    printFiles(buff, deep);
                }
                else {
                    for(i = 0; i <= ((*deep)-1)*4; i++) printf(" ");
                    if(*deep > 1) for(i = 0; i < (*deep)-1; i++) printf(" ");
                    printf("|----%s\n", ent->d_name);
                }
            }
        }
        closedir(dir);
        (*deep)--;
    }
    else {
        printf("Somethig went wrong while trying to open the folder \"%s\" - %s\n", cwd, strerror(errno));
        exit(1);
    }
}

void usage(){
    printf("Usage: treeview [FLAGS] [PATH]\n");
    printf("If you don't specify a path, path will be you current working directory\n");
    printf("Folders will be displayed in magenta\n\n");
    printf("FLAGS:\n");
    printf("      -h: shows information about this tool\n\n");
    exit(0);
}

void usageSimplified(){
    printf("Usage: treeview [FLAGS] [PATH]\n");
    printf("Use the flag -h to get more help\n\n");
    exit(0);
}
