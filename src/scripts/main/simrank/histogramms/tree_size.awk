BEGIN{
  FS="|"
}
{
  ++hist[NF - 1]
}
END{
  for (x in hist)
    print x, hist[x]
}
