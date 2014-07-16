#!/bin/bash -eu

	thisDir=$(dirname $0)

	outdir=$1
	pushd $thisDir
		git diff --quiet --exit-code || (echo "ERROR: the current state of the git repository is not committed"; exit 42)
		sha=$(git log -1 --pretty=format:%H)
	popd

	mkdir "$outdir"
	date=$(date +%Y-%m-%d-%H-%M-%S)

	cat > "$outdir"/info <<EOF
	date=$date
	sha=$sha
	outdir=$outdir
	HOST=$(hostname)
	HOSTTYPE=$HOSTTYPE
	USER=$USER
	\$0=$0
	\$*=$*
	pwd=$(pwd)
EOF
