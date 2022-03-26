
objs = commonFunctions.o firstPass.o secondPass.o fileMaker.o
%.o : %.c
	gcc -c -Wall -ansi -pedantic $< -o $@

test: testFirstPass testSecondPass
#	./testStructs
	./testFirstPass
	./testSecondPass
test% : test%.o $(objs)
	gcc -Wall -ansi -pedantic $^ -o $@

