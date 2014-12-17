/*
 * algo.cpp
 *
 *  Created on: 2013.08.09.
 *      Author: kisstom
 */

#include "algo.h"
#include "../../common/thread/run_thread.h"


Algo::Algo(char* master_host, int master_port, int slave_port,
			int send_limit, long all_node, int num_slaves, int slave_index,
			long num_nodes, long min_node, bool _isMulticast) {
	strcpy(master_host_, master_host);
	master_port_ = master_port;
	slave_port_ = slave_port;
	send_limit_ = send_limit;
	all_node_ = all_node;
	num_slaves_ = num_slaves;
  slave_index_ = slave_index;
  num_nodes_ = num_nodes;
  min_node_ = min_node;
  logger_ = &log4cpp::Category::getInstance(std::string("Algo"));

  isMulticast = _isMulticast;

  masterSocketManager_ = NULL;
  socketManager_ = NULL;
  //clientSocketManager_ = NULL;
  senderBuffer_ = NULL;
  node_ = NULL;
  storeFromBinary_ = NULL;
}

int Algo::getSlaveIndex() {
	return slave_index_;
}

int Algo::getPartitionIndex(long node) {
  for (int i = partition_min_node_.size() - 1; i >= 0; --i) {
    if (partition_min_node_[i] <= node) return i;
  }
  return 0;
}

long Algo::getPartitionStartNode(int part_index) {
	if (part_index < 0) return -1;
	if (part_index >= (int) partition_min_node_.size()) {
		return partition_min_node_[partition_min_node_.size() - 1] + num_nodes_;
	}

	return partition_min_node_[part_index];
}

short Algo::getNumberOfPartitions() {
	return (short) partition_min_node_.size();
}

bool Algo::setUp() {
	logger_->info("setting up node");

	int numSockets = -1;
	try {
	  // Connecting to master.
		masterSocketManager_->connectToMaster(master_host_, master_port_);

		// Getting config. Should be deleted.
		initFromMaster();

		// Waiting for all nodes to finish.
		masterSocketManager_->sendReadyToMaster();

		// Initing connections between nodes.
		// Ready to master is included.
		socketManager_->initConnections();

    // Setting up the client communication manager.
		//clientSocketManager_->setUp();
	} catch (MasterException& e) {
    logger_->info("Master said i must die. I die.");
    return false;
  }
  catch (LogError& e) {
  	logger_->info("Error: %s.\nExiting.", e.what());
  	masterSocketManager_->sendFailToMaster();
    return false;
  }
  catch (ConnectionError& e) {
    logger_->info("Error: %s.\nExiting.", e.what());
    masterSocketManager_->sendFailToMaster();
    return false;
  }

  logger_->info("Algo setup finished.");
  return true;
}

void Algo::run()
{
	logger_->info("Starting run.");
  char instr[1024];
  try {
    while (1)
    {
      logger_->info("Algo waiting for instruction from master.");
      // Sync with run threads in master.
      masterSocketManager_->recvFromMaster(1024, instr);
    	if (!strcmp(instr, "exit")) {
    		break;
    	}
    	if (!strcmp(instr, "die")) {
    		throw MasterException();
    	}

    	logger_->info("Recieving instruction %s.", instr);
    	logger_->info("Sending ready for master.");
    	masterSocketManager_->sendReadyToMaster();

    	// Sync with inner master nextIter.
    	logger_->info("Waiting for before iteration instr.");
    	masterSocketManager_->recvFromMaster(1024, instr);

    	logger_->info("Received instr %s.", instr);
    	node_->beforeIteration(instr);
    	//clientSocketManager_->resetFinishCounter();
    	runThreads();
    	bool cont = node_->afterIteration();

    	if (cont) {
    	  masterSocketManager_->sendReadyToMaster();
    	} else {
    	  masterSocketManager_->sendEmptyToMaster();
    	}
    }
  }
  catch (MasterException& e) {
    return;
  }
  catch (LogError& e) {
    masterSocketManager_->sendFailToMaster();
    return;
  }
  catch (ConnectionError& e) {
    masterSocketManager_->sendFailToMaster();
    return;
  }
}

void Algo::final() {
	 node_->final();
	 logger_->info("Algo finished.");
}

void Algo::receiver() {
	logger_->info("Starting receiver.");
	int finished = 0, socket_index, size = 0;
	bool is_more = true;
	int timeout = 0;
	Selector* selector = socketManager_->getSelector(timeout);

	socketManager_->resetFinishCount();
	while (1)
	{
		socket_index = selector->SelectIndex();
		if (socket_index >= 0) {
		  size = socketManager_->recvFromNode(
		      send_limit_, storeFromBinary_->getEndOfBufferAt(socket_index), socket_index);

		  storeFromBinary_->remains_size_[socket_index] += (unsigned) size;
		  is_more = storeFromBinary(socket_index);

		  if (!is_more) {
		    socketManager_->finishedSocket(socket_index);
		  }

		  if (socketManager_->isFinishedAll()) break;
		}

		//if (clientSocketManager_->isFinished()) break;
	}

	delete selector;
	logger_->info("Finished receiver.");
}

bool Algo::storeFromBinary(int socket_index) {
	return storeFromBinary_->storeFromIndex(socket_index);
}

void Algo::initFromMaster() {
	char buf[send_limit_];
	int size;
	size = masterSocketManager_->recvFromMaster(1024, buf);

	if (strcmp(buf, "die") == 0) throw MasterException();
	stringstream ss (stringstream::in | stringstream::out);
	ss << buf;
	long actMin;
	for (int i = 0; i < num_slaves_; ++i) {
		ss >> actMin;
		partition_min_node_.push_back(actMin);
	}

	node_->setPartitionIndex(slave_index_);

	if (ss.tellg() != -1) {
		node_->initFromMaster(ss.str().substr(ss.tellg()));
	} else {
		node_->initFromMaster("");
	}
}

void Algo::runThreads() {
	logger_->info("run threads");
	ReceiverThread *receiver = new ReceiverThread(this);
	SenderThread *sender = new SenderThread(node_);
	//RunThread* slaveCommunication = new RunThread(clientSocketManager_);

	receiver->start();
	sender->start();
	//slaveCommunication->start();
	receiver->waitForThread();
	sender->waitForThread();
	//slaveCommunication->waitForThread();

	logger_->info("threads ended");
	delete receiver;
	delete sender;
	//delete slaveCommunication;
}


void Algo::setNode(Node * node) {
	node_ = node;
}

void Algo::setSocketManager(ISocketManager* manager) {
	socketManager_ = manager;
	senderBuffer_->setSocketManager(manager);
}

void Algo::setMasterSocketManager(MasterSocketManager* manager) {
  masterSocketManager_ = manager;
}

/*void Algo::setClientSocketManager(ClientSocketManager* manager) {
  clientSocketManager_ = manager;
}*/

void Algo::setStoreFromBinary(StoreFromBinary* storeFromBinary) {
	storeFromBinary_ = storeFromBinary;
}

void Algo::setSenderBuffer(SenderBuffer* senderBuffer) {
	senderBuffer_ = senderBuffer;
	node_->setSenderBuffer(senderBuffer);
}

void Algo::sendAndSignal(int self_index) {
  logger_->info("Sending end signal.");
	for (int part_index = 0; part_index < (int) senderBuffer_->getBufferNum(); ++part_index) {
	  if (!isMulticast && part_index == self_index) continue;

	  if (!senderBuffer_->canAdd(part_index, 1)) {
	    senderBuffer_->emptyBuffer(part_index);
	  }

	  senderBuffer_->setFinish(part_index);
    senderBuffer_->emptyBuffer(part_index);
	}

	//clientSocketManager_->publishEndSignal();
}

long Algo::getNumberOfPartitionNodes() {
  return num_nodes_;
}

long Algo::getAllNodes() {
  return all_node_;
}

long Algo::getMinnode() {
  return min_node_;
}


Algo::~Algo() {
}
