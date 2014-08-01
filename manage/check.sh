#!/bin/bash
dirName=$(dirname $0) || false
cd $dirName
thisDir=`pwd`
source ./shflags
set -o pipefail
set -o errtrace
DEFINE_boolean color 'false' "use colors" c
DEFINE_boolean no_exclude 'false' "do not use exclude_patterns" e
DEFINE_boolean no_filter 'false' "do not filter, only highlight" f
FLAGS "$@" || exit $?
eval set -- "${FLAGS_ARGV}"

if [ "" == "${FLAGS_jobs}" ] ; then
  FLAGS_jobs=1
fi

source ${thisDir}/../path_config.sh
source ${TESTER_DIR}/scripts/utils/test_util_inc.sh

if [ "$FLAGS_color" == "$FLAGS_TRUE" ] ; then
  COLOR="--color=always"
else
  COLOR=""
fi

if [ "$FLAGS_no_filter" == "$FLAGS_TRUE" ] ; then
  FILTER="warning\|error\|exception\|failure\|"
else
  FILTER="warning\|error\|exception\|failure"
fi

if [ "$FLAGS_no_exclude" == "$FLAGS_TRUE" ] ; then
  grep $COLOR -i "$FILTER"
else
  ${TESTER_DIR}/scripts/utils/grep_patterns.sh "$thisDir"/exclude_patterns.txt -v | grep $COLOR -i "$FILTER"
fi

exit 0

