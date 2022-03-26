
objs = commonFunctions.o firstPass.o secondPass.o fileMaker.o preAssembler.o
%.o : %.c
	gcc -c -Wall -ansi -pedantic $< -o $@


test: testFirstPass testSecondPass
#	./testStructs
	./testFirstPass
	./testSecondPass

test% : test%.o $(objs)
	gcc -Wall -ansi -pedantic $^ -o $@

main : main.o $(objs)
	gcc -Wall -ansi -pedantic $^ -o $@

clean:
	rm *.o
	rm main testFirstPass testSecondPass