#ifndef REF_H
#define REF_H

#include "worktree.h"
#include "blobwt.h"
#include "commit.h"
#include "hash.h"
#include "file.h"

//Fonctions
void createFile(char* file);
void initRefs();
void createUpdateRef(char* ref_name, char* hash);
void deleteRef(char* ref_name);
char* getRef(char* ref_name);
void myGitAdd(char* file_or_folder);
void myGitCommit(char *branch_name, char *message);

#endif