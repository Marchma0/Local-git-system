CC = gcc
CFLAGS = -Wall -g
SRC = src/
OBJ = obj/

all: mygit

mygit: hash.o list.o file.o worktree.o blobwt.o commit.o ref.o branch.o checkout.o merge.o $(SRC)mygit.c
	$(CC) $(CFLAGS) -o myGit $(OBJ)hash.o $(OBJ)list.o $(OBJ)file.o $(OBJ)worktree.o $(OBJ)blobwt.o $(OBJ)commit.o $(OBJ)ref.o $(OBJ)branch.o $(OBJ)checkout.o $(OBJ)merge.o $(SRC)mygit.c

hash.o: $(SRC)hash.c
	$(CC) $(CFLAGS) -c $(SRC)hash.c -o $(OBJ)hash.o

list.o: $(SRC)list.c
	$(CC) $(CFLAGS) -c $(SRC)list.c -o $(OBJ)list.o

file.o: $(SRC)hash.c $(SRC)list.c $(SRC)file.c
	$(CC) $(CFLAGS) -c $(SRC)file.c -o $(OBJ)file.o

worktree.o: $(SRC)worktree.c
	$(CC) $(CFLAGS) -c $(SRC)worktree.c -o $(OBJ)worktree.o

blobwt.o: $(SRC)blobwt.c
	$(CC) $(CFLAGS) -c $(SRC)blobwt.c -o $(OBJ)blobwt.o

commit.o: $(SRC)commit.c
	$(CC) $(CFLAGS) -c $(SRC)commit.c -o $(OBJ)commit.o

ref.o: $(SRC)ref.c
	$(CC) $(CFLAGS) -c $(SRC)ref.c -o $(OBJ)ref.o

branch.o: $(SRC)branch.c
	$(CC) $(CFLAGS) -c $(SRC)branch.c -o $(OBJ)branch.o

checkout.o: $(SRC)checkout.c
	$(CC) $(CFLAGS) -c $(SRC)checkout.c -o $(OBJ)checkout.o

merge.o : $(SRC)merge.c
	$(CC) $(CFLAGS) -c $(SRC)merge.c -o $(OBJ)merge.o


clean:
	rm -f $(OBJ)*.o myGit
