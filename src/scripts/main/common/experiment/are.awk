{
  if ($2 - $3 > 0) {
    s += ($2 - $3) / $2
  } else {
    s += ($3 - $2) / $2
  }
}
END{
  print s / NR
}
