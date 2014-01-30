thisDir=$(dirname $0)

resourcePath=${1}
testFiles=`find  $thisDir/../../../bin/test/useFileSystem/* -name "*test*"| grep -v ".*\.o"`

for x in $testFiles
do
  $x $resourcePath
  if [ $? != 0 ]; then
    echo Test failed. Exiting.
    exit
  fi
  echo -e "\n\n--------------------------------------------------"

done

echo "File system tests passed."
