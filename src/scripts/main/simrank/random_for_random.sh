this_dir="$(dirname "$0")"
output_dir=$this_dir/../../../../resources/pseudo_tests

mkdir -p $output_dir

for s in 13 29 39 41
do
  echo ""|awk -v numline=1000 -v seed=$s -f $this_dir/random_for_random.awk > $output_dir/random_$s
done
