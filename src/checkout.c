#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../hdr/checkout.h"

char* workTreePath( char* hash){
	if(hash == NULL){
		perror("Tentative de conversion avec un hash null");
		return NULL;
	}

	return strcat(hashToPath(hash),".t");;
}

void restoreCommit(char *hash){
	char *path = commitPath(hash);
	Commit *c = ftc(path);

	if(c == NULL){
		perror("La fonction ftc a renvoyé NULL");
		return;
	}

	char *wt_hash = commitGet(c, "tree");
	if(wt_hash == NULL){
		perror("Le commit de contien pas tree ?");
		return;
	}

	char *tree_hash = workTreePath(wt_hash);
	if(tree_hash == NULL){
		perror("Impossible de convertir en chemin d'accès...");
		return;
	}

	WorkTree *wt = ftwt(tree_hash);
	if(wt == NULL){
		perror("Problème de conversion en WorkTree du fichier ");
		return;
	}
	
	restoreWorkTree(wt, ".");

	free(path);
	free(tree_hash);
	freeCommit(c);
	freeWT(wt);
}

void myGitCheckoutBranch(char *branch){
	FILE *f = fopen(".current_branch", "w");
	fprintf(f, "%s", branch);
	fclose(f);

	char* hash_commit = getRef(branch);

	if(hash_commit == NULL){
		perror("Le hash a retourné null...");
		return;
	}

	createUpdateRef("HEAD", hash_commit);
	restoreCommit(hash_commit);

	free(hash_commit);
}

List *filterList(List * L,  char *pattern){
    List *filtered = initList();

    int len = strlen(pattern);
	if(len>0){
		for(Cell *cursor = *L; cursor != NULL; cursor = cursor->next){
			if(strncmp(cursor->data, pattern, len)==0){
				Cell *new_cell = buildCell(cursor->data);
				insertFirst(filtered, new_cell);
			}
		}
	}
    return filtered;
}

int sizeList(List *L){
    int len = 0;

    for(Cell *cursor = *L; cursor != NULL; cursor = cursor->next) 
        len++;

    return len;
}

void myGitCheckoutCommit( char* pattern){

	List *L = getAllCommits();
	List *filtered_list = filterList(L, pattern);

	int len = sizeList(filtered_list);
	if (len == 1){
		char *commit_hash = (*filtered_list)->data;;
		createUpdateRef("HEAD", commit_hash);
		restoreCommit(commit_hash);
	}
	else if (len == 0){
		printf("Aucun hash ne commence par...\n");
	}
	else {
		printf("Plusieurs hash commencent");
		for(Cell *c = *filtered_list; c; c = c->next)
		printf(" -> %s\n", c->data);
	}

	freeList(L);
	freeList(filtered_list);
}

/*
int main(){
		
	//restoreCommit("7c9ea0b4ebf76a75e1df2d05d8cca74bfdb8763ef4787e880092f13952e7a6e6");

	//myGitCheckoutBranch("master");

	//myGitCheckoutCommit("");

	
	//Ca marche ici mais pas dans myGitCheckoutCommit
	List* all_com = getAllCommits();
	ltof(all_com, "1.txt");

	List* dis = filterList(all_com, "2");
	ltof(dis, "2.txt");
	

	return 0;
}
*/

/*
int main(){

	//restoreCommit("fe46acb19cddf0cc3c767c4ed5323fefe7be6d4806cb61330f2b6f38938be5a6");

	//myGitCheckoutBranch("test");

	//myGitCheckoutBranch("master");

	// List* all_com = getAllCommits();

	// List* dis = filterList(all_com, "0");
	// char* sdis = ltos(dis);
	// printf("%s\n",sdis);

	// free(sdis);
	// freeList(all_com);
	// freeList(dis);

	myGitCheckoutCommit("09");

	return 0;
}
*/
