#include <cstring>
#include <sstream>
#include "./master_base.h"

using std::vector;

MasterBase::MasterBase(int master_port, vector<Slave>* slaves)
{
  master_port_ = master_port;
  slaves_ = slaves;
  logger_ = &log4cpp::Category::getInstance(std::string("Master"));
  master_socket_ = NULL;
  innerMaster_ = NULL;
}

MasterBase::~MasterBase()
{
  delete master_socket_;
  for (int i = 0; i < (int) slaves_->size(); ++i) {
    if ((*slaves_)[i].socket) delete (*slaves_)[i].socket;
  }
}

bool MasterBase::setUp() {
  logger_->info("Setting up master.");
  try {
    InitServer();

    WaitForAccepts();

    MakeNodeConnections();
  } catch (ConnectionError& e) {
    logger_->info("Error: %s. Killing nodes.", e.what());
    KillNodes();
    return false;
  }
  logger_->info("Master setup finished.");
  return true;
}

void MasterBase::InitServer() {
  logger_->info("Creating master server at port %d.", master_port_);
  master_socket_ = ServerSocket::Create(master_port_);
  logger_->info("Created master server at port %d.", master_port_);
}

void MasterBase::KillNodes() {
  logger_->info("Killing nodes.");
  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    (*slaves_)[i].socket->Send(4, "die");
  }
}

void MasterBase::run()
{
  logger_->info("Starting run.");
  bool cont = true;
  bool retval = true;
  try {
    while (cont) {
      // Telling nodes to start next iter.
      RunThreads();
      WaitForNodes();

      // Some special set up by the inner master
      retval = innerMaster_->nextIter();
      if (!retval) cont = retval;

      // Waiting for finish.
      retval = WaitForNodes();
      logger_->info("Retval %d", retval);
      if (!retval) cont = retval;
      logger_->info("Cont %d", cont);
      if (!cont) break;

    }
  } catch (ConnectionError& e) {
    KillNodes();
    return;
  }
  // Stopping nodes.
  Final();
  logger_->info("Master run finished.");
  // TODO waiting for full destroy of nodes
  //WaitForNodes();
}

void MasterBase::WaitForAccepts()
{
  logger_->info("Waiting for %d accepts.", (int)slaves_->size());
  char instr[1024], ip[1024], msg[1024];
  int port;
  SocketConnection* client_socket;
  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    bool success = false;
    client_socket = master_socket_->Accept();
    client_socket->Recv(1024, instr);
    if (sscanf(instr, "%d,%s", &port, ip) == EOF)
      throw NodeFailException("Bad instructions from node.");
    for (unsigned int j = 0; j <  slaves_->size(); ++j) {
      if ((*slaves_)[j].port == port) {
        (*slaves_)[j].socket = client_socket;
        strcpy((*slaves_)[j].ip, ip);
        logger_->info("Accepting connection from port %d ip %s.", port, ip);
        success = true;
      }
    }
    sprintf(msg, "Not valid port %d received from node.", port);
    if (!success) throw NodeFailException(msg);
  }
}


bool MasterBase::WaitForNodes()
{
  logger_->info("Waiting for nodes.");
  char msg[1024];
  unsigned finished = 0;
  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    (*slaves_)[i].socket->Recv(1024, msg);
    logger_->info("Received from slave %s", msg);
    if (strcmp(msg, "empty") == 0) ++finished;
    else if (strcmp(msg, "ready")) {
      logger_->info("Error! Received from node %d msg: %s.", i, msg);
      throw NodeFailException("Node failing while master waiting for it.");
    }
    logger_->info("Slave %d finished iteration.", i);
  }
  logger_->info("Number of finished: %d numslaves %d", finished, (int)slaves_->size());
  return (finished != slaves_->size()) ;
}

void MasterBase::RunThreads()
{
  logger_->info("Sending to nodes start threading.");
  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    (*slaves_)[i].socket->Send(7, "thread");
  }
}

void MasterBase::Final()
{
  logger_->info("Stop nodes.");
  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    (*slaves_)[i].socket->Send(5, "exit");
    // close socket TODO
  }
}

void MasterBase::setInnerMaster(InnerMaster* innerMaster) {
  innerMaster_ = innerMaster;
}

void MasterBase::sendMessageForAllNodes(char* msg) {
  logger_->info("Sending msg to nodes.");

  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    (*slaves_)[i].socket->Send(strlen(msg)+1, msg);
  }
}

