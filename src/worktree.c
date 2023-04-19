#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hdr/worktree.h"

//WorkFile
WorkFile* createWorkFile(char* name){
    WorkFile* workFile = malloc(sizeof(WorkFile));
    workFile->name = strdup(name); 
    workFile->hash = NULL;
    workFile->mode = 0;
    return workFile;
}

void freeWF(WorkFile* wf){
    free(wf->name);
    free(wf->hash);
    free(wf);
}

char* wfts(WorkFile* wf){
    char* str = malloc(sizeof(char) * 1000);
    snprintf(str, 1000, "%s\t%s\t%d", wf->name, wf->hash, wf->mode);
    return str;
}

WorkFile* stwf(char* ch){
    char* name = malloc(sizeof(char)*1000);
    char* hash = malloc(sizeof(char)*1000);
    int mode;
    sscanf(ch, "%s\t%s\t%d", name, hash ,& mode);
    WorkFile* wf = createWorkFile(name);
    wf->mode = mode;
    wf->hash = hash;
    free(name);
    return wf;
}


//WorkTree
WorkTree* initWorkTree(){
    WorkTree* wt = malloc(sizeof(WorkTree));
    wt->tab = malloc(MAX_FILES * sizeof(WorkFile));
    wt->size = MAX_FILES;
    wt->n = 0;
    return wt;
}

void freeWT(WorkTree* wt){
    for(int i=0; i < wt->n ; i++){
        free(wt->tab[i].name);
        free(wt->tab[i].hash);
    }
    free(wt->tab);
    free(wt);
    
}

int inWorkTree(WorkTree* wt, char* name){
    for (int i = 0; i < wt->n; i++){
        if (strcmp(wt->tab[i].name, name) == 0){
            return i;
        }
    }
    return -1;
}

int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode){
    int pos = inWorkTree(wt, name);
    if (pos != -1){
        printf("Nom deja dans le worktree a la position : %d.\n",pos);
        return pos;
    }

    if (wt->n == wt->size){
        printf("Worktree plein.");
        return -1;
    }

    WorkFile* wf = &wt->tab[wt->n];

    wf->name = strdup(name);

    if(hash==NULL){
        wf->hash=NULL;
    }
    else{
        wf->hash = strdup(hash);; 
    }
    wf->mode = mode;
    wt->n++;


    return wt->n - 1;
}

char* wtts(WorkTree* wt){
    //int taille = wt->size * 100;
    char str[1000] = "";

    for (int i = 0; i < wt->n; i++) {
        char* strWf = wfts(&wt->tab[i]); 
        strcat(str, strWf);
        strcat(str, "\n");
        free(strWf);
    }
    return strdup(str);
}

WorkTree* stwt(char* ch){
    int pos = 0;
    int n_pos = 0;
    int sep = '\n';
    char* ptr;
    char* result = malloc(sizeof(char)*10000);
    WorkTree* wt = initWorkTree();
    while(pos<strlen(ch)){
        ptr = strchr(ch+pos, sep);
        if(ptr == NULL){
            n_pos = strlen (ch)+1;
        } else{
            n_pos = ptr - ch + 1;
        }
        memcpy(result, ch + pos, n_pos - pos - 1);
        result[ n_pos - pos - 1] = '\0';
        pos = n_pos;
        WorkFile* wf = stwf(result);
        appendWorkTree(wt, wf->name, wf->hash, wf->mode);
        freeWF(wf);
    }
    
    free(result);
    return wt;
}

int wttf(WorkTree* wt, char* file){
    FILE* f = fopen(file, "w");
    if(f == NULL){
        perror("Error\n");
        return -1;
    }
    char* str = wtts(wt);
    fprintf(f, "%s", str);
    fclose(f);
    free(str);
    return 0;
}

WorkTree* ftwt(char* file){
    FILE* f;
    if ((f = fopen(file, "r")) == NULL) {
        return NULL;
    }
    WorkTree* wt = initWorkTree();
    char buffer[256];
    while (fgets(buffer, 256, f) != NULL) {
        WorkFile* wf = stwf(buffer);
        
        if(strcmp(wf->hash, "(null)")==0){
            appendWorkTree(wt, wf->name,NULL, wf->mode);
        }
        else{
            appendWorkTree(wt, wf->name, wf->hash, wf->mode);
        }
        freeWF(wf);
    }
    fclose(f);
    return wt;
}


/*                                  // VALGRINDED
int main(){

    //WorkFile

    WorkFile* wf = createWorkFile("workfile");
    char* swf = wfts(wf);
    printf("wf: %s\n", swf);

    WorkFile* wf2 = stwf("workfile2     hash    213");
    char* swf2 = wfts(wf2);
    printf("wf2: %s\n\n", swf2);

    free(swf);
    free(swf2);
    freeWF(wf);
    freeWF(wf2);


    //WorkTree

    printf("WorkTree:\n");

    WorkTree* wt = initWorkTree();

    printf("%d\n",inWorkTree(wt,"workfile1"));

    appendWorkTree(wt,"wf1","hash",777);
    appendWorkTree(wt,"wf2","h",777);

    appendWorkTree(wt,"wf2","hash",777); //test ajout deja existant

    char* wts = wtts(wt);
    printf("%s\n",wts);
    free(wts);
    
    char* chaine2 = "WorkFileX\t454\t000\nWorkFileY\t465\t000";
    WorkTree* wt2 = stwt(chaine2);
    char* wt2s = wtts(wt2);
    printf("%s\n",wt2s);
    free(wt2s);

    wttf(wt,"tmp.txt");

    WorkTree* wt3 = ftwt("tmp3.txt");
    char* wt3s = wtts(wt3);
    printf("%s\n",wt3s);
    free(wt3s);
    
    freeWT(wt);
    freeWT(wt2);
    freeWT(wt3);

    return 0;
}

*/