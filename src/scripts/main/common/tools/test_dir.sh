#!/bin/bash -eu

function test_dir {
# set -o pipefail

  my_dir=$1
  
  params=""
  if [ "$#" == 2 ]; then
    params=$2
  fi

  files=`find $my_dir -name "*test*"|grep -v ".*\.o"| grep -v "swp"`

  for x in $files
  do
    if [ -d $x ]; then
      continue
    fi

    $x $params
    echo -e "--------------------------------------------------\n\n"
  done
}

function test_scripts {
  my_dir=$1

  params=""
  if [ "$#" == 2 ]; then
    params=$2
  fi
  
  files=`find $my_dir -name "*test*"|grep  "sh"|grep -v "swp"`

  for x in $files
  do
    if [ -d $x ]; then
      continue
    fi

    $x $params
    echo -e "--------------------------------------------------\n\n"
  done
}
