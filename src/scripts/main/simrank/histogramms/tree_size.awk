# Computes the number of edges in each fingerprint tree
# and counts it.


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
