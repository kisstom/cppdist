{
  split($1, a, ",|:")
  time = mktime("1970 1 1 "a[1]" "a[2]" "a[3])

  if (NR%2 == 1) {
    start = time
  } else {
    print time - start
  }
}
