# lab_project 
Devloped by Amir Wolf and Achinoam Yarden
In order to build the project

```
make
```

In order to run tests
```
make test
```

To accept the test results changes run

```
make update
```

The file `expected.txt` contains the expected output of the tests.

The folder `testMultipleFiles` can be used to test running the assembler on multiple files:

```
make
cd testMultipleFiles
../main test1 test2
```

