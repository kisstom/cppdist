{
  ++hist[NF]
}
END{
  for (x in hist)
    print x, hist[x]
}
