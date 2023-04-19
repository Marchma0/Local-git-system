#ifndef LIST_H
#define LIST_H

//Structures
typedef struct cell {
	char* data ;
	struct cell* next ;
} Cell ;

typedef Cell* List;


//Fonctions
List* initList();
Cell* buildCell(char* ch);
void insertFirst(List *L, Cell* C);
void freeCell(Cell* c);
void freeList(List *l);
char* ctos(Cell* c);
char* ltos(List* L);
Cell* listGet(List* L, int i);
Cell* searchList(List* L, char* str);
List* stol(char* s);
void ltof(List* L, char* path);
List* ftol(char* path);


#endif