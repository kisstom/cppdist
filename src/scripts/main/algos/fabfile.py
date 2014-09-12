from fabric.api import run, env, sudo, cd, task, settings, roles
from fabric.operations import local,put,get
from fabric.utils import error
from fabric.decorators import hosts
from fabric.context_managers import shell_env
import ConfigParser
import time, os, tempfile

global configFile
global isConfReaded
global conf
global cfg_hosts
global numJobs
global partCfg
global pids
global crawlMaxNodes
global BASE_LOCAL_DIR
global BASE_CONCAT_DIR
global MASTER_LOG

configFile = None
isConfReaded = False
cfg_hosts = []
numJobs = 0
partCfg = []
pids = dict()
crawlMaxNodes = []
BASE_LOCAL_DIR = ""
BASE_CONCAT_DIR = ""
MASTER_HOST = ''

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
  global BASE_LOCAL_DIR, MASTER_HOST
  if configFile is None:
    error('Configfile of the deployment is missing! Use task "cfg"!')
  if not isConfReaded:
    conf = readConfig(configFile)
    isConfReaded = True

  env.user = 'kisstom'
  BASE_LOCAL_DIR = conf.get('ALGO', 'LOCAL_DIR')
  MASTER_HOST = conf.get('MASTER_HOST')

  conf.set("ALGO", "MASTER_LOG", BASE_LOCAL_DIR + "master.log")
  buildHosts(conf)
  return conf

def readConfig(configFile):
  config = ConfigParser.RawConfigParser()
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


def cleanup():
  global conf, pids
  local_dir = conf.get('ALGO', 'LOCAL_DIR')
  run('rm -rf %s'%local_dir)
  run('mkdir -p %s'%local_dir) 
  pids.clear()


@task
def copyCfg():
  global conf, configFile
  local_dir = conf.get('ALGO', 'LOCAL_DIR')
  configFile = local_dir + 'deploy.ini'

  tempf = tempfile.mktemp()
  tempfO = open(tempf, 'w')
  conf.write(tempfO)
  tempfO.close()

  put(tempf, configFile)
  os.system('rm ' + tempf)
  

@task
def createLocalDir():
  global conf
  local_dir = conf.get('ALGO', 'LOCAL_DIR')
  run('mkdir -p %s'%local_dir)

@task
def createConcatDir():
  global conf
  concat_dir = conf.get('ALGO', 'CONCAT_DIR')
  run('mkdir -p %s'%concat_dir)

def storePid(host, pid):
  global pids
  if host not in pids:
    pids[host] = []

  pids[host] += [pid]

def startMaster():
    global conf, configFile, numJobs, pids
    with settings(host_string=MASTER_HOST):
      bin_dir = conf.get('ALGO', 'BIN')
      master_log = conf.get('ALGO', 'MASTER_LOG')
      run('rm -f %s'%master_log)
      logfile = conf.get('ALGO', 'LOCAL_DIR') + 'err_master'
      pid = run('''(nohup %s/main/algos/task/master_task %s %d 1> %s 2>&1 < /dev/null & 
        echo $!)'''%(bin_dir, configFile, numJobs, logfile), pty = False)
      storePid(env.host[0], pid)
      time.sleep(2)


def isWorkFinished():
  global pids
  for host in pids.keys(): 
    with settings(host_string=host):
      for pid in pids[host]:
        isRun = checkProcess(pid)
        if isRun:
          return False

  return True

def waitForFinish():
  print 'Waiting for finish'
  while 1 == 1:
    isFinished = isWorkFinished()
    if (isFinished):
      break

    os.system('sleep 10')

  print 'Run finished.'

def startNodes():
    global conf, numJobs
    slave_index = 0
    for host in cfg_hosts:
        jobs_on_host = int(conf.get('MACHINES', host))
        for x in xrange(jobs_on_host):
          startOnMachine(slave_index, host)
          slave_index += 1
          time.sleep(1)

def startOnMachine(slave_index, host):
  global configFile, numJobs, conf
  with settings(host_string=host):
    bin_dir = conf.get('ALGO', 'BIN') 
    logfile = conf.get('ALGO', 'LOCAL_DIR') + 'err_' + str(slave_index)
    pid = run('''(nohup %s/main/algos/task/node_task %s %d %d %s %s %s %s 1> %s 2>&1 < /dev/null &
      echo $!)'''%(bin_dir, configFile, slave_index, numJobs, partCfg[slave_index][0], partCfg[slave_index][1], partCfg[slave_index][2], partCfg[slave_index + 1][2], logfile), pty = False)
    storePid(env.host[0], pid)
  

@task
def makePartition():
  with settings(host_string=MASTER_HOST):
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

def checkProcess(pid):
  return not bool(int(run('ps -p ' + pid + ' >/dev/null 2>&1; echo -n $?')))


def pagerankInversePartition():
  with settings(host_string=MASTER_HOST):
    global conf

    bin_dir = conf.get('ALGO', 'BIN')
    inputData = conf.get('ALGO', 'INPUT_DATA')
    prPartitionDir = conf.get('ALGO', 'REMOTE_DIR')
    inversePartDir = conf.get('ALGO', 'INVERSE_PARTITION_DIR')
    slaveryCfg = conf.get('ALGO', 'SLAVE_CONFIG')
    rowLen = conf.get('PREPROCESS', 'ROWLEN')
    run('%s/main/common/tools/inverse_partition_maker_job %s %s %s %d %s'%
      (bin_dir, inputData, inversePartDir, slaveryCfg, numJobs, rowLen))

def pagerankInversePreprocess():
  with settings(host_string=MASTER_HOST):
    global conf, cfg_hosts

    # Copying slavery config to remote nodes
    slaveryFile = conf.get('ALGO', 'SLAVERY_CFG')
    remoteDir = conf.get('ALGO', 'REMOTE_DIR')
    localDir = conf.get('ALGO', 'LOCAL_DIR')
    put(remoteDir + '/' + slaveryFile, localDir)
    inversePartDir = conf.get('ALGO', 'INVERSE_PARTITION_DIR')

    # making inverse parts
    for i in xrange(numJobs):
      run('mkdir -p %s/part_%d'%(inversePartDir, i))
    pagerankInversePartition()
  
@task
def preprocess():
  global conf
  with  shell_env(LD_LIBRARY_PATH='/home/kisstom/git/DistributedComp/DistributedFrame/src/dep/gmp/lib/:/home/kisstom/git/DistributedComp/DistributedFrame/src/dep/log4cpp/lib/'):
    cleanup()   
    if conf.has_section('PREPROCESS'):
      if conf.has_option('PREPROCESS', 'MAKE_PARTITION'):
        makePart = conf.get('PREPROCESS', 'MAKE_PARTITION')
        if makePart == '1':
          makePartition()

      preprocessType = conf.get('PREPROCESS', 'TYPE')
      if preprocessType == 'PAGERANK_INVERSE':
        pagerankInversePreprocess()
      else:
        print 'Error unknown type of preprocess:', preprocessType

@task
def compute():
  with  shell_env(LD_LIBRARY_PATH='/home/kisstom/git/DistributedComp/DistributedFrame/src/dep/gmp/lib/:/home/kisstom/git/DistributedComp/DistributedFrame/src/dep/log4cpp/lib/'):
    cleanup()
    mainCompute()

@task
def computeAll():
  with  shell_env(LD_LIBRARY_PATH='/home/kisstom/git/DistributedComp/DistributedFrame/src/dep/gmp/lib/:/home/kisstom/git/DistributedComp/DistributedFrame/src/dep/log4cpp/lib/'):
    
    preprocess()
    mainCompute()

def mainCompute():
  global conf
  with  shell_env(LD_LIBRARY_PATH='/home/kisstom/git/DistributedComp/DistributedFrame/src/dep/gmp/lib/:/home/kisstom/git/DistributedComp/DistributedFrame/src/dep/log4cpp/lib/'):

    storePartitionCfg()
    copyCfg()
    startMaster()
    startNodes() 
    waitForFinish()


@task
def crawlExperiment():
  global crawlMaxNodes, conf, BASE_LOCAL_DIR, BASE_CONCAT_DIR
  buildCrawlMaxNodes()
  for it, maxNode in enumerate(crawlMaxNodes):
    local_dir = BASE_LOCAL_DIR + "crawl_" + str(it) + "/"
    concat_dir = BASE_CONCAT_DIR + "crawl_" + str(it) + "/"

    master_log = local_dir + "master.log"
    conf.set("ALGO", "LOCAL_DIR", local_dir)
    conf.set("ALGO", "CONCAT_DIR", concat_dir)
    conf.set("ALGO", "MASTER_LOG", master_log)
    conf.set("NODE", "MAX_NODE_TO_KEEP", maxNode)

    cleanup()
    mainCompute()

  #concat()

def buildCrawlMaxNodes():
  global crawlMaxNodes, conf
  section = 'CRAWL_EXPERIMENT'
  options = conf.options(section)
  for option in options:
    maxNodeToKeep = int(conf.get(section, option))
    crawlMaxNodes += [maxNodeToKeep]


# Not working.
@task
def concat():
  global conf, cfg_hosts
  section = 'ALGO'
  concat_dir = conf.get(section, 'CONCAT_DIR')
  remote_local_dir = conf.get(section, 'LOCAL_DIR')
  run('mkdir -p %s'%concat_dir)
  for host in cfg_hosts:
    env.rolesdefs['server'] = [host]
    copyFromMachineTo(remote_local_dir, concat_dir)

  run('cat %s/out* > %s/concat'%(concat_dir, concat_dir))

@task
def gitInfo():
  global conf
  localDir = conf.get('ALGO', 'LOCAL_DIR')
  scriptDir = conf.get('ALGO', 'SCRIPTDIR')
  gitLog = localDir + '/gitlog.txt'
  f = open(gitLog, 'w')

  with cd(scriptDir):
    print run("""git diff --quiet --exit-code || """ +
      """(echo "ERROR: the current state of the git repository is not committed"; exit 42)""")
    out = run("""git log -1 --pretty=format:%H""")
    print out
    f.write(str(out))

  f.close()

@task
def bitpropExperiment():
  global conf
  est_index = 0
  epsilon = float(conf.get('NODE', 'EPSILON'))
  BASE_LOCAL_DIR = conf.get('ALGO', 'LOCAL_DIR')

  numEstimations = 1
  if conf.has_option('NODE', 'NUM_ESTIMATIONS'):
    numEstimations = conf.get('NODE', 'NUM_ESTIMATIONS')

  for i in xrange(numEstimations):
    local_dir = BASE_LOCAL_DIR + '/est' + str(est_index) + '/'
    prev_dir = BASE_LOCAL_DIR + '/est' + str(int(est_index - 1)) + '/'

    conf.set("ALGO", "LOCAL_DIR", local_dir)
    conf.set("NODE", "PREV_OUTDIR", prev_dir)
    conf.set('NODE', 'EST_INDEX', est_index)
    conf.set('NODE', 'EPSILON', epsilon)

    epsilon /= 2
    est_index += 1

    cleanup()
    mainCompute()



