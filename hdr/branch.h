#ifndef BRANCH_H
#define BRANCH_H

//includes
#include "ref.h"
#include "commit.h"
#include "worktree.h"
#include "file.h"
#include "hash.h"

//Fonctions
void initBranch();
int branchExists(char *branch);
void createBranch(char *branch);
char *getCurrentBranch();
char* commitPath(char* hash);
void printBranch(char *branch);
List* branchList(char *branch);
List* getAllCommits();


#endif