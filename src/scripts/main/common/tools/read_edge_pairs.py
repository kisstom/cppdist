class Reader:
	def rconvert(self, input, output):
		f = open(input, 'r')
		mx = dict()

		count = 0
		for line in f:
			count += 1
			if count % 10000 == 0:
				print '%d rows are processed'%count
				
			spl = line.strip().split('\t')
			spl[0] = int(spl[0])
			spl[1] = int(spl[1])

			if spl[0] not in mx.keys():
				mx[spl[0]] = []

			mx[spl[0]] += [spl[1]]

		f.close()

		f = open(output, 'w')
		for k, v in mx.iteritems():
			f.write('%d', k)

			for neighb in mx[k]:
				f.write(' %d', neighb)

			f.write('\n')

		f.close()


if __name__ == '__main__':
	import sys
	inp = sys.argv[1]
	outp = sys.argv[2]

	r = Reader()
	r.rconvert(inp, outp)


