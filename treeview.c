#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/param.h>
#include <getopt.h>

#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BLUE    "\x1b[34m"

const int MAXFILENAME = 1024;
struct File {
        char *name;
        char type;
};

void printFiles(char *cwd, int *deep, char *name);

int main(int argc, char **argv) {
    char cwd[MAXPATHLEN];
    char name[1024];
    int c;
    char *opt = "hs:";
    struct option long_opt[] = {
      {"help", no_argument, NULL, 'h'},
      {"search", required_argument, NULL, 's'},
      {NULL, 0, NULL, 0}
    };

    while((c = getopt_long(argc, argv, opt, long_opt, NULL)) != -1){
      switch(c){
        case 'h':
          printf("Usage: %s [OPTIONS]\n", argv[0]);
          printf("  -s, --search file/folder            search for this file or folder\n");
          printf("  -h, --help                          print this help and exit\n");
          printf("\n");
          return(0);

        case 's':
          strcpy(name, optarg);
          break;

         case ':':
         case '?':
         fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
         return(-2);

         default:
         fprintf(stderr, "%s: invalid option -- %c\n", argv[0], c);
         fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
         return(-2);
      }
    }
    if(optind>=argc) getwd(cwd);
    else strcpy(cwd, argv[optind]);


    printf("%s\n\n", cwd);
    int deep = 1;
    printFiles(cwd, &deep, name);
}

void printFiles(char *cwd, int *deep, char *name){
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
                    if(!strcmp(name, ent->d_name)) printf(COLOR_BLUE "%s\n" COLOR_RESET, ent->d_name);
                    else printf(COLOR_MAGENTA "%s\n" COLOR_RESET, ent->d_name);
                    (*deep)++;
                    sprintf(buff, "%s/%s",cwd, ent->d_name);
                    printFiles(buff, deep, name);
                }
                else {
                    for(i = 0; i <= ((*deep)-1)*4; i++) printf(" ");
                    if(*deep > 1) for(i = 0; i < (*deep)-1; i++) printf(" ");
                    if(!strcmp(name, ent->d_name)) printf(COLOR_BLUE "|----%s\n" COLOR_RESET, ent->d_name);
                    else printf("|----%s\n", ent->d_name);
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
