BEGIN{
  found = 0
}
{
  if (found == 1 && $0 ~ /Starting iteration/) {
    exit
  } 

  if ($0 ~ /Starting iteration/) {
    if ($NF == ind) {
      found = 1
    }
  }

  if (found == 1) print $0
  
}
