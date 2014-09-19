{
	shuffleIndex = $1
	pr = $2

	getline < shuffleIndexFile	
	origIndex = $2

	print origIndex, pr
}