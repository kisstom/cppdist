BEGIN{
  found = 0
}
{
  if (found == 1 && $0 ~ /Algo waiting for instruction from master./) {
    print $0
    exit
  } 

  if ($0 ~ /Starting iteration/) {
    if ($NF == ind) {
      found = 1
      print $0
    }
  }

}
