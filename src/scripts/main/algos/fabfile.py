from fabric.api import run, env, sudo, cd, task, settings, roles, execute, parallel
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
global pids
global crawlMaxNodes
global BASE_LOCAL_DIR
global BASE_CONCAT_DIR
global MASTER_LOG

configFile = None
isConfReaded = False
cfg_hosts = []
numJobs = 0
pids = dict()
crawlMaxNodes = []
BASE_LOCAL_DIR = ""
BASE_CONCAT_DIR = ""
MASTER_HOST = ''

@task
def yes():
  run('screen -d -m "yes" > /dev/null < /dev/null ')

"""
  ####################################################
  ########  TOOLS FOR INITIALIZING CONFIG ############
  ####################################################
"""

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

  if conf.has_option('ALGO', 'USER'):
    env.user = conf.get('ALGO', 'USER')
  else:
    env.user = 'kisstom'

  if conf.has_option('ALGO', 'KEY_FILENAME'):
    env.key_filename = conf.get('ALGO', 'KEY_FILENAME')

  if conf.has_option('NODE', 'REMOTE_DIR'):
    slaveryCfg = conf.get('NODE', 'REMOTE_DIR') + '/slavery.cfg'
    conf.set('NODE', 'SLAVERY_CFG', slaveryCfg)

  BASE_DIR = conf.get('ALGO', 'BASE_DIR')
  LOCAL_DIR = BASE_DIR + "/outdir/"

  conf.set('ALGO', 'LOCAL_DIR', LOCAL_DIR)

  BASE_LOCAL_DIR = LOCAL_DIR
  MASTER_HOST = conf.get('ALGO', 'MASTER_HOST')

  conf.set("ALGO", "MASTER_LOG", BASE_LOCAL_DIR + "master.log")

  buildHosts(conf)
  return conf

def readConfig(configFile):
  config = ConfigParser.RawConfigParser()
  config.optionxform = str
  config.read(configFile)
  return config

def buildHosts(cfg):
  global numJobs, cfg_hosts
  section = 'MACHINES'
  options = cfg.options(section)
  for option in options:
    numJobs += int(cfg.get(section, option))
    cfg_hosts += [option]
  env.hosts = [MASTER_HOST]

def listfiles():
  run('ls -l')

@task
def hello():
  env.hosts = cfg_hosts
  execute(listfiles)
  env.hosts = [MASTER_HOST]


"""
  ####################################################
  ########  TOOLS APPLIED TO ALL MACHINES ############
  ####################################################
"""

def cleanup():
  print env.hosts
  global conf, pids
  local_dir = conf.get('ALGO', 'LOCAL_DIR')
  run('rm -rf %s'%local_dir)
  run('mkdir -p %s'%local_dir) 
  pids.clear()

def copyCfg():
  global conf

  configFile = conf.get('ALGO', 'LOCAL_DIR') + 'deploy.ini'

  tempf = tempfile.mktemp()
  tempfO = open(tempf, 'w')
  conf.write(tempfO)
  tempfO.close()

  put(tempf, configFile)
  os.system('rm ' + tempf)


def createLocalDir():
  global conf
  local_dir = conf.get('ALGO', 'LOCAL_DIR')
  run('mkdir -p %s'%local_dir)

def createConcatDir():
  global conf
  concat_dir = conf.get('ALGO', 'CONCAT_DIR')
  run('mkdir -p %s'%concat_dir)

@task
def concat():
  global conf, cfg_hosts 
  concat_dir = conf.get('ALGO', 'CONCAT_DIR')
  run('mkdir -p %s'%concat_dir)

  slave_index = 0
  for host in cfg_hosts:
    jobs_on_host = int(conf.get('MACHINES', host))
    for x in xrange(jobs_on_host):
      concatFromHost(slave_index, host)
      slave_index += 1


def concatFromHost(slave_index, host):
  outfile = conf.get('ALGO', 'LOCAL_DIR') + '/out_' + str(slave_index)
  concat_dir = conf.get('ALGO', 'CONCAT_DIR')

  with settings(host_string=host):
    run('cat %s* >> %s/concat'%(outfile, concat_dir))

"""
  ####################################################
  ########  TOOLS FOR PREPROCESSING ############
  ####################################################
"""
def runPreprocessTask(section, function):
  global conf
  if conf.has_section('PREPROCESS'):
    if conf.has_option('PREPROCESS', section):
      sectionValue = conf.get('PREPROCESS', section)
      if sectionValue == '1':
        function()


@task
def preprocess():
  global conf
  runOnAllNodes(cleanup)

  bin_dir = conf.get('ALGO', 'BIN')
  depDir = bin_dir + '../dep/'
  libPath = depDir + 'gmp/lib/'
  libPath += ':' + depDir + 'log4cpp/lib/'

  with  shell_env(LD_LIBRARY_PATH=libPath):
    runPreprocessTask('MAKE_PARTITION', makePartition)
    runPreprocessTask('PUT_TO_LOCAL', putPartitionIfNeeded)
    runPreprocessTask('PAGERANK_INVERSE', pagerankInversePreprocess)
    runPreprocessTask('PUT_PAGERANK_TO_LOCAL', putPagerankInversePartitionIfNeeded)
    runPreprocessTask('OUTPARTITION_INDEX', outpartitionIndexCompute)
    runPreprocessTask('COUNTER_INVERSE', counterInversePreprocess)
    runPreprocessTask('RATING_MX_SPLITTER', ratingMxSplitter)
    runPreprocessTask('RATING_MX_PUT', putRatingSplitsToCluster)


########### For partitioning ##################

@task
def makePartition():
  with settings(host_string=MASTER_HOST):
    global conf, numJobs

    bin_dir = conf.get('ALGO', 'BIN')
    remoteDir =  conf.get('NODE', 'REMOTE_DIR')
    inputData =  conf.get('NODE', 'INPUT_DATA')
    slaveryCfg =  conf.get('NODE', 'SLAVERY_CFG')
    initSlavePort =  conf.get('ALGO', 'INIT_SLAVE_PORT')
    try:
      splitType = conf.get('ALGO', 'SPLIT_TYPE')
    except Exception:
      splitType = ''

    run('mkdir -p %s'%remoteDir)
    if splitType == '' or splitType == 'EDGE_BALANCED':
      print 'Using partitioner edge balanced'
      numEdgePerPart = getNumEdgePerPart()
      run('%s/main/common/tools/split_clueweb_vector_node  %s %s %d %s %s'%(bin_dir, inputData, initSlavePort, numEdgePerPart, remoteDir + 'slavery', slaveryCfg))
 
 
    elif splitType == 'NODE_BALANCED':
      print 'Using partitioner node balanced'
      numNodePerPart = getNumNodePerPart()
      logFile = conf.get('ALGO', 'LOCAL_DIR') + '/err'
      run('%s/main/common/graph_converter/split_by_row_job %s %s %s %d %s'%(bin_dir, inputData, logFile, remoteDir + 'slavery', numNodePerPart, slaveryCfg))
    

def putPartitionIfNeeded():
  global conf, numJobs
  try:
    putToLocal = conf.get('PREPROCESS', 'PUT_TO_LOCAL')
  except Exception as e:
    print 'Exception %s' % e
    return 

  if putToLocal == '0': return

  slave_index = 0
  for host in cfg_hosts:
      if host == MASTER_HOST:
        slave_index += int(conf.get('MACHINES', host))
        continue
      jobs_on_host = int(conf.get('MACHINES', host))
      prPartitionDir = conf.get('NODE', 'REMOTE_DIR')

      createRemoteDir(host)
      putOldSplitConfigOnMachine(host)
      for x in xrange(jobs_on_host):
        putOnMachine(slave_index, host)
        slave_index += 1

def putOnMachine(slave_index, host):
  with settings(host_string=host):
    remoteDir = conf.get('NODE', 'REMOTE_DIR')
    run('mkdir -p %s'%remoteDir)
    partitionFile = conf.get('NODE', 'REMOTE_DIR') + '/slavery_' + str(slave_index) + '.txt'
    put(partitionFile, remoteDir)

def putOldSplitConfigOnMachine(host):
  global conf

  with settings(host_string=host):
    remoteDir = conf.get('NODE', 'REMOTE_DIR')
  
    configFile = conf.get('NODE', 'SLAVERY_CFG')
    put(configFile, remoteDir)

########### Rating matrix splitter ################

def ratingMxSplitter():
  with settings(host_string=MASTER_HOST):
    global conf, numJobs
    
    createRemoteDir(MASTER_HOST)

    bin_dir = conf.get('ALGO', 'BIN')
    numEdgePerPart = getNumEdgePerPart()
    remoteDir =  conf.get('NODE', 'REMOTE_DIR')

    inputData =  conf.get('NODE', 'USER_RATING_DATA')
    configFile = remoteDir + '/user_part.cfg'
    prefix = remoteDir + '/user_part_'
    run('%s/main/common/tools/sparse_rating_splitter_tool  %s %d %s %s'%(bin_dir, prefix, numEdgePerPart, inputData, configFile))


    inputData =  conf.get('NODE', 'ITEM_RATING_DATA')
    configFile = remoteDir + '/item_part.cfg'
    prefix = remoteDir + '/item_part_'
    run('%s/main/common/tools/sparse_transposed_rating_splitter_tool  %s %d %s %s'%(bin_dir, prefix, numEdgePerPart, inputData, configFile))

def putRatingSplitsToCluster():
  global conf
  slave_index = 0
  for host in cfg_hosts:
      if host == MASTER_HOST:
        slave_index += int(conf.get('MACHINES', host))
        continue
      jobs_on_host = int(conf.get('MACHINES', host))

      createRemoteDir(host)
      putRatingSplitConfigOnMachine(host)
      for x in xrange(jobs_on_host):
        putRatingSplitOnMachine(slave_index, host)
        slave_index += 1

def putRatingSplitOnMachine(slave_index, host):
  global conf
  remoteDir = conf.get('NODE', 'REMOTE_DIR') 
  with settings(host_string=host):
    partitionFile = remoteDir + '/user_part_' + str(slave_index)
    put(partitionFile, remoteDir)

    partitionFile = remoteDir + '/item_part_' + str(slave_index)
    put(partitionFile, remoteDir)

def putRatingSplitConfigOnMachine(host):
  global conf
  with settings(host_string=host):
    remoteDir = conf.get('NODE', 'REMOTE_DIR')
  
    configFile = remoteDir + '/user_part.cfg'
    put(configFile, remoteDir)

    configFile = remoteDir + '/item_part.cfg'
    put(configFile, remoteDir)

def createRemoteDir(host):
  global conf
  with settings(host_string=host):
    remoteDir = conf.get('NODE', 'REMOTE_DIR')
    run('mkdir -p %s'%remoteDir)


########### For pagerank inverse ##################
def pagerankInversePreprocess():
  global conf, cfg_hosts
  with settings(host_string=MASTER_HOST):
    
    inversePartDir = conf.get('NODE', 'INVERSE_PARTITION_DIR')

    # making inverse parts
    for i in xrange(numJobs):
      run('mkdir -p %s/part_%d'%(inversePartDir, i))
    pagerankInversePartition()
#putPagerankInversePartitionIfNeeded()


def pagerankInversePartition():
  with settings(host_string=MASTER_HOST):
    global conf

    bin_dir = conf.get('ALGO', 'BIN')
    inputData = conf.get('NODE', 'INPUT_DATA')
    prPartitionDir = conf.get('NODE', 'REMOTE_DIR')
    inversePartDir = conf.get('NODE', 'INVERSE_PARTITION_DIR')
    slaveryCfg = conf.get('NODE', 'SLAVERY_CFG')

    rowLen = conf.get('PREPROCESS', 'ROWLEN')
    run('%s/main/common/tools/inverse_partition_maker_job %s %s %s %d %s'%
      (bin_dir, inputData, inversePartDir, slaveryCfg, numJobs, rowLen))

def putPagerankInversePartitionIfNeeded():
  global conf, numJobs
  try:
    putToLocal = conf.get('PREPROCESS', 'PUT_PAGERANK_TO_LOCAL')
  except Exception as e:
    print 'Exception %s' % e
    return 

  if putToLocal == '0': return

  slave_index = 0
  for host in cfg_hosts:
      if host == MASTER_HOST:
        slave_index += int(conf.get('MACHINES', host))
        continue

      jobs_on_host = int(conf.get('MACHINES', host))
      for x in xrange(jobs_on_host):
        putInverseOnMachine(slave_index, host)
        slave_index += 1


def putInverseOnMachine(slave_index, host):
  with settings(host_string=host):
    inversePartDir = conf.get('NODE', 'INVERSE_PARTITION_DIR') + '/part_' + str(slave_index)
    run('mkdir -p %s'%inversePartDir)
    boundFile = inversePartDir + '/bound.txt'
    edgesFile = inversePartDir + '/edges.txt'
    put(boundFile, inversePartDir)
    put(edgesFile, inversePartDir)

########### Out partition index compute ##################

@task
def outpartitionIndexCompute():
  global conf, numJobs

  outPartIndicesDir = conf.get('NODE', 'OUT_PARTITION_INDICES_DIR')
  run('mkdir -p %s'%outPartIndicesDir)

  for slave_index in xrange(numJobs):
      outpartitionIndexComputeOnePart(slave_index)

@parallel
def outpartitionIndexComputeOnePart(slave_index):
  global numJobs, conf
  rowLen = conf.get('PREPROCESS', 'ROWLEN')
  outPartIndicesDir = conf.get('NODE', 'OUT_PARTITION_INDICES_DIR')
  slaveryCfg = conf.get('NODE', 'SLAVERY_CFG')
  with settings(host_string=MASTER_HOST):
    command = '%s/main/common/tools/outpartition_index_as_edgelist_computer '%conf.get('ALGO', 'BIN')
    command += '%s/slavery_%d.txt ' % (conf.get('NODE', 'REMOTE_DIR'), slave_index)
    command += '%s %d %s %d ' % (slaveryCfg, numJobs, rowLen, slave_index)
    command += '%s/outpartition_indices_%s.txt '%(outPartIndicesDir, slave_index)
    command += '%s/neighbors_file_%s.txt'%(outPartIndicesDir, slave_index)
    run(command)


########### Counter inverse ##################

def counterInversePreprocess():
  runOnAllNodes(lambda : run('mkdir -p %s'%conf.get('NODE', 'COUNTER_INVERSE_OUTPUT_DIR')))
  runOnAllNodes(lambda : run('mkdir -p %s'%conf.get('NODE', 'PARTITION_BOUNDS_DIR')))


"""
  ####################################################
  ########  TOOLS FOR SETTING UP CLUSTER ############
  ####################################################
"""


def startMaster():
    global conf, numJobs, pids
    iniFile = conf.get('ALGO', 'LOCAL_DIR') + 'deploy.ini'
    with settings(host_string=MASTER_HOST):
      bin_dir = conf.get('ALGO', 'BIN')
      master_log = conf.get('ALGO', 'MASTER_LOG')
      run('rm -f %s'%master_log)
      logfile = conf.get('ALGO', 'LOCAL_DIR') + 'err_master'
      pid = run('''(nohup %s/main/algos/task/master_task %s %d 1> %s 2>&1 < /dev/null & 
        echo $!)'''%(bin_dir, iniFile, numJobs, logfile), pty = False)
      storePid(MASTER_HOST, pid)
      time.sleep(2)

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
  global numJobs, conf
  iniFile = conf.get('ALGO', 'LOCAL_DIR') + 'deploy.ini'
  bin_dir = None
  if conf.has_option('ALGO', 'REMOTE_BIN'):
    bin_dir = conf.get('ALGO', 'REMOTE_BIN') 
  else:
    bin_dir = conf.get('ALGO', 'BIN') 

  with settings(host_string=host):
    logfile = conf.get('ALGO', 'LOCAL_DIR') + 'err_' + str(slave_index)
    pid = run('''(nohup %s/main/algos/task/node_task %s %d %d 1> %s 2>&1 < /dev/null &
      echo $!)'''%(bin_dir, iniFile, slave_index, numJobs, logfile), pty = False)
    storePid(host, pid)

def mainCompute():
  global conf, MASTER_HOST
  debug = False
  if conf.has_option('ALGO', 'DEBUG'):
    debug = True

  bin_dir = conf.get('ALGO', 'BIN')
  depDir = bin_dir + '../dep/'
  libPath = depDir + 'gmp/lib/'
  libPath += ':' + depDir + 'log4cpp/lib/'
  libPath += ':' + depDir + 'zmq/lib/'
  libPath += ':' + depDir + 'gsl/lib/'
  libPath += ':' + depDir + 'cpplibs/'

  with  shell_env(LD_LIBRARY_PATH=libPath):
    copyIniToAll()
    createLocalDir()
    gitInfo(debug)
    execute(copyCfg)
    startMaster()
    startNodes() 
    waitForFinish()

@task
def compute():
    runOnAllNodes(cleanup)
    mainCompute()

@task
def computeAll():    
    preprocess()
    mainCompute()


"""
  ####################################################
  ########  HELPER MATHODS FOR RUNNING CLUSTER #######
  ####################################################
"""

def storePid(host, pid):
  global pids
  if host not in pids:
    pids[host] = []

  pids[host] += [pid]

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

def getNumNodePerPart():
    global conf, numJobs

    numNode = int(conf.get('NODE', 'NUMLINE'))
    if numNode % numJobs:
      numNodePerPart = numNode // numJobs + 1
    else:
      numNodePerPart = numNode // numJobs

    return numNodePerPart


def getNumEdgePerPart():
    global conf, numJobs

    numEdge = int(conf.get('NODE', 'NUMEDGE'))
    if numEdge % numJobs:
      numEdgePerPart = numEdge // numJobs + 1
    else:
      numEdgePerPart = numEdge // numJobs

    return numEdgePerPart

def checkProcess(pid):
  return not bool(int(run('ps -p ' + pid + ' >/dev/null 2>&1; echo -n $?')))


def runOnAllNodes(function):
  env.hosts = cfg_hosts
  execute(function)
  env.hosts = [MASTER_HOST]

@task
def gitInfo(debug):
  global conf
  localDir = conf.get('ALGO', 'LOCAL_DIR')
  scriptDir = conf.get('ALGO', 'SCRIPTDIR')
  gitLog = localDir + '/gitlog.txt'

  tempf = tempfile.mktemp()
  tempfO = open(tempf, 'w')

  with cd(scriptDir):
    if not debug:
      run("""git diff --quiet --exit-code || """ +
        """(echo "ERROR: the current state of the git repository is not committed"; exit 42)""")
    out = run("""git log -1 --pretty=format:%H""")
    tempfO.write(out.strip())

  tempfO.close()
  put(tempf, gitLog)
  os.system('rm ' + tempf)


"""
  ####################################################
  ######## ALGOS WITH ITERATED COMPUTATION #######
  ####################################################
"""

@task
def crawlExperiment():
  preprocess()

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

    runOnAllNodes(cleanup)
    mainCompute()

def buildCrawlMaxNodes():
  global crawlMaxNodes, conf
  section = 'CRAWL_EXPERIMENT'
  options = conf.options(section)
  for option in options:
    maxNodeToKeep = int(conf.get(section, option))
    crawlMaxNodes += [maxNodeToKeep]

@task
def bitpropExperiment():
  global conf
  preprocess()
  est_index = 0
  epsilon = float(conf.get('NODE', 'EPSILON'))
  BASE_LOCAL_DIR = conf.get('ALGO', 'LOCAL_DIR')

  numEstimations = 1
  if conf.has_option('NODE', 'NUM_ESTIMATIONS'):
    numEstimations = int(conf.get('NODE', 'NUM_ESTIMATIONS'))

  for i in xrange(numEstimations):
    local_dir = BASE_LOCAL_DIR + '/est' + str(est_index) + '/'
    prev_dir = BASE_LOCAL_DIR + '/est' + str(int(est_index - 1)) + '/'

    conf.set("ALGO", "LOCAL_DIR", local_dir)
    conf.set("NODE", "PREV_OUTDIR", prev_dir)
    conf.set('NODE', 'EST_INDEX', est_index)
    conf.set('NODE', 'EPSILON', epsilon)

    epsilon /= 2
    est_index += 1

    runOnAllNodes(cleanup)
    mainCompute()


@task
def copyIniToAll():
  runOnAllNodes(copyCfg)
