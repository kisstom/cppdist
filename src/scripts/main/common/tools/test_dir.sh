function test_dir {
  my_dir=$1
  for x in `find $my_dir -name "*test*"|grep -v ".*\.o"`
  do
    $x
    if [ $? != 0 ]; then
      echo Test failed. Exiting.
      exit 1
    fi
    echo -e "\n\n--------------------------------------------------"
  done
}
