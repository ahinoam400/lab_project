#!/bin/bash -ex

gcc commonFunctions.c firstPass.c testFirstPass.c 
for f in testsForFirstPass/*.am; do 
  echo $f; 
  ./a.out ${f%.*}; # removes extension 
  echo ""; 
done > output.txt
diff -y output.txt expected.txt
echo Sucess !