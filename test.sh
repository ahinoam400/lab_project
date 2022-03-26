#!/bin/bash -ex

make main
for f in testsForFirstPass/*.as; do 
  echo "####### $f #######"; 
  ./main ${f%.*}; # removes extension
  cat ${f%.*}.am ${f%.*}.ob || true
  echo ""; 
done > output.txt
diff -y output.txt expected.txt
echo Sucess !