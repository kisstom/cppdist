import math

class Kendal:

  def __init__(self, file1, file2, topk, delta):
    self.file1 = file1
    self.file2 = file2
    self.topk = topk
    self.delta = delta
    self.kendal = 0.0
    self.section = 0.0

  def run(self):
    score1 = dict()
    score2 = dict()
    self.readScores(score1, self.file1, False)
    self.readScores(score2, self.file2, False)

    users = score1.keys()
    for u in users:
      try:
        self.kendalForOneUser(score1[u], score2[u])
      except KeyError:
        print 'error'

    print 'Average kendal: %f'%(self.kendal / len(users))
    print 'Average section: %f'%(self.section / len(users))


  def kendalForOneUser(self, d1, d2):
    concord = 0
    diconc = 0
    etie = 0
    utie = 0
    union = list(set(d1.keys()) | set(d2.keys()))
    m = 0
    for i,v in enumerate(union):
      for j in xrange(i):
        code = self.kendalTau(union[i], union[j], d1, d2)
        if code == 1: concord += 1; m+=1
        if code == 2: diconc += 1; m+=1
        if code == 3: etie += 1; m+=1
        if code == 4: utie += 1; m+=1
        if code == 5: utie +=1; etie += 1; m+=1

    if m == 1 or m == utie or m == etie: kendal = 0.0 
    else: kendal = (concord - diconc) / math.sqrt((m-utie) * (m-etie))

    section = len(d1.keys()) + len(d2.keys()) - len(union)
    prec = section / min(self.topk, len(d1.keys()), len(d2.keys()))
    print 'kendal', kendal, 'section', section, 'prec', prec 
    self.kendal += kendal
    self.section += section

  def readScores(self, userScores, fileN, rev):
    f = open(fileN, 'r')

    for line in f:
      spl = line.split()
      u1 = long(spl[0])
      u2 = long(spl[1])

      if rev:
        sc = 30 - float(spl[2])
      else:
        sc = float(spl[2])

#print sc
      if u1 not in userScores.keys():
        userScores[u1] = dict()
      if len(userScores[u1]) < self.topk:
        userScores[u1][u2] = sc


    for k in userScores.keys():
      if len(userScores[k].keys()) < 3:
        del userScores[k]

    f.close()

  def kendalTau(self, i, j, d1, d2):
    if i not in d1.keys() and j not in d1.keys(): order1 = 0
    elif i not in d1.keys():
      if d1[j] > self.delta: order1 = -1
      else: order1 = 0
    elif j not in d1.keys():
      if d1[i] > self.delta: order1 = 1
      else: order1 = 0
    elif abs(d1[i] - d1[j]) <= self.delta: order1 = 0
    elif d1[i] - d1[j] > self.delta: order1 = 1
    elif d1[i] - d1[j] < -1*self.delta: order1 = -1

    if i not in d2.keys() and j not in d2.keys(): order2 = 0
    elif i not in d2.keys():
      if d2[j] > self.delta: order2 = -1
      else: order2 = 0
    elif j not in d2.keys():
      if d2[i] > self.delta: order2 = 1
      else: order2 = 0
    elif abs(d2[i] - d2[j]) <= self.delta: order2 = 0
    elif d2[i] - d2[j] > self.delta: order2 = 1
    elif d2[i] - d2[j] < -1*self.delta: order2 = -1

#    try:
#      print i, j, 'order1', order1, 'order2', order2, 'd1[i]',  d1[i], 'd1[j]',  d1[j], 'd2[i]', d2[i], 'd2[j]', d2[j]
#    except Exception:
#      print ''
    if order1*order2 > 0: return 1
    if order1*order2 < 0: return 2
    if order1 == 0 and order2 != 0: return 3
    if order2 == 0 and order1 != 0: return 4
    if order2 == order1 == 0: return 5


if __name__ == "__main__":
  import sys

  file1 = sys.argv[1]
  file2 = sys.argv[2]
  topk = int(sys.argv[3])
  delta = float(sys.argv[4])
  kendal = Kendal(file1, file2, topk, delta)
  kendal.run()

