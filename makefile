
objs = commonFunctions.o firstPass.o secondPass.o fileMaker.o preAssembler.o
%.o : %.c
	gcc -c -Wall -ansi -pedantic $< -o $@

main : main.o $(objs)
	gcc -Wall -ansi -pedantic $^ -o $@

test: main
	./test.sh

update:
	cp output.txt expected.txt

clean:
	rm *.o main || true 