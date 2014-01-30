import sys, os

script_f = open(sys.argv[1], 'r')
outfile = sys.argv[2]
os.system('rm -f %s'%outfile)

appendix = ''
if len(sys.argv) == 4:
  appendix = '.' + sys.argv[3]

machine = []
num = []
for line in script_f:
  spl = line.strip().split('=')
  if spl[0] == 'MACHINE':
    machine += [spl[1]]
    num += [int(spl[2])]

  if spl[0] == 'LOCAL_DIR':
    local_dir = spl[1]

sum = 0
for i in xrange(len(machine)):
  for j in xrange(sum, sum + num[i]):
    if machine[i] == 'hulk':
      os.system('cat /%s/logs/out_%d%s >> %s'%(local_dir, j, appendix, outfile))
    else:
      os.system('cat /mnt/%s/%s/logs/out_%d%s >> %s'%(machine[i], local_dir, j, appendix, outfile))
    
  sum += num[i]
