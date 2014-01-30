import sys, operator
from time import gmtime, strftime

class TreeComputer:
  def __init__(self):
    self.prev_end = ''
    self.nodes = set()
    self.act_fp = ''
    #self.outfile = sys.stdout

  def compare(self, x, y):
    if x[0] > y[0]:
      return 1
    if x[0] < y[0]:
      return -1

    if x[1] > y[1]:
      return 1
    if x[1] < y[1]:
      return -1
    return 0


  def finishTree(self):
    if len(self.nodes) > 0:
      self.outfile.write('%s %s\t'%(self.act_fp, self.prev_end))
      for triple in sorted(self.nodes, cmp=self.compare):
        self.outfile.write('|%d %d %d' % (triple[0], triple[1], triple[2]))
      self.outfile.write('\n')
      self.nodes.clear()

  def processLine(self, line):
    line.rstrip()
    spl = line.split(' ')

    end = spl[1]    
    if end != self.prev_end:
      self.finishTree()
      self.prev_end = end

    fp = spl[0]
    if fp != self.act_fp:
      self.act_fp = fp
      #self.outfile.write('#Path %s\n'% self.act_fp)

    for ind, node in enumerate(spl[2:-1]):
      self.nodes.add((ind, int(node), int(spl[ind + 3])))

  def init(self, line):
    line.rstrip()
    spl = line.split(' ')
    self.prev_end = spl[1]
    self.act_fp = spl[0]   
    #self.outfile.write('#Path %s\n'% self.act_fp)

  def run(self, iFileName, oFileName):
    f = open(iFileName, 'r')
    self.outfile = open(oFileName, 'w')

    line = f.readline()
    self.init(line)
    self.processLine(line)

    numline = 1
    for line in f:
      self.processLine(line)
      numline += 1
      self.log(numline)
   
    self.finishTree()
    f.close()
    self.outfile.close()

  def log(self, numline):
    if numline % 1000000 == 0:
      print '%d line processed %s'%(numline, strftime("%Y-%m-%d %H:%M:%S", gmtime()))
      sys.stdout.flush()


if __name__ == '__main__':
  treeComputer = TreeComputer()
  treeComputer.run(sys.argv[1], sys.argv[2])

