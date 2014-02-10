/*
 * algo.cpp
 *
 *  Created on: 2013.08.09.
 *      Author: kisstom
 */

#include "algo.h"


Algo::Algo(char* master_host, int master_port, int slave_port,
			int send_limit, long all_node, int num_slaves, int slave_index,
			long num_nodes, long min_node) {
	strcpy(master_host_, master_host);
	//strcpy(logfile_name_pref_, logfile_name_pref);
	master_port_ = master_port;
	slave_port_ = slave_port;
	send_limit_ = send_limit;
	all_node_ = all_node;
	current_iteration_ = 0;
	num_slaves_ = num_slaves;
  slave_index_ = slave_index;
  num_nodes_ = num_nodes;
  min_node_ = min_node;
  logger_ = &log4cpp::Category::getInstance(std::string("Algo"));
  //startLogger();
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


// TODO ez nem lep ki
bool Algo::setUp() {
	logger_->info("setting up node");

	try {

		socketManager_->initClient(slave_port_);
		socketManager_->connectToMaster(master_host_, master_port_);

		initFromMaster();

		socketManager_->initSockets(num_slaves_);
		senderBuffer_->resizeBufferNum(num_slaves_);
		senderBuffer_->resizeBuffers(send_limit_);
		storeFromBinary_->resizeSocketNum(num_slaves_);
		storeFromBinary_->setBufferCapacity(send_limit_ * 2);

		socketManager_->sendReadyToMaster();
		socketManager_->initConnections();

	} catch (MasterException& e) {
    logger_->info("Master said i must die. I die.");
    return false;
  }
  catch (LogError& e) {
  	logger_->info("Error: %s.\nExiting.", e.what());
  	socketManager_->sendFailToMaster();
    return false;
  }
  catch (ConnectionError& e) {
    logger_->info("Error: %s.\nExiting.", e.what());
  	socketManager_->sendFailToMaster();
    return false;
  }

  return true;
}

void Algo::run()
{
	logger_->info("Starting run.");
  char instr[1024];
  try {
  	//setUp();
    //bool exit = false;
    while (1)
    {

    	socketManager_->recvFromMaster(1024, instr);
    	if (!strcmp(instr, "exit")) {
    		break;
    	}
    	if (!strcmp(instr, "die")) {
    		throw MasterException();
    	}

    	node_->beforeIteration();
    	runThreads();
    	bool cont = node_->afterIteration();

    	if (cont) {
    	  socketManager_->sendReadyToMaster();
    	} else {
    		socketManager_->sendEmptyToMaster();
    	}
    }
  }
  catch (MasterException& e) {
    //log_err(logfile_, "Master said i must die. I die.");
    return;
  }
  catch (LogError& e) {
    //log_err(stderr, "Error: %s.\nExiting.", e.what());
  	socketManager_->sendFailToMaster();
    return;
  }
  catch (ConnectionError& e) {
    //log_err(logfile_, "Error: %s.\nExiting.", e.what());
  	socketManager_->sendFailToMaster();
    return;
  }

  //log_info(logfile_, "Finished at iteration %d.", current_iteration_);

  // destructor should destroy and send to master
}

void Algo::final() {
	 node_->final();
	 logger_->info("Algo finished.");
}

void Algo::receiver() {
	logger_->info("Starting receiver.");
	int finished = 0, socket_index, size = 0;
	bool is_more = true;
	Selector selector;
	selector.Init(socketManager_->getReceiverSockets());
	while (1)
	{
		socket_index = selector.SelectIndex();
		size = socketManager_->recvFromNode(send_limit_, storeFromBinary_->getEndOfBufferAt(socket_index), socket_index);
		/*if (storeFromBinary_->getRemainsSize(socket_index) + size > 2 * send_limit_) {
			logger_->error("Memory overlaping socket %d with size %d", socket_index,
					storeFromBinary_->getRemainsSize(socket_index) + size - 2 * send_limit_);
		}*/

		storeFromBinary_->remains_size_[socket_index] += (unsigned) size;
		is_more = storeFromBinary(socket_index);

		if (!is_more)
		{
			++finished;
			// TODO -1?
			if (finished == num_slaves_ - 1)
			{
				break;
			}
		}
	}
	logger_->info("Receiver finished.");
	//logger_->info("remains size %d", storeFromBinary_->remains_size_[socket_index]);
}

bool Algo::storeFromBinary(int socket_index) {
	return storeFromBinary_->storeFromIndex(socket_index);
}

void Algo::initFromMaster() {
	char buf[send_limit_];
	int size;
	size = socketManager_->recvFromMaster(1024, buf);

	if (strcmp(buf, "die") == 0) throw MasterException();
	stringstream ss (stringstream::in | stringstream::out);
	ss << buf;
	long actMin;
	while (ss.peek() != EOF) {
		ss >> actMin;
		partition_min_node_.push_back(actMin);
	}
	part_index_ = getPartitionIndex(min_node_);

	node_->setPartitionIndex(part_index_);

	if (ss.tellg() != -1) {
		node_->initFromMaster(ss.str().substr(ss.tellg()));
	} else {
		node_->initFromMaster("");
	}
}

void Algo::runThreads() {
	logger_->info("run threads");
	char rec[1024] = "receiver";
	ReceiverThread *receiver = new ReceiverThread(this);
	char sen[1024] = "sender";
	SenderThread *sender = new SenderThread(node_);
	receiver->start();
	sender->start();
	receiver->waitForThread();
	sender->waitForThread();

	logger_->info("threads ended");
	delete receiver;
	delete sender;
}


void Algo::setNode(Node * node) {
	node_ = node;
}

void Algo::setSocketManager(SocketManager* manager) {
	socketManager_ = manager;
	senderBuffer_->setSocketManager(manager);
}

void Algo::setStoreFromBinary(StoreFromBinary* storeFromBinary) {
	storeFromBinary_ = storeFromBinary;
}

void Algo::setSenderBuffer(SenderBuffer* senderBuffer) {
	senderBuffer_ = senderBuffer;
	node_->setSenderBuffer(senderBuffer);
}

void Algo::sendAndSignal(int self_index) {
	for (int part_index = 0; part_index < (int) senderBuffer_->pack_.size(); ++part_index) {
		if (part_index == self_index) continue;

		if (!senderBuffer_->canAdd(part_index , 1)) {
	    senderBuffer_->emptyBuffer(part_index);
		}

    senderBuffer_->setFinish(part_index);
    senderBuffer_->emptyBuffer(part_index);
	}
}

Algo::~Algo() {
	//delete storeFromBinary_;
}
