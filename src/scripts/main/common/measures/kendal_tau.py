def kendal(i, j, d1, d2):
    if i not in d1.keys() and j not in d1.keys(): order1 = 0
    elif i not in d1.keys():
      if d1[j] > delta: order1 = -1
      else: order1 = 0
    elif j not in d1.keys():
      if d1[i] > delta: order1 = 1
      else: order1 = 0
    elif abs(d1[i] - d1[j]) <= delta: order1 = 0
    elif d1[i] - d1[j] > delta: order1 = 1
    elif d1[i] - d1[j] < -1*delta: order1 = -1

    if i not in d2.keys() and j not in d2.keys(): order2 = 0
    elif i not in d2.keys():
      if d2[j] > delta: order2 = -1
      else: order2 = 0
    elif j not in d2.keys():
      if d2[i] > delta: order2 = 1
      else: order2 = 0
    elif abs(d2[i] - d2[j]) <= delta: order2 = 0
    elif d2[i] - d2[j] > delta: order2 = 1
    elif d2[i] - d2[j] < -1*delta: order2 = -1

    if order1*order2 > 0: return 1
    if order1*order2 < 0: return 2
    if order1 == 0 and order2 != 0: return 3
    if order2 == 0 and order1 != 0: return 4
    if order2 == order1 == 0: return 5


def readJustScore(file1, file2, topk, delta):
  d1 = dict()
  d2 = dict()

  num = 0
  first = True
  for line in file1:
    score = float(line.strip())
    if first:
      first = False
      if score < delta:
        print 'Tul kicsi ertek.'
        sys.exit(0)
    d1[num] = score
    num+=1
    if num >= topk: break

  num = 0
  first = True
  for line in file2:
    score = float(line.strip())
    if first:
      first = False
      if score < delta:
        print 'Tul kicsi ertek.'
        sys.exit(0)
    d2[num] = score
    num+=1
    if num >= topk: break
  return d1, d2

if __name__ == '__main__':
  import sys, math
  f1 = open(sys.argv[1], 'r')
  f2 = open(sys.argv[2], 'r')
  topk = int(sys.argv[3])
  delta = float(sys.argv[4])

  d1, d2 = readJustScore(f1, f2, topk, delta)

  concord = 0
  diconc = 0
  etie = 0
  utie = 0
  union = list(set(d1.keys()) | set(d2.keys()))
  m = 0
  for i,v in enumerate(union):
    for j in xrange(i):
      code = kendal(union[i], union[j], d1, d2)
      if code == 1: concord += 1; m+=1
      if code == 2: diconc += 1; m+=1
      if code == 3: etie += 1; m+=1
      if code == 4: utie += 1; m+=1
      if code == 5: utie +=1; etie += 1; m+=1

  print 'concord ', concord
  print 'diconc ', diconc
  print 'etie ', etie
  print 'utie ', utie
  print 'kendal ',

  if m == 1 or m == utie or m == etie: print 0.0, page_id
  else: print (concord - diconc) / math.sqrt((m-utie) * (m-etie))

