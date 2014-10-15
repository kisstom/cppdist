{
  split($1, a, ":");
  split(a[3], b, ",");
  time = a[1]*1000000000 + a[2]*1000000 + b[1]*1000 + b[2]
  
  if (NR%2 == 1) {
    start = time
  } else {
    print (time - start)/1000.0
  }
}
