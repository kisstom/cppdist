$1 - last >= gap {
  print $1, sum
  sum = 0

  last = $1
}

{
  sum += $2
}

