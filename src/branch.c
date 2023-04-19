#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../hdr/branch.h"

void initBranch(){
	FILE *f = fopen(".current_branch", "w");

	if(f == NULL){
		perror("Problème lors de l'ouverture du fichier .current_branch");
		return;
	}

	fputs("master", f);
	fclose(f);
}

int branchExists(char *branch){
	List *refs = listdir(".refs/");
	int result = searchList(refs, branch) != NULL;
	freeList(refs);
	return result;
}

void createBranch(char *branch){
	if(branch == NULL){
		return;
	}

	char *hash = getRef("HEAD");
	if(hash == NULL){
		perror("HEAD est vide.\n");
		return;
	}

	createUpdateRef(branch, hash);
	
	free(hash);
}

char *getCurrentBranch(){
	FILE *f = fopen(".current_branch", "r");
	char *buff = malloc(sizeof(char) * 100);

	fgets(buff, 100, f);
	fclose(f);

	return buff;
}

char* commitPath(char* hash){

	if(hash == NULL){
		perror("Tentative de conversion avec un hash null");
		return NULL;
	}

	char *hash_path = hashToPath(hash);

	if(hash_path == NULL){
		perror("hashToPath a renvoyé NULL");
		return NULL;
	}

	char *path = malloc(sizeof(char) * 1000);

	
	sprintf(path,"%s.c",hash_path);
	free(hash_path);

	return path;
}

void printBranch(char *branch){
	
	char *commit_hash = getRef(branch);
	char *path = commitPath(commit_hash);
	
	Commit *c = ftc(path);

	if(path==NULL) free(path);
	
	while(c != NULL){
		char *msg = commitGet(c, "message");
		char *prev = commitGet(c, "predecessor");
		if(msg) {
			printf("%s => \"%s\"\n", commit_hash, msg);
		}

		else{
			printf("%s\n", commit_hash);
		}

		if(prev){
			free(commit_hash);
			commit_hash = strdup(prev);

			free(path);
			path = commitPath(commit_hash);
			
			freeCommit(c);
			c = ftc(path);
			
			if (c == NULL){
				perror("ftc a renvoyé null..");
				if(path){
					free(path);
				}
				break;
			}

			if(path){
				free(path);
			} 
		}

		else {
			free(commit_hash);
			freeCommit(c);
			c = NULL;
		}
	}
	free(path);
}

List *branchList( char *branch){
	List *L = initList();

	char *hash = getRef(branch);
	if(hash == NULL){
		perror("getRef() a renvoye NULL");
		return NULL;
	}
	
	char *path = commitPath(hash);
	if(path == NULL){
		perror("commitPath() a renvoye NULL");
		free(hash);
		return NULL;
	}

	Commit *c = ftc(path);
	if(c == NULL){
		perror("La conversion de () a échoué");
		free(hash); 
		free(path);
		return NULL;
	}

	while(c != NULL){
		insertFirst(L, buildCell(hash));
		char *prev = commitGet(c, "predecessor");

		if(path){
		//free(path);
		}
		if(hash){
		free(hash);
		}

		if(prev){
		hash = strdup(prev); // On duplique pour facilité la libération de mémoire
		free(path);
		path = commitPath(hash);

		freeCommit(c);
		c = ftc(path);
		}
		else {
		freeCommit(c);
		c = NULL;
		}
	}

	free(path);

	return L;
}

List *getAllCommits(){
	List *L = initList();
	List *content = listdir(".refs");

	for(Cell *cursor = *content; cursor != NULL; cursor = cursor->next){
		if(cursor->data[0] == '.') continue;

		List *list = branchList(cursor->data);
		Cell *cell = *list;

		while(cell != NULL){
			if(searchList(L, cell->data) == NULL){
				insertFirst(L, buildCell(cell->data));
			}
			cell = cell->next;
		}

		freeList(list);
	}

	freeList(content);

	return L;
}


/*

int main(){
	//initBranch();

	//printf("%d\n", branchExists("test"));

	//createBranch("branch1");

	//char* branche = getCurrentBranch();
    //printf("%s\n", branche);
    //free(branche);

	//printBranch("master");

	
	// List* l = branchList("master");
	// char* ls = ltos(l);
	// printf("%s\n",ls);
	// free(ls);
	// freeList(l);
	

	List* l = getAllCommits();
	char* ls = ltos(l);
	printf("%s\n",ls);
	free(ls);
	freeList(l);

	return 0;
}*/