#ifndef BLOBWT_H
#define BLOBWT_H

//Includes
#include "file.h"
#include "worktree.h"

char* concat_paths(char* path1, char* path2);
char* blobWorkTree(WorkTree* wt);
char * saveWorkTree(WorkTree* wt,char * path);
int isWorkTree(char* hash);
void restoreWorkTree(WorkTree* wt, char* path);
void setMode(int mode, char* path);
int getChmod(const char* path);
char* hashToFile(char* hash);


#endif