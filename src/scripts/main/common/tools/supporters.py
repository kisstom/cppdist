def readGraph(f_name):
  f = open(f_name, 'r')
  numr = 0
  graph = []
  for line in f:
    spl = line.strip().split()
    graph += [[long(x) for x in spl]]
    numr += 1

  inverse = [[] for i in xrange(len(graph))]
  for fro in xrange(len(graph)):
    for to in graph[fro]:
      inverse[to] += [fro]
  return inverse

def supporters(x, graph, num_iter):
  next = [x]
  marked = set([x])
  for k in xrange(num_iter):
    if len(next) == 0: return
    n = next.pop(0)
    for to in graph[n]:
      if to not in marked:
        next += [to]
        marked.add(to)
  print len(marked)
      
if __name__ == '__main__':
  import sys
  graph = readGraph(sys.argv[1])
  supporters(long(sys.argv[3]), graph, int(sys.argv[2]))

