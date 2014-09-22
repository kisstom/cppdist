BEGIN{
  while ((getline < egyik)) {
    e[$1] = $2
  }
}
{
  rmse += ($2 - e[$1]) * ($2 - e[$1])
}
END{
#  print rmse, NR
  print sqrt(rmse/NR)
}
