import sys, math

class NDCG:
  def __init__(self, file1, file2, topk):
    self.file1 = file1
    self.file2 = file2
    self.topk = topk
    self.cum_ndcg = 0.0
 
  def run(self):
    score1 = dict()
    score2 = dict()
    self.readScores(score1, self.file1)
    self.readScores(score2, self.file2)

    users = score1.keys()
    print 'Counting ndcg'
    sys.stdout.flush()
    for u in users:
      try:
        print u
        self.ndcgForOneUser(score1[u], score2[u])
      except KeyError:
        print 'error'

    print 'Average ndcg: %f'%(self.cum_ndcg / len(users))

  def ndcgForOneUser(self, d1, d2):
    dcg = 0.0
    idcg = 0.0
    
    sort_by_simrank = sorted(d1.items(), key = lambda x: x[1], reverse = True)
    print sort_by_simrank    
    rel = []
    for x in sort_by_simrank:
      if x[0] in d2:
        rel += [d2[x[0]]]
      else:
        rel += [0]
      print 'Rel', x[0], rel[-1]

    dcg = self.ndcg(rel)
    idcg = self.ndcg(sorted(d2.values(), reverse = True))
    
    print dcg, idcg, dcg/idcg
    sys.stdout.flush()

#    if dcg/idcg == 1.0: print d1, d2
    self.cum_ndcg += dcg / idcg

  def ndcg(self, rels):
    rel = 0.0

    for i, x in enumerate(rels):
      rel += (2**x - 1.0) / math.log(i+2, 2)

    return rel

  def readScores(self, userScores, fileN):
    print 'Reading scores from ', fileN
    sys.stdout.flush()
    f = open(fileN, 'r')

    for line in f:
      spl = line.split()
      u1 = long(spl[0])
      u2 = long(spl[1])
      sc = float(spl[2])

      if u1 not in userScores.keys():
        userScores[u1] = dict()
      if len(userScores[u1]) < self.topk:
        userScores[u1][u2] = sc

    f.close()

if __name__ == "__main__":
  file1 = sys.argv[1]
  file2 = sys.argv[2]
  topk = int(sys.argv[3])
  ndcg = NDCG(file1, file2, topk)
  ndcg.run()

