#!/bin/bash -e

gcc commonFunctions.c firstPass.c testFirstPass.c 
for f in tests/*.am; do 
  echo $f;
  cp $f assemblyExample.am 
  ./a.out; 
  echo ""; 
done > output.txt
diff -y output.txt expected.txt
echo Sucess !