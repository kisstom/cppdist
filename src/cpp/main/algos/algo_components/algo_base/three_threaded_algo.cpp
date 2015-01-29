#include "three_threaded_algo.h"

#include "../../../common/thread/receiver_thread.h"
#include "../../../common/thread/sender_thread.h"
#include "../../../common/thread/thread_manager.h"
#include "../../../common/thread/run_thread.h"

ThreeThreadedAlgo::ThreeThreadedAlgo(unordered_map<string, string>* _params):
AlgoBase(_params) {
  logger_ = &log4cpp::Category::getInstance(std::string("ThreeThreadedAlgo"));
  clientSocketManager_ = NULL;
}

bool ThreeThreadedAlgo::setUp() {
  logger_->info("setting up node");

  try {
    // Connecting to master.
    masterSocketManager_->connectToMaster(master_host_, master_port_);

    socketManager_->initConnections();

    clientSocketManager_->setUp();
  } catch (MasterException& e) {
    logger_->info("Master said i must die. I die.");
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

void ThreeThreadedAlgo::run() {
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
      clientSocketManager_->resetFinishCounter();
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
  catch (ConnectionError& e) {
    masterSocketManager_->sendFailToMaster();
    return;
  }
}

void ThreeThreadedAlgo::receiver() {
  logger_->info("Starting receiver.");
  int socket_index, size = 0;
  int timeout = 20;
  Selector* selector = socketManager_->getSelector(timeout);

  while (1)
  {
    socket_index = selector->SelectIndex();
    if (socket_index >= 0) {
      size = socketManager_->recvFromNode(
          send_limit_, storeFromBinary_->getEndOfBufferAt(socket_index), socket_index);

      storeFromBinary_->remains_size_[socket_index] += (unsigned) size;
      storeFromBinary(socket_index);
    }

    if (clientSocketManager_->isFinished()) break;
  }

  delete selector;
  logger_->info("Finished receiver.");
}

void ThreeThreadedAlgo::runThreads() {
  logger_->info("run threads");
  ReceiverThread *receiver = new ReceiverThread(this);
  SenderThread *sender = new SenderThread(node_);
  RunThread* slaveCommunication = new RunThread(clientSocketManager_);

  receiver->start();
  sender->start();
  slaveCommunication->start();
  slaveCommunication->waitForThread();
  sender->waitForThread();
  receiver->waitForThread();

  logger_->info("threads ended");
  delete receiver;
  delete sender;
  delete slaveCommunication;
}

void ThreeThreadedAlgo::sendAndSignal(int self_index) {
  logger_->info("Sending end signal.");
  for (int part_index = 0; part_index < (int) senderBuffer_->getBufferNum(); ++part_index) {
    senderBuffer_->emptyBuffer(part_index);
  }

  clientSocketManager_->publishEndSignal();
}

void ThreeThreadedAlgo::setClientSocketManager(ClientSocketManager* clientSocketManager) {
  clientSocketManager_ = clientSocketManager;
}
