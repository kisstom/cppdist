cat outdir/out_*| awk '{printf "%s %s", $1, $NF; for (i = 2;i < NF; ++i) printf " %s", $i; printf "\n"}' > concat
