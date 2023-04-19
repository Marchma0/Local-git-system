#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../hdr/ref.h"

void createFile(char* file){
    char buff[100];
    sprintf(buff, "touch %s", file);
    system(buff);
}

void initRefs(){
    if (access(".refs", F_OK) != 0){
        // Crée le répertoire .refs
        if (mkdir(".refs", 0777) != 0) {
            perror("Erreur lors de la création du répertoire .refs");
            exit(EXIT_FAILURE);
        }
        createFile(".refs/master");
        createFile(".refs/HEAD");
    }
}

void createUpdateRef(char* ref_name, char* hash){
    char buff[100];
    sprintf (buff ,"echo %s > .refs/%s", hash, ref_name);
    system (buff);
}

void deleteRef(char* ref_name){
    char buff[256];
    sprintf (buff,".refs/%s", ref_name);
    if (access(".refs", F_OK) != 0){
        printf ("The reference %s does not exist", ref_name);
    } else{
    sprintf(buff, "rm .refs/%s ", ref_name);
    system(buff);
    }
}

char* getRef(char* ref_name){
    FILE* fp;
    char* result = malloc (sizeof(char)*256);
    strcpy(result,"");
    char buff[256];
    sprintf(buff, ".refs/%s", ref_name);
    if (access(".refs", F_OK) != 0){
        printf ("The Reference %s does not exist", ref_name);
        return NULL;
    }
    fp = fopen (buff ,"r");
    if (fp == NULL ){
        perror("Erreur\n");
        return NULL;
    }
    fgets (result, 256, fp);
    fclose(fp);

    int index = strcspn(result, "\n");
    result[index] = '\0';
    return result;
}

void myGitAdd(char* file_or_folder){
    WorkTree* wt;
    if (access(".add", F_OK) != 0){
        createFile(".add");
        wt = initWorkTree();
    } else{
        wt = ftwt (".add");
    }
    if (access(file_or_folder, F_OK) == 0){
        int etat_file = isFile(file_or_folder);
        if(etat_file==1){
            char* hash = sha256file(file_or_folder);
            int len = strlen(hash);
            if (len > 0){
                hash[len - 3] = '\0';
            }
            appendWorkTree(wt , file_or_folder, hash, getChmod(file_or_folder));
            wttf(wt, ".add");
            free(hash);
        }
        else{
            appendWorkTree(wt , file_or_folder, NULL, getChmod(file_or_folder));
            wttf(wt, ".add");
        }
        
    }else{
        printf ("file or folder %s does not exist\n", file_or_folder);
    }

    freeWT(wt);
}

void myGitCommit(char *branch_name, char *message){
    if(!file_exists(".refs")){
        printf("Il faut d'abord initialiser les références du projets.\n");
        return;
    }

    char buff[256];
    sprintf(buff, ".refs/%s", branch_name);
    if(!file_exists(buff)){
        printf("La branche %s n'existe pas.\n", buff);
        return;
    }

    char *last_hash = getRef(branch_name);
    char *head_hash = getRef("HEAD");

    if(strcmp(head_hash,"")==0 || strcmp(last_hash,"")==0){}
    else{
        if(strcmp(last_hash, head_hash)!=0){
            printf("HEAD doit pointer sur le dernier commit de la branche %s.\n", branch_name);
            return;
        }
    }
    free(head_hash);

    
    WorkTree *wt = ftwt(".add");
    system("rm .add");

    if(wt == NULL){
        printf("Il faut crée le fichier .add");
        free(wt->tab);
        free(wt);
        return;
    }

    char *hash = saveWorkTree(wt, ".");
    Commit *c = createCommit(hash);
    free(hash);

    if (strlen(last_hash) > 0){
        commitSet(c, "predecessor", last_hash);
    }
    if(message != NULL){
        commitSet(c, "message", message); // commit["message"] = message;
    }
    free(last_hash);

    char *commit_hash = blobCommit(c);
    createUpdateRef(branch_name, commit_hash);
    createUpdateRef("HEAD", commit_hash);

    free(commit_hash);
    freeCommit(c);
    freeWT(wt);
}



/*
int main (){
    initRefs();
    //createUpdateRef("ref_name", "HASDKJCE");
    //createUpdateRef("ref_name", "MAJSZHDJX");

    //deleteRef("ref_name");

    //char* refS = getRef("ref_name");
    //printf("%s\n",refS);
    //free(refS);

    myGitAdd("wt/Workfile1");
    myGitAdd("wt/Workfile4");
    myGitAdd("wt/Workfile4/Workfile3");

    myGitCommit("master","haha");

    return 0;
}
*/