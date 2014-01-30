BEGIN{
  srand()
}
{
}
END{
  for (j = 0; j < maxNode; ++j) {
    nr = int(rand() * maxCol)
    if (nr > 0) printf("%d", int(rand() * maxNode))
    for (i = 1; i < nr; ++i) printf(" %d", int(rand() * maxNode))
    printf("\n")
  }
}
