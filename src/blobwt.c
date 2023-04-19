#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../hdr/blobwt.h"

void setMode(int mode, char* path){
    char buff[100];
    sprintf(buff, "chmod %d %s",mode ,path);
    system(buff);
}

int getChmod(const char* path){
    struct stat ret;

    if(stat(path, &ret) == -1){
        return -1;
    }

    char buff[1024];

    sprintf(buff, "stat -c %%a %s",path);
    //sprintf(buff, "stat -f '%%Lp' %s", path);                     // MacOS version of stat command
    FILE* pipe = popen(buff, "r"); // exécute la commande ls -l
    if (!pipe) {
        fprintf(stderr, "Erreur lors de l'exécution de la commande\n");
        return 1;
    }
    while (fgets(buff, sizeof(buff), pipe)) {
        //printf("%s", buff); // affiche la sortie de la commande
    }
    pclose(pipe); // ferme le processus

    return atoi(buff);
}

char* hashToFile(char* hash){
    char* ch2 = strdup (hash);
    struct stat st;
    ch2[2] = '\0';
    if(stat(ch2, &st) == -1){
        mkdir(ch2 ,0700);
    }
    free(ch2);
    return hashToPath(hash);
}

char* concat_paths(char* path1, char* path2){
    char* result = malloc(strlen(path1) + strlen(path2)+2);
    if(result == NULL){
        printf ("Error : unable to allocate memory\n");
        return NULL;
    }
    strcpy(result, path1); // Copy path1 to result
    strcat(result, "/"); // Append a slash to result
    strcat(result, path2); // Append path2 to result
    return result;
}

char* blobWorkTree(WorkTree* wt){
    char fname[1000] = "/tmp/myfileXXXXXX";
    mkstemp(fname);
    wttf(wt, fname);
    char* hash = sha256file(fname);
    char* ch = hashToFile(hash);
    strcat(ch, ".t");
    cp(ch, fname);
    free(ch);
    return hash;
}

char * saveWorkTree(WorkTree *wt,char * path){ //La fonction sauvegarde les fichiers présents dans le WorkTree, dont la racine est àpath
    if (wt == NULL){ //Teste si le WorkTree est NULL
        printf("Le WorkTree est NULL -> saveWorkTree\n");
        exit(EXIT_FAILURE);
    }
    if (path == NULL){ //Teste si le path est NULL
        printf("Le path est NULL -> saveWorkTree\n");
        exit(EXIT_FAILURE);
    }
    char * absPath;
    for(int i=0;i<wt->n;i++){ //Parcours des WorkFile présents
        //On récupère le path du WorkFile correspondant
        absPath = concat_paths(path,wt->tab[i].name);
        //On stocke le statut du fichier pour faire nos cas
        int etat_file = isFile(absPath);
        if (etat_file == -1){ //Teste si le fichier n'existe pas
            printf("Le fichier %s n'existe pas à l'endroit indiqué (peut-être une erreur du paramètre path ?) -> saveWorkTree\n",absPath);
            exit(EXIT_FAILURE);
        }
        if (etat_file == 1){ //Teste si le WorkFile est un fichier
            blobFile(absPath);
            free(wt->tab[i].hash);
            wt->tab[i].hash = sha256file(absPath);
            wt->tab[i].mode = getChmod(absPath);
        }
        if (etat_file == 0){ //Teste si le WorkFile est un dossier
            WorkTree *wt2 = initWorkTree();
            //On récupère l'ensemble des fichiers et dossiers présents dans le dossier
            List* L = listdir(absPath);   
            Cell* c = *L;          
            while(c!=NULL){ //On construit notre WorkTree
                if(c->data[0]=='.'){ //Ignore les fichiers commençant par .
                    continue;
                }
                appendWorkTree(wt2,c->data,NULL,0);
                c=c->next;
                
            }

            freeList(L);
            free(wt->tab[i].hash);
            wt->tab[i].hash = saveWorkTree(wt2,absPath);
            wt->tab[i].mode = getChmod(absPath);
            freeWT(wt2);
        }
    free(absPath);
    }
    return blobWorkTree(wt);
}

int isWorkTree(char* hash){
    char* pathWF = hashToPath(hash);
    char* pathWT = strcat(hashToPath(hash),".t");
    if(file_exists(pathWT)){
        free(pathWF);
        free(pathWT);
        return 1;
    }
    if(file_exists(pathWF)){
        free(pathWF);
        free(pathWT);
        return 0;
    }
    free(pathWF);
    free(pathWT);
    return -1;
}

void restoreWorkTree(WorkTree* wt, char* path){
    for(int i=0; i< wt->n; i++){
        char* absPath = concat_paths(path, wt->tab[i].name);
        char* copyPath = hashToPath(wt->tab[i].hash);
        char* hash = wt->tab[i].hash;
        if(isWorkTree(hash)==0){
            cp(absPath, copyPath);            
            setMode(wt->tab[i].mode,absPath);
        }
        else{
            if(isWorkTree(hash)==1){
                strcat(copyPath,".t");
                WorkTree* nwt = ftwt(copyPath);
                restoreWorkTree(nwt,absPath);
                setMode(wt->tab[i].mode,absPath);
                freeWT(nwt);
            }
        }
        free(absPath);
        free(copyPath);
    }
}

//VALGRINDED
/*
int main(){
    
    //char* testHash = hashToFile("zefzfzfzfezgrkkzagrj");
    //printf("%s\n",testHash);
    //free(testHash);

    //char* testConc = concat_paths("ala","fe");
    //printf("%s\n",testConc);
    //free(testConc);
    

    WorkTree* wt = initWorkTree();
    appendWorkTree(wt, "Workfile1", "00faa69fc85eed464ef9790204189f7340fcdd0c2baa360c1ebbd4a125b0f86d" ,666);     //"a3960f48bb1f93e212cd1ea623b9b58a50d93a1e876f0172b8c07c34824a50f1"
    appendWorkTree(wt, "Workfile2", "faaa54d90076657c282cb6d158bcf59a8b2846c06639b756ec1a46224c59e474" ,666);     //"7b62626cd1ded1b2403af6d8bd2ad6020be05b85c351001f43fde8b5072f8e2a"
    appendWorkTree(wt, "Workfile4", "c677b6699e15f0852f6f7e34f79c49159be830159d6d5aecd0a9c7cdb2deff03" ,777);     //"e6958c1e13ea49dae30103d2b73b66be18602852e2ce77776c4715e11d9a0a14"

    //char* path = blobWorkTree2(wt);
    //printf("%s\n",path);
    //free(path);

    //char* path2 = saveWorkTree(wt, "wt");
    //free(path2);
    
    //printf("%d\n",getChmod("wt/Workfile1"));
    //printf("%d\n",getChmod("wt/Workfile2"));
    //printf("%d\n",getChmod("wt/Workfile4"));
    //printf("%d\n",getChmod("wt/Workfile4/Workfile3"));
    
    //printf("%s  %d\n",wt->tab[2].hash,isWorkTree(wt->tab[2].hash));

    restoreWorkTree(wt,"wt");

    freeWT(wt);
    return 0;
}
*/