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
      while read sor ; do
        echo $sor >> $newfile
      done < $file
      i=$(( i+1 )) ;
      file="$1"$i.txt;
  done 
fi
