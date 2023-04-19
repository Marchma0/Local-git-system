#ifndef FILE_H
#define FILE_H

//Includes
#include "hash.h"
#include "list.h"

//Fonctions
List* listdir(char* root_dir);
int file_exists(char* file);
int isFile(const char *path);
void cp(char *to, char *from);
char* hashToPath(char* hash);
void blobFile(char* file);


#endif