#ifndef CHECKOUT_H
#define CHECKOUT_H

//Includes
#include "branch.h"
#include "ref.h"
#include "commit.h"
#include "worktree.h"
#include "file.h"
#include "hash.h"
#include "checkout.h"

char* workTreePath(char* hash);
void restoreCommit(char *hash);
void myGitCheckoutBranch(char *branch);
List *filterList(List * L, char *pattern);
int sizeList(List *L);
void myGitCheckoutCommit(char* pattern);


#endif