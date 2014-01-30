BEGIN{
  while ((getline < url2id_) > 0) {
    url2id[$1] = $2
  }
}
{
  if ($1 in url2id) {
    print $2, url2id[$1]
  }
}
