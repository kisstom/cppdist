# st
# buck
# xcol
# ycol
# expected sorted inut on xcol field

BEGIN {
  if (st == "") st = 0.0
  if (buck == "") buck = 0.01
  if (xcol == "") xcol = 1 
  if (ycol == "") ycol = 2
  max = st
}
{
  hist[int(($xcol - st) / buck)] += $ycol

  if (max > $xcol) max = $xcol
}
END{
  size = int ((max - st) / buck)
  for (i = 0; i < size; ++i) {
    if (!(i in hist)) hist[i] = 0
    print i * buck, hist[i]
  }
}
