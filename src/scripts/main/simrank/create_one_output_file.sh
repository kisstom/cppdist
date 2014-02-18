#! /bin/bash

if  [ $# != 1 ] ;
  then echo "param are: trunkated file name ";
else
  i=0;
  file="$1"0.txt;  
  newfile=$1.out;
  if  [ -e $newfile ]; 
    then rm $newfile; 
  fi
  while [ -e $file ] ; do
      cat $file1 >> $newfile
      i=$(( i+1 )) ;
      rm $file;
      file="$1"$i.txt;
  done 
fi
