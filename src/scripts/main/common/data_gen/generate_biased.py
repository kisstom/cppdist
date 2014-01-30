import sys, random

def randomize(x, y, max):
  global maxP, minP
  if x < max / 2 and y < max /2:
    if random.random() < maxP: return 1
  elif x >= max / 2 and y >= max /2:
    if random.random() < maxP: return 1
  else:
    if random.random() < minP: return 1
  return 0


numRow = int(sys.argv[1])
maxP = float(sys.argv[2])
minP = float(sys.argv[3])


for x in xrange(numRow):
  first = True
  for y in xrange(numRow):
    coin = randomize(x, y, numRow)
    if coin:
      if first:
        print '%d'%y,
        first = False
      else:
        print '%d'%y,
  
  print   
