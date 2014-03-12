import sys, math

class NDCG:
  def __init__(self, file1, file2, topk):
    self.file1 = file1
    self.file2 = file2
    self.topk = topk
    self.cum_ndcg = 0.0
    self.userNotInOther = 0

 
  def run(self):
    score1 = dict()
    score2 = dict()
    self.readScores(score1, self.file1)
    self.readScores(score2, self.file2)

    users = score1.keys()
    print 'Counting ndcg'
    sys.stdout.flush()
    for u in users:
      if u not in score2.keys():
        self.userNotInOther += 1
        continue

      self.ndcgForOneUser(score1[u], score2[u])

    print 'Average ndcg: %f'%(self.cum_ndcg / len(users))

  def ndcgForOneUser(self, d1, d2):
    dcg = 0.0
    idcg = 0.0
    
    sort_by_simrank = sorted(d1.items(), key = lambda x: x[1], reverse = True)
    rel = []
    for x in sort_by_simrank:
      if x[0] in d2:
        rel += [d2[x[0]]]
#    rel += [1]
      else:
        rel += [0]

    dcg = self.ndcg(rel)
    idcg = self.ndcg(sorted(d2.values(), reverse = True))
    
    sys.stdout.flush()

    self.cum_ndcg += dcg / idcg

  def ndcg(self, rels):
    rel = 0.0

    for i, x in enumerate(rels):
      rel += (2**x - 1.0) / math.log(i+2, 2)

    return rel

  def readScores(self, userScores, fileN):
    sys.stdout.flush()
    f = open(fileN, 'r')

    for line in f:
      spl = line.split()
      u1 = long(spl[0])
      u2 = long(spl[1])
#     sc = float(spl[2])
      sc = 1.0

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

  print 'Number of users not found in other %d'%ndcg.userNotInOther
