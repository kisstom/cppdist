{
  printf("%d %d", NR - 1, NF)
  for (i = 1; i <= NF; ++i) {
    printf(" %d", $i)
  }
  printf("\n")
}
