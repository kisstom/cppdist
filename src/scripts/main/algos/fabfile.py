from fabric.api import run, env, sudo, cd, task, settings, roles
from fabric.operations import local,put,get
from fabric.utils import error
from fabric.context_managers import shell_env
import ConfigParser
import time

global configFile
global isConfReaded
global conf
global cfg_hosts
global numJobs
global partCfg
partCfg = []

env.rolesdefs = {'server' : []}

cfg_hosts = []
numJobs = 0

configFile = None
isConfReaded = False

@task
def yes():
  run('screen -d -m "yes" > /dev/null < /dev/null ')

@task
def cfg(configFileParam):
  '''Sets the config file.'''
  global configFile, conf
  configFile = configFileParam
  conf = readCfg()


def readCfg():
  global conf
  global isConfReaded
  global configFile
  if configFile is None:
    error('Configfile of the deployment is missing! Use task "cfg"!')
  if not isConfReaded:
    conf = readConfig(configFile)

    #storePartitionCfg()

    isConfReaded = True
  env.user = 'kisstom'
  buildHosts(conf)
  return conf

def readConfig(configFile):
  config = ConfigParser.ConfigParser()
  config.optionxform = str
  config.read(configFile)
  return config


def storePartitionCfg():
  global partCfg
  
  partitionDir = conf.get('ALGO', 'REMOTE_DIR')
  partCfgName = conf.get('ALGO', 'SLAVERY_CFG')
  cfg = partitionDir + '/' + partCfgName

  partCfgFile = open(cfg, 'r')
  for line in partCfgFile:
    spl = line.strip().split(' ')
    partCfg += [(spl[1], spl[2], spl[4])]

  lastMaxNode = int(partCfg[-1][1]) + int(partCfg[-1][2])
  partCfg += [('', -1, str(lastMaxNode))]

  partCfgFile.close()

def buildHosts(cfg):
  global numJobs, cfg_hosts
  section = 'MACHINES'
  options = cfg.options(section)
  for option in options:
    numJobs += int(cfg.get(section, option))
    cfg_hosts += [option]
  env.hosts = cfg_hosts

@task
def cleanup():
  global conf
  local_dir = conf.get('ALGO', 'LOCAL_DIR')
  run('rm -rf %s'%local_dir)
  run('mkdir -p %s'%local_dir) 
 
@task
def copyCfg():
  global conf, configFile

  local_dir = conf.get('ALGO', 'LOCAL_DIR')
  put(configFile, local_dir)

@task
def createLocalDir():
  global conf
  local_dir = conf.get('ALGO', 'LOCAL_DIR')
  run('mkdir -p %s'%local_dir)

@task
def startMaster():
    global conf, configFile, numJobs
    bin_dir = conf.get('ALGO', 'BIN')
    master_log = conf.get('ALGO', 'MASTER_LOG')
    run('rm -f %s'%master_log)
    run('(nohup %s/main/algos/master_task %s %d 1> /dev/null 2>&1 < /dev/null &)'%(bin_dir, configFile, numJobs), pty = False) 
    time.sleep(2)

@task
def startNodes():
    global conf, configFile, numJobs
    slave_index = 0
    for host in cfg_hosts:
      env.rolesdefs['server'] = [host]
      jobs_on_host = int(conf.get('MACHINES', host))

      for x in xrange(jobs_on_host):
        startOnMachine(slave_index)
        slave_index += 1
        time.sleep(1)

@roles('server')
def startOnMachine(slave_index):
   global configFile, numJobs, conf
   bin_dir = conf.get('ALGO', 'BIN') 
   logfile = conf.get('ALGO', 'LOCAL_DIR') + 'err_' + str(slave_index)
   run('(nohup %s/main/algos/node_task %s %d %d %s %s %s %s 1> %s 2>&1 < /dev/null &)'%(bin_dir, configFile, slave_index, numJobs, partCfg[slave_index][0], partCfg[slave_index][1], partCfg[slave_index][2], partCfg[slave_index + 1][2], logfile), pty = False)
  

@task
def makePartition():
  # TODO: zipped if null
  print 'makePartition'
  global conf, numJobs

  bin_dir = conf.get('ALGO', 'BIN')
  remoteDir =  conf.get('ALGO', 'REMOTE_DIR')
  inputData =  conf.get('ALGO', 'INPUT_DATA')
  slaveryCfg =  conf.get('ALGO', 'SLAVERY_CFG')
  initSlavePort =  conf.get('ALGO', 'INIT_SLAVE_PORT')
  try:
    splitType = conf.get('ALGO', 'SPLIT_TYPE')
  except Exception:
    splitType = ''

  run('mkdir -p %s'%remoteDir)
  if splitType == '' or splitType == 'EDGE_BALANCED':
    print 'Using partitioner edge balanced'
    numEdgePerPart = getNumEdgePerPart()
    run('%s/main/common/tools/split_clueweb_vector_node -input %s -init_slave_port %s -numedge_perpart %d -part_prefix %s -partition_cfg %s'%(bin_dir, inputData, initSlavePort, numEdgePerPart, remoteDir + 'slavery', remoteDir + slaveryCfg))
 
 
  elif splitType == 'NODE_BALANCED':
    print 'Using partitioner node balanced'
    numNodePerPart = getNumNodePerPart()
    logFile = conf.get('ALGO', 'LOCAL_DIR') + '/err'
    run('%s/main/common/graph_converter/split_by_row_job %s %s %s %d %s %s'%(bin_dir, inputData, logFile, remoteDir + 'slavery', numNodePerPart, remoteDir + slaveryCfg, initSlavePort))


def getNumNodePerPart():
    global conf, numJobs

    numNode = int(conf.get('ALGO', 'NUMLINE'))
    if numNode % numJobs:
      numNodePerPart = numNode // numJobs + 1
    else:
      numNodePerPart = numNode // numJobs

    return numNodePerPart


def getNumEdgePerPart():
    global conf, numJobs

    numEdge = int(conf.get('ALGO', 'NUMEDGE'))
    if numEdge % numJobs:
      numEdgePerPart = numEdge // numJobs + 1
    else:
      numEdgePerPart = numEdge // numJobs

    return numEdgePerPart

@task
def compute():
  global conf
  with  shell_env(LD_LIBRARY_PATH='/home/kisstom/git/DistributedComp/DistributedFrame/src/dep/gmp/lib/:/home/kisstom/git/DistributedComp/DistributedFrame/src/dep/log4cpp/lib/'):
    cleanup()
    storePartitionCfg()
    env.hosts = [conf.get('ALGO', 'MASTER_HOST')]
    copyCfg()
    startMaster()
    startNodes() 

@task
def computeAll():
  global conf, configFile
  with  shell_env(LD_LIBRARY_PATH='/home/kisstom/git/DistributedComp/DistributedFrame/src/dep/gmp/lib/:/home/kisstom/git/DistributedComp/DistributedFrame/src/dep/log4cpp/lib/'):

    cleanup()
    makePartition()
    storePartitionCfg()
    copyCfg()
    env.hosts = [conf.get('ALGO', 'MASTER_HOST')]
    startMaster()
    startNodes() 
