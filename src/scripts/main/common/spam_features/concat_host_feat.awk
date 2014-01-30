BEGIN {
  while ((getline < join_with_this_file)> 0) {
    value = $2" "$3" "$4" "$5
    join_with_this[$1] = value
  }
}
{
  if ($1 in join_with_this) {
    print $0, join_with_this[$1]
  } else {
    print $0, "0", "0", "0", "0"
  }
}
