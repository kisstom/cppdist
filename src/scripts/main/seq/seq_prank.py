class ParseError(Exception):
  def __init__(self, value):
    self.value = value

  def __str__(self):
    return self.value

class PageRank:
  configToVarName = dict()
  configToVarName['NUM_ITER'] = 'num_distance'
  configToVarName['TELEPORT_PROB'] = 'dump'
  configToVarName['INPUT_DATA'] = 'input_data'
  configToVarName['INPUT_DATA'] = 'input_data'
  configToVarName['TRUNC_DIST'] = 'trunc'
  configToVarName['TRUST_RANK'] = 'trust_rank'
  configToVarName['TRUST_FILE'] = 'trust_file'

  def __init__(self, config_file, outfile_name):
    self.parseConfig(config_file)
    self.matrix = []
    self.trusted_pages = []
    self.readMatrix()
    self.score = [0.0] * self.num_nodes
    self.rank = [0.0] * self.num_nodes
    self.aux = [0.0] * self.num_nodes
    self.initBuffers()
    self.outfile_name = outfile_name

  def parseConfig(self, config):
    config_file = open(config, 'r')
    for line in config_file:
      spl = line.strip().split('=')
      try:
        varname = self.configToVarName[spl[0]]
        vars(self)[varname] = spl[1]
      except KeyError:
        pass

    for value in self.configToVarName.values():
      try:
        vars(self)[value]
      except KeyError:
        raise ParseError('%s not found in config pars.'%value)

    self.num_distance = int(self.num_distance)
    self.dump = 1.0 - float(self.dump)
    self.trunc = int(self.trunc)
    self.trust_rank = bool(int(self.trust_rank))
    if self.trust_rank  and self.trunc >= 0:
      raise ParseError('Trust rank when truncated.')

    config_file.close()

  def readMatrix(self):
    f = open(self.input_data, 'r')
    for line in f:
      self.matrix +=  [[long(x) for x in line.strip().split()]]
    f.close()

    if self.trust_rank:
      trust_file = open(self.trust_file, 'r')
      for line in trust_file:
        self.trusted_pages += [long(line.strip())]
      trust_file.close()
    self.num_nodes = len(self.matrix)

  def initBuffers(self):
    for node in xrange(self.num_nodes):
      if self.trust_rank:
        if node in self.trusted_pages:
          self.rank[node] = (1.0 - self.dump) / len(self.trusted_pages)
      else:
        self.rank[node] = (1.0 - self.dump) / (self.dump**(self.trunc + 1) * self.num_nodes)
      if self.trunc < 0:
        self.score[node] = self.rank[node]

  def run(self):
    for distance in xrange(self.num_distance):
      self.aux = [0.0] * self.num_nodes
      for src in xrange(self.num_nodes):
        for dest in self.matrix[src]:
          self.aux[dest] += self.rank[src] / len(self.matrix[src])

      # Applying dump factor.
      for src in xrange(self.num_nodes):
        self.rank[src] = self.aux[src] * self.dump
        if distance + 1 > self.trunc: self.score[src] += self.rank[src]

  def flush(self):
    outfile = open(self.outfile_name, 'w')
    for k, v in enumerate(self.score):
      outfile.write('%d %f\n'%(k, v))
    outfile.close()

  def main(self):
    self.run()
    self.flush()
