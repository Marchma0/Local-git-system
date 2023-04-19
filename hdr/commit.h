#ifndef COMMIT_H
#define COMMIT_H

//Includes
#include "hash.h"
#include "blobwt.h"

//Constantes
#define NCOMMIT 100

//Structures
typedef struct key_value_pair {
    char * key ;
    char * value ;
} kvp ;

typedef struct hash_table {
    kvp ** T ;
    int n ;
    int size ;
} HashTable ;

typedef HashTable Commit ;

//Fonctions
kvp* createKeyVal(char* key, char* val);
void freeKeyVal(kvp* kv);
char* kvts(kvp* k);
kvp* stkv(char* str);
Commit* initCommit();
void freeCommit(Commit* c);
void commitSet(Commit* c, char* key, char* value);
Commit* createCommit(char* hash);
char* commitGet(Commit *c, char *key);
char* cts(Commit* c);
Commit* stc(char* ch);
Commit* ftc(char* file);
void ctf(Commit* c, char* file);
char* blobCommit(Commit* c);




#endif