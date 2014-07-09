# st
# end
# buck
# col


BEGIN {
  
}
{
  hist[int(($col - st) / buck)] ++
}
END{
  size = int ((end - st) / buck)
  for (i = 0; i < size; ++i) {
    print i * buck, hist[i]
  }
}
