BEGIN{
  num1 = 0
  while ((getline < egyik)) {
    e[$1","$2] = $3
    avg1 += $3
    ++num1
  }

  avg1 /= num1
}
{
  avg2 += $3
  if ($1","$2 in e) {
    rmse += ($3 - e[$1","$2]) * ($3 - e[$1","$2])
    rmseAll += ($3 - e[$1","$2]) * ($3 - e[$1","$2])
    delete e[$1","$2]

    ++supp
  } else {
#numDel++
    rmseAll += $3 * $3
  }
}
END{
# printf "numDel %d\n", numDel
# print length(e)

  unionInstance = NR
  for (x in e) {
    rmseAll += e[x] * e[x]
    ++unionInstance
  }

#  printf "section %d union %d\n", supp, unionInstance
#  printf "first avg %f second avg %f\n", avg1, avg2/NR
#  printf "rmse on section %f rmse on union %f\n", sqrt(rmse/supp), sqrt(rmseAll/unionInstance)
  printf "%f\n", sqrt(rmse/supp)
#  print sqrt(rmse/NR)
}
