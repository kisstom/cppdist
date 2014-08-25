class Supporters:

  def run(self, graph, dist, node):
    inverse = self.inverseIt(graph)
    return self.supporters(inverse, dist, node)


  def readGraph(self, f_name):
    f = open(f_name, 'r')
    numr = 0
    graph = []
    for line in f:
      spl = line.strip().split()
      graph += [[long(x) for x in spl]]
      numr += 1
    return graph

  def inverseIt(self, graph):
    inverse = [[] for i in xrange(len(graph))]
    for fro in xrange(len(graph)):
      for to in graph[fro]:
        inverse[to] += [fro]
    return inverse

  def supporters(self, graph, num_iter, x):
    next = [x]
    marked = set([x])
    for k in xrange(num_iter):
      if len(next) == 0: return
      n = next.pop(0)
      for to in graph[n]:
        if to not in marked:
          next += [to]
          marked.add(to)
    return len(marked)
      
if __name__ == '__main__':
  import sys
  supporters = Supporters()
  graph = supporters.readGraph(sys.argv[1])
  print supporters.run(graph, int(sys.argv[2]), long(sys.argv[3]))

