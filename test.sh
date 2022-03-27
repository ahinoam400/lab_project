#!/bin/bash -ex

for f in testsForFirstPass/*.as; do 
  echo "####### $f #######"; 
  ./main ${f%.*}; # removes extension
  cat ${f%.*}.am ${f%.*}.ob ${f%.*}.ent ${f%.*}.ext || true
  echo ""; 
done > output.txt
diff -y output.txt expected.txt
echo Sucess !