all:
	gcc -o bin/parasite src/parasite.c src/ptrace.c src/helper.c src/inject.c src/memsearch.c -std=gnu99
	gcc -o bin/test src/test.c 
