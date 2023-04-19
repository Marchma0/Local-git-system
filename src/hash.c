#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../hdr/hash.h"

int hashFile(char* source, char* dest){
	char cmd[250];
	sprintf(cmd,"cat %s|sha256sum|cut -d ' ' -f 1 > %s", source, dest);
	system(cmd);
	return 0;
}

char* sha256file(char* file){
	static char template[] = "/tmp/hash_tmpXXXXXX";
	char fname[1000];
	strcpy(fname, template);
	mkstemp(fname);
	
	hashFile(file,fname);
	
	FILE* f = fopen(fname,"r");
	char* res = malloc(1000*sizeof(char));
	fscanf(f,"%[^\n]",res);

    fclose(f);
    remove(fname);
	
	return res;
}

/*
int main(){

    hashFile("tmp1.txt","tmp2.txt");

	char* hash = sha256file("tmp1.txt");
    printf("%s\n", hash);
	free(hash);

    return 0;
}
*/