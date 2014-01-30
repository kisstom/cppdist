import sys

class TreeCounter:
  def __init__(self):
    self.prev_end = ''
    self.tree_found = 0
    self.tree_size = 0
    self.nodes = set()
    self.act_fp = ''
    self.outfile = sys.stdout

  def finishTree(self):
    self.tree_found += 1
    self.tree_size += len(self.nodes)
    self.nodes.clear()

  def finishFpIndex(self):
    if self.tree_found != 0:
      avg_tree_size = self.tree_size / float(self.tree_found)
      self.outfile.write('avg edge size %f for fp index %s\n'% (avg_tree_size, self.act_fp))
    self.tree_found = 0
    self.tree_size = 0


  def processLine(self, line):
    line.rstrip()
    spl = line.split(' ')
    end = spl[-1]
    
    if end != self.prev_end:
      self.finishTree()
      self.prev_end = end

    fp = spl[0]
    if fp != self.act_fp:
      self.finishFpIndex()
      self.act_fp = fp

    for ind, node in enumerate(spl[1:-1]):
      self.nodes.add((ind, int(node), int(spl[ind + 1])))

  def init(self):
    line.rstrip()
    spl = line.split(' ')
    self.prev_end = spl[-1]
    self.act_fp = spl[0]   

  def run(self, iFileName, oFileName):
    f = open(iFileName, 'r')
    self.outfile = open(oFileName, 'r')

    line = f.readline()
    init(line)
    processLine(line)

    for line in f:
      self.processLine(line)
    
    f.close()


if __name__ == '__main__':
  treeCounter = TreeCounter()
  treeCounter.run(sys.argv[1], sys.argv[2])
