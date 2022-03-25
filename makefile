assembler: main.o preAssembler.o commonFunctions.o firstPass.o secondPass.o
	gcc -Wall -ansi -pedantic main.o preAssembler.o commonFunctions.o firstPass.o secondPass.o -o assembler
main.o: main.c preAssembler.h commonFunctions.h firstPass.h secondPass.h
	gcc -Wall -ansi -pedantic main.c -o main.o
commonFunctions.o: commonFunctions.c commonFunctions.h
	gcc -Wall -ansi -pedantic commonFunctions.c -o commonFunctions.o
firstPass.o: firstPass.c commonFunctions.h
	gcc -Wall -ansi -pedantic firstPass.c -o firstPass.o
secondPass.o: secondPass.c commonFunctions.h
	gcc -Wall -ansi -pedantic secondPass.c -o secondPass.o