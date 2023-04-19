#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include <sys/stat.h>
#include "../hdr/file.h"

List* listdir(char* root_dir){
    DIR* dp = opendir(root_dir);
    struct dirent* ep;
    if (dp == NULL){
        printf("Erreur\n");
        return NULL;
    }
    List* L = initList();
    while ((ep = readdir(dp)) != NULL){
        if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0){
            continue;
        }
        Cell* C = buildCell(ep->d_name);
        insertFirst(L, C);
    }
    closedir(dp);
    return L;
}

int file_exists (char *file){ 
    struct stat buffer;
    return (stat(file, &buffer) == 0);
}

int isFile(const char *path){
    struct stat path_stat;
    if (file_exists((char*)path) == 1){
        stat(path,&path_stat); 
        return S_ISREG(path_stat.st_mode);   //1 si file 0 si pas file ou n'existe pas
    }
    return -1;
}

void cp(char *to, char *from){
    FILE *src = fopen(from, "r");
    if (src == NULL){
        printf("Erreur ouverture 1 : %s\n", from);
        return;
    }
    
    FILE *dst = fopen(to, "w");

    if(dst == NULL){
        printf("Erreur ouverture 2 : %s\n", to);
        fclose(src);
        return;
    } 
    
    
    char line[256];
    while (fgets(line, 256, src) != NULL){
        fputs(line, dst);
    }
    
    fclose(src);
    fclose(dst);
}

char* hashToPath(char* hash){
    char* path = malloc((strlen(hash) + 10) * sizeof(char));
    path[0] = hash[0];
    path[1] = hash[1];
    path[2] = '/';
    int i;
    for(i=3; i<=strlen(hash);i++){
        path[i] = hash[i-1];
    }
    path[i]='\0';
    return path;
}

void blobFile(char* file){
    char* hash= sha256file(file);
    char* ch2 = strdup(hash);
    ch2[2]='\0';

    if(!file_exists(ch2)){
        char cmd[250];
        sprintf(cmd,"mkdir %s",ch2);
        system(cmd);
    }    

    char* cheminFin = hashToPath(hash);
    cp(cheminFin,file);
    
    free(cheminFin);
    free(hash);
    free(ch2);
}

/*                          // VALGRINDED
int main(){
    List* l = listdir(".");

    printf("%d\n",file_exists("test"));
    printf("%d\n",isFile("test"));
    printf("%d\n",isFile("wt"));

    cp("tmp2.txt","tmp.txt");

    char* hash = sha256file("tmp1.txt");
    char* path = hashToPath(hash);
    printf("%s\n", path);

    blobFile("tmp1.txt");


    free(path);
    free(hash);
    freeList(l);
}

*/