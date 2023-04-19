#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hdr/list.h"

List* initList(){
    List* l = (List*)malloc(sizeof(List));
    *l = NULL;
    return l;
}

Cell* buildCell(char* ch){
    Cell* c = (Cell*)malloc(sizeof(Cell));
    c->data = strdup(ch);
    c->next = NULL;
    return c;
}

void insertFirst(List *L, Cell* C){
    C->next = *L;
    *L = C;
}

void freeCell(Cell* c){
    if(c==NULL){
        return ;
    }
    free(c->data);
    free(c);
}

void freeList(List *l){
    if(*l==NULL){
        free(l);
        return;
    }

    Cell *c = *l;

    while(c != NULL){
        Cell *c2 = c;
        c = c->next;
        freeCell(c2);
    }
    free(l);
}
    

char* ctos(Cell* c){
    return c->data;
}

char* ltos(List* L){
    int len = 0;
    Cell* courant = *L;

    while (courant != NULL){
        len += strlen(courant->data) + 1; // Ajouter 1 pour le séparateur '|'
        courant = courant->next;
    }

    char* res = (char*)malloc(len * sizeof(char));
    res[0] = '\0';

    // Concaténer les chaînes de toutes les cellules avec le séparateur '|'
    courant = *L;
    while (courant != NULL){
        strcat(res, courant->data);
        if (courant->next != NULL){
            strcat(res, "|");
        }
        courant = courant->next;
    }

    return res;
}


Cell* listGet(List* L, int i){
    Cell* courant = *L;
    int j = 0;
    while (courant != NULL && j < i){
        courant = courant->next;
        j++;
    }
    return courant;
}

Cell* searchList(List* L, char* str){
    Cell* courant = *L;
    while (courant != NULL){
        if (strcmp(courant->data, str) == 0){
            return courant;
        }
        courant = courant->next;
    }
    freeCell(courant);
    return NULL;
}

List* stol(char* s){
    List* L = initList();
    if (*s == '\0'){
        return L;
    }

    Cell* courant = NULL;
    char* deb = s;
    char* fin = s;

    while (*fin != '\0'){
        if (*fin == '|'){
            char* name = strndup(deb, fin - deb);
            Cell* c = buildCell(name);
            free(name);
            if (courant != NULL){
                courant->next = c;
            }
            else{
                *L = c;
            }
            courant = c;
            deb = fin + 1;
        }
        fin++;
    }
    if (deb != fin){
        char* name = strndup(deb, fin - deb);
        Cell* c = buildCell(name);
        free(name);
        if (courant != NULL){
            courant->next = c;
        }
        else{
            *L = c;
        }
    }

    return L;
}

void ltof(List* L, char* path){
    FILE* f = fopen(path, "w");
    if (f == NULL){
        printf("Erreur");
        return;
    }
    char* list_str = ltos(L);
    fprintf(f, "%s", list_str);
    fclose(f);
    free(list_str);
}

List* ftol(char* path){
    FILE* f = fopen(path, "r"); 
    if (f == NULL){ 
        printf("Erreur");
        return NULL;
    }
    char s[256]; 
    fgets(s, 256, f); 
    fclose(f); 
    List* L = stol(s); 
    return L;
}

/*                                      //  VALGRINDED
int main(){
    List* l = initList();
    Cell* c = buildCell("cell");
    Cell* c2 = buildCell("cell2");
    Cell* c3 = buildCell("cell3");

    insertFirst(l, c);
    insertFirst(l, c2);
    insertFirst(l, c3);

    char *l1 = ltos(l);
    printf("%s",l1);
    free(l1);

    Cell* lg = listGet(l,1);
    Cell* ls = searchList(l,"cell"); 

    printf("\n");

    char* chaine = "cell|cell2|cell3";
    List* l2 = stol(chaine);

    ltof(l,"tmp.txt");

    List* l3 = ftol("tmp.txt");

    freeList(l);
    freeList(l2);
    freeList(l3);



    return 0;
}
*/