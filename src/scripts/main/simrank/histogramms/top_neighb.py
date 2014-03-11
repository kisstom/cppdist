import sys, heapq, string

h = []
graphFile = open(sys.argv[1], 'r')
topCount = int(sys.argv[3])

rowIndex = 0
for line in graphFile:
  if line == '\n':
    c = 0
  else:
    c = string.count(line, ' ') + 1
  
  if len(h) >= topCount:
    heapq.heappushpop(h, (c, rowIndex))
  else:
    heapq.heappush(h, (c, rowIndex))

  rowIndex += 1

  if rowIndex % 1000000 == 0:
    print '%d rows are processed.'%rowIndex
    sys.stdout.flush()

graphFile.close()

outputFile = open(sys.argv[2], 'w')
for c, node in h:
  outputFile.write('%d %d\n'%(node, c))

outputFile.close()
