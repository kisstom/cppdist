BEGIN{
  while ((getline < input) > 0) {
    if (!($2 in reverse)) {
      reverse[$2] = $1","$3
    } else {
      reverse[$2] = reverse[$2]","$1","$3
    }
  }
  last = -1
}

{
  if ($1 != last) {
    last = $1
    split (reverse[$1], spl, ",")
    for (i = 1; i <= length(spl)/2; ++i) {
      print $1, spl[2*i-1], spl[2*i]
    }
    delete reverse[$1]
  }
  print $1, $2, $3
}
END{
  for (x in reverse) {
    split (reverse[x], spl, ",")
    for (i = 1; i <= length(spl)/2; ++i) {
      print x, spl[2*i-1], spl[2*i]
    }
  }
}
