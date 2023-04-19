#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../hdr/commit.h"

//HashFunction
static unsigned long
    sdbm(str)
    unsigned char *str;
    {
        unsigned long hash = 0;
        int c;

        while ((c = *str++))
            hash = c + (hash << 6) + (hash << 16) - hash;

        return hash;
    }


kvp* createKeyVal(char* key, char* val){
    kvp* k = malloc (sizeof(kvp)) ;
    k->key = strdup (key) ;
    k->value = strdup (val) ;
    return k ;
}

void freeKeyVal(kvp* kv){
    if(kv==NULL){
        free(kv);
        return;
    }
    
    free(kv->key);
    free(kv->value);
    free(kv);
}

char* kvts(kvp* k){
    char* buff = malloc(sizeof(char) * 100);
    sprintf(buff, "%s : %s", k->key, k->value);
    return buff;
}

kvp* stkv(char* str){
    char key[1000], val[1000];
    sscanf(str, "%s : %s", key, val);
    return createKeyVal(key, val);
}

Commit* initCommit(){
    Commit* c = malloc(sizeof(Commit));
    c->T = malloc(NCOMMIT * sizeof(kvp*));
    c->size = NCOMMIT;
    for (int i = 0; i < c->size; i++) {
        c->T[i] = NULL;
    }
    c->n = 0;
    return c;
}

void freeCommit(Commit* c){
    for (int i = 0; i < c->size; i++){
        freeKeyVal(c->T[i]);
    }
    c->n=0;
    free(c->T);
    free(c);
    
}

void commitSet(Commit* c, char* key, char* value){
    int p = sdbm ((unsigned char*)key)%c->size ;
    while (c->T[p] != NULL ) {
        p = (p +1)%c->size ; //probing lineaire
    }

    c->T[p] = createKeyVal(key, value);
    c->n++;
}

Commit* createCommit(char* hash){
    Commit* c = initCommit();
    commitSet(c, "tree", hash);
    return c;
}

char* commitGet(Commit *c, char *key){
    int p = sdbm((unsigned char*)key) % c->size;
    int attempt = 0;
    while (c->T[p] != NULL && attempt < c->size) {
        if (strcmp(c->T[p]->key, key) == 0) {
            return c->T[p]->value;
        }
        p = (p + 1) % c->size;
        attempt = attempt + 1;
    }
    return NULL;
}

char* cts(Commit* c){
    char* str = malloc(sizeof(char) * 1000 * c->n);
    str[0] = '\0';
    for (int i = 0; i < c->size; i++) {
        if (c->T[i] != NULL) {
            char* tmp = kvts(c->T[i]);
            strcat(str, tmp);
            strcat(str, "\n");
            free(tmp);
        }
    }
    return str;
}

Commit* stc(char* ch){
    int pos = 0;
    int n_pos = 0;
    char sep = '\n';
    char* ptr;
    char* result = malloc(sizeof(char) * 10000);
    Commit* c = initCommit();
    while (pos < strlen(ch)) {
        ptr = strchr(ch + pos, sep);
        if (ptr == NULL)
            n_pos = strlen(ch) + 1;
        else
            n_pos = ptr - ch + 1;
        memcpy(result, ch + pos, n_pos - pos - 1);
        result[n_pos - pos - 1] = '\0';
        pos = n_pos;
        kvp* elem = stkv(result);
        commitSet(c, elem->key, elem->value);
        freeKeyVal(elem);
    }
    free(result);
    return c;
}

Commit* ftc(char* file){

    const int MESSAGE_SIZE = 100000; // taille maximale d'un message
    char buff[MESSAGE_SIZE];
    char* all = malloc(sizeof(char) * MESSAGE_SIZE);
    strcpy(all,"");

    FILE* f = fopen(file, "r");
    if (f == NULL) {
        printf("ERROR: file does not exist\n");
        return NULL;
    }
    while (fgets(buff, MESSAGE_SIZE, f) != NULL) {
        strcat(all, buff);
    }
    Commit* c = stc(all);
    fclose(f);
    free(all);
    return c;
}

void ctf(Commit* c, char* file){
    FILE* fp = fopen(file, "w");
    if (fp != NULL) {
        char* cs = cts(c);
        fputs(cs, fp);
        free(cs);
        fclose(fp);
    }
    
}

char* blobCommit(Commit* c){
    char fname[100] = "/tmp/myfileXXXXXX";
    mkstemp(fname);
    ctf(c, fname);
    char* hash = sha256file(fname);
    char* ch = hashToFile(hash);
    strcat(ch, ".c");
    cp(ch, fname);
    free(ch);
    return hash;
}

/*                                              // VALGRINDED
int main(){

    kvp* k1 = createKeyVal("key1","val1");
    char* k1str = kvts(k1);
    printf("chaine : %s \n",k1str);
    free(k1str);
    freeKeyVal(k1);

    char* chaine = "key2 : val2";
    kvp* k2 = stkv(chaine);
    char* k2str = kvts(k2);
    printf("chaine: %s\n", k2str);
    free(k2str);
    freeKeyVal(k2);

    Commit* c = initCommit();
    commitSet(c, "key3", "val3");
    commitSet(c, "key4", "val4");

    Commit* c2 = createCommit("REFFRFEFCQ");

    printf("val for key3 in c: %s\n", commitGet(c, "key3"));
    printf("val for key4 in c: %s\n", commitGet(c, "key4"));
    printf("val for key5 in c: %s\n\n", commitGet(c, "key5"));

    char* cs = cts(c);
    printf("commit c :\n%s\n", cs);
    free(cs);

    char* chaine2 = "val6 : key6\nval7 : key7";
    Commit* c3 = stc(chaine2);
    char* c3s = cts(c3);
    printf("chaine:\n%s\n", c3s);
    free(c3s);

    ctf(c3, "tmp4.txt");

    Commit* c4 = ftc("tmp5.txt");
    char* c4s = cts(c4);
    printf("%s",c4s);
    free(c4s);

    char* hash = blobCommit(c4);
    free(hash);

    
    freeCommit(c);
    freeCommit(c2);
    freeCommit(c3);
    freeCommit(c4);

    return 0;
}
*/