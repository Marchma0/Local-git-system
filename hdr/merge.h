#ifndef MERGE_H
#define MERGE_H

//Includes
#include "branch.h"
#include "ref.h"
#include "commit.h"
#include "worktree.h"
#include "file.h"
#include "hash.h"
#include "checkout.h"

//Fonctions
WorkTree *mergeWorkTrees(WorkTree *wt1, WorkTree *wt2, List **conflicts);
List* merge(char* remote_branch, char* message);
void createDeletionCommit(char *branch, List *conflicts, char* message);



#endif