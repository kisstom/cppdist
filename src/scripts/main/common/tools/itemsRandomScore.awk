BEGIN{
	srand(13)
}
{
	node[$1]
}
END{
	for (n in node) {
		for (m in node) {
			if (m < n) {
			    r = rand()
				print m, n, r
				print n, m, r
			}
		}
	}
}