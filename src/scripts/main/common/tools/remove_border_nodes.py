import sys

class RemoveBorder:
  def remove(graph_file_name, output_file_name):
    graph_file = open(graph_file_name, 'r')
    output_file = open(output_file_name, 'w')

    swallowers = []
    nr = 0

    for line in graph_file:
      if line.strip() == '':
        swallowers += [nr]
      nr += 1
      if nr % 1000000 == 0:
        print nr, ' number of rows processed while searching bounder nodes'
      sys.stdout.flush()

    swallowers = set(swallowers)

    nr = 0
    graph_file.close()
    graph_file = open(graph_file_name, 'r')
    for line in graph_file:
      spl = line.split()
      first = True
      for node in spl:
        if long(node) not in swallowers:
          if first:
            output_file.write('%s' % node)
            first = False
          else:
            output_file.write(' %s' % node)
      output_file.write('\n')
      nr += 1
      if nr % 1000000 == 0:
        print nr, ' number of rows processed while deleting swallowers'
      sys.stdout.flush()



    graph_file.close()
    output_file.close()

if __name__ == '__main__':
  rb = RemoveBorder()
  rb.remove(sys.argv[1], sys.argv[2])
