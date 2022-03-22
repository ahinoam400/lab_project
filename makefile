assembler: main.o preAssembler.o commonFunction.o firstPass.o secondPass.o
	gcc -Wall -ansi -pedantic main.o preAssembler.o commonFunction.o firstPass.o secondPass.o -o assembler
main.o: main.c preAssembler.h commonFunction.h firstPass.h secondPass.h
	gcc -Wall -ansi -pedantic main.c -o main.o
commonFunction.o: commonFunction.c commonFunction.h
	gcc -Wall -ansi -pedantic commonFunction.c -o commonFunction.o
firstPass.o: firstPass.c commonFunction.h
	gcc -Wall -ansi -pedantic firstPass.c -o firstPass.o
secondPass.o: secondPass.c commonFunction.h
	gcc -Wall -ansi -pedantic secondPass.c -o secondPass.o