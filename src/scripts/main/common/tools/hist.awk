# st
# end
# buck
# col


BEGIN {
  if (st == "") st = 0.0
  if (end == "") end = 1.0
  if (buck == "") buck = 0.01
  if (col == "") col = 1 

}
{
  hist[int(($col - st) / buck)] ++
}
END{
  size = int ((end - st) / buck)
  for (i = 0; i < size; ++i) {
    if (!(i in hist)) hist[i] = 0
    print i * buck, hist[i]
  }
}
