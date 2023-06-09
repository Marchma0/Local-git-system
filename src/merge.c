#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hdr/merge.h"

WorkTree *mergeWorkTrees(WorkTree *wt1, WorkTree *wt2, List **conflicts){
    WorkTree *wt = initWorkTree();

  // Recherche des conflits
    if(wt1){
    for(int i = 0; i < wt1->n; i++)
        for(int j = 0; j < wt2->n; j++)
            if( (!strcmp(wt1->tab[i].name, wt2->tab[j].name)) && ( (strcmp(wt1->tab[i].hash, wt2->tab[j].hash)) || (wt1->tab[i].mode != wt2->tab[j].mode) ) )
                insertFirst(*conflicts, buildCell(wt1->tab[i].name));
    }

  // On rajoute les éléments de wt1
    if(wt1){
        for(int i = 0; i < wt1->n; i++){
            Cell* tmp = searchList(*conflicts, wt1->tab[i].name);
            if(!tmp){
                appendWorkTree(wt, wt1->tab[i].name, wt1->tab[i].hash, wt1->tab[i].mode);
            }
        }
    }
    // On rajoute les éléments de wt2
    if(wt2){
        for(int i = 0; i < wt2->n; i++){
            Cell* tmp2 = searchList(*conflicts, wt2->tab[i].name);
            if(!tmp2){
                appendWorkTree(wt, wt2->tab[i].name, wt2->tab[i].hash, wt2->tab[i].mode);
            }
        }
    }

    return wt;
}

List* merge(char* remote_branch, char* message){
    char *current_branch_name = getCurrentBranch();
    char *current_branch = getRef(current_branch_name);

    char *current_commit_path = commitPath(current_branch);
    if(current_commit_path == NULL){
        perror("Impossible de trouver le chemin du fichier correspondant à la branche");
    }
    
    char *remote_commit_path = commitPath(getRef(remote_branch));
    if(current_commit_path == NULL){
        perror("Impossible de trouver le chemin du fichier correspondant à la branche ..");
    }

    Commit *current_commit = ftc(current_commit_path);
    if(current_commit == NULL){
        perror("Conversion vers commit du fichier  impossible..");
        return NULL;
    }

    Commit *remote_commit = ftc(remote_commit_path);
    if(current_commit == NULL){
        perror("Conversion vers commit du fichier  impossible..");
        return NULL;
    }

    char *current_wt_hash = commitGet(current_commit, "tree");
    if(current_wt_hash == NULL){
        perror("Impossible de trouver tree dans le commit ");
    }
    char *remote_wt_hash = commitGet(remote_commit, "tree");
    if(remote_wt_hash == NULL){
        perror("Impossible de trouver tree dans le commit ");
    }
    WorkTree *current_wt = ftwt(workTreePath(current_wt_hash));
    if(current_wt == NULL){
        perror("Conversion du worktree ");
    }

    WorkTree *remote_wt = ftwt(workTreePath(remote_wt_hash));
    if(current_wt == NULL){
        perror("Conversion du worktree ");
    }

    List *conflicts = initList();

    WorkTree* merged_wt = mergeWorkTrees(current_wt, remote_wt, &conflicts);

    if(sizeList(conflicts) > 0){
        perror("La fusion de la branche  et rencontre des conflits");
        return conflicts;
    }

    char *hash = saveWorkTree(merged_wt, ".");  

    Commit *merged_commit = createCommit(hash);
    
    commitSet(merged_commit, "predecessor", current_wt_hash);
    commitSet(merged_commit, "merged_predecessor", remote_wt_hash);

    if(message != NULL){
        commitSet(merged_commit, "message", remote_wt_hash);
    }
    
    const char* commit_hash = blobCommit(merged_commit);

    createUpdateRef(current_branch_name, (char*)commit_hash);
    createUpdateRef("HEAD", (char*)commit_hash);

    deleteRef(remote_branch);

    restoreCommit((char*)commit_hash);

    free(current_commit);
    free(remote_commit);
    free(remote_wt);
    free(current_wt);
    free(merged_wt);
    free(current_wt_hash);
    free(remote_wt_hash);
    free(hash);
    freeList(conflicts);
    free(current_branch);
    free(current_branch_name);
    free(current_commit_path);
    free(remote_commit_path);
    free(merged_commit);
    free((char*)commit_hash);

    return NULL;
}

void createDeletionCommit(char *branch, List *conflicts, char* message){
    char *branchCour = getCurrentBranch();
    myGitCheckoutBranch((char*)branch);

    char *branch_hash = getRef((char*)branch);
    if(branch_hash == NULL){
        perror("Récupérer la référence semble impossible...");
        free(branchCour);
        return;
    }

    char *commit_path = commitPath(branch_hash);
    if(commit_path == NULL){
        perror("Transformer le hash en chemin a renvoyé null");
        free(branch_hash);  
        free(branchCour);
        return;
    }
    free(branch_hash);


    Commit *commit = ftc(commit_path);
    if(commit == NULL){
        perror("La conversion du fichier en commit a renvoyé null");
        free(commit_path);
        free(branchCour);
        return;
    }
    free(commit_path);

    char *wt_hash = commitGet(commit, "tree");
    free(commit);
    if(wt_hash == NULL){
        perror("Commit[\"tree\"] = null");
        free(branchCour);
        return;
    }

    char *wt_path = workTreePath(wt_hash);
    if(wt_path == NULL){
        perror("La conversion en chemin a échoué..");
        free(wt_hash);
        free(branchCour);
        return;
    }
    free(wt_hash);
    
    WorkTree *wt = ftwt(wt_path);
    if(wt == NULL){
        perror("ftwt a renvoyé null ");
        free(branchCour);
        free(wt_path);
        return;
    }
    free(wt_path);
        

    remove(".add");

    createFile(".add");

    for(int i = 0; i < wt->n; i++)
        if(searchList(conflicts, wt->tab[i].name))
        myGitAdd(wt->tab[i].name);

    freeWT(wt);

    myGitCommit((char*)branch, (char*)message);

    myGitCheckoutBranch(branchCour);

    free(branchCour);
}


/*
int main(){

    WorkTree* wt1 = initWorkTree();
    appendWorkTree(wt1, "WorkFile1", "FERGFVREGFVEQ", 000);
    appendWorkTree(wt1, "WorkFile2", "FERGFREGRGRZE", 000);
    appendWorkTree(wt1, "WorkFile3", "RGTRHRTVGR5HT", 000);

    WorkTree* wt2 = initWorkTree();
    appendWorkTree(wt2, "WorkFile1", "FERGFVREGFVEQ", 000);
    appendWorkTree(wt2, "WorkFile2", "FEFDFREGRGRZE", 000);
    appendWorkTree(wt2, "WorkFile5", "RGTRHRDSGR5HT", 000);


    List *l = initList();
    WorkTree *wt3 = mergeWorkTrees(wt1,wt2,&l);
    char* ls = ltos(l);
    printf("l: %s\n",ls);

    char* swt3 = wtts(wt3);
    printf("wt \n%s\n", swt3);


    freeWT(wt1);
    freeWT(wt2);
    freeWT(wt3);
    freeList(l);
    free(ls);
    free(swt3);


    //merge("branche2", "merge?");

    return 0;
}
*/
