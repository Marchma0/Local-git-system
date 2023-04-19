#ifndef WORKTREE_H
#define WORKTREE_H

//Constantes
#define MAX_FILES 1000

//Structures
typedef struct {
    char * name ;
    char * hash ;
    int mode ;
} WorkFile ;

typedef struct {
    WorkFile * tab ;
    int size ;
    int n ;
} WorkTree ;


//Fonctions
//WorkFile
WorkFile* createWorkFile(char* name);
void freeWF(WorkFile* wf);
char* wfts(WorkFile* wf);
//WorkTree
WorkFile* stwf(char* ch);
WorkTree* initWorkTree();
void freeWT(WorkTree* wt);
int inWorkTree(WorkTree* wt, char* name);
int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode);
char* wtts(WorkTree* wt);
WorkTree* stwt(char* ch);
int wttf(WorkTree* wt, char* file);
WorkTree* ftwt(char* file);



#endif