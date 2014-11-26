/*
 * master.cpp
 *
 *  Created on: 2013.08.13.
 *      Author: kisstom
 */
#include <cstring>
#include <sstream>
#include "./master.h"

using std::vector;

Master::Master(int master_port, vector<Slave>* slaves, long numNodes, bool _isMulti)
{
  master_port_ = master_port;
  slaves_ = slaves;
  logger_ = &log4cpp::Category::getInstance(std::string("Master"));
  numNodes_ = numNodes;
  master_socket_ = NULL;
  innerMaster_ = NULL;
  isMulti = _isMulti;
}

Master::~Master()
{
  delete master_socket_;
}

void Master::InitServer() {
	logger_->info("Creating master server at port %d.", master_port_);
  master_socket_ = ServerSocket::Create(master_port_);
  logger_->info("Created master server at port %d.", master_port_);
}

void Master::KillNodes() {
  logger_->info("Killing nodes.");
  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    (*slaves_)[i].socket->Send(4, "die");
  }
}

void Master::SetUpClientManager() {
  sendMessageForAllNodes("HAVA NAGILA");
  WaitForNodes();
  sendMessageForAllNodes("HAVA NAGILA");
  WaitForNodes();
}

bool Master::setUp() {
	logger_->info("Setting up master.");
	try {
		InitServer();
		// Bevarjuk az osszes node kostrualodasat.
		WaitForAccepts();
		// Elkuldjuk a node-k szamat, minkey, maxkey, input data,
		// + esetleg hogy a lokalis gepen hova masolja.
		SendInfoToNodes();
		// Varunk, a masolas mellett be is olvassak a node-k az adatot.
		WaitForNodes();
		// Letrehozzuk a kapcsolatokat a node-k kozott.

		if (isMulti) {
		  MakeNodeConnections2();
		} else {
		  MakeNodeConnections();
		}

		//SetUpClientManager();
	} catch (ConnectionError& e) {
    logger_->info("Error: %s. Killing nodes.", e.what());
    KillNodes();
    return false;
  }
	logger_->info("Master setup finished.");
	return true;
}

void Master::MakeNodeConnections2() {
  sendMessageForAllNodes("HAVA NAGILA");
  WaitForNodes();
  sendMessageForAllNodes("HAVA NAGILA");
  WaitForNodes();
}

void Master::run()
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
      if (!retval) cont = retval;
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

void Master::WaitForAccepts()
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

void Master::SendInfoToNodes()
{
  // TODO ures stringre a scanf jol mukodike-e
	// sstream helyett stringet
  logger_->info("Sending info to nodes.");
  char info[1024];
  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    stringstream ss(stringstream::in | stringstream::out);

    ss << (*slaves_)[0].minNode;
    for (unsigned int j = 1; j < slaves_->size(); ++j) {
      ss << " " << (*slaves_)[j].minNode;
    }

    strcpy(info, ss.str().c_str());
    innerMaster_->addInfoForNodes(info + strlen(info));

    logger_->info("Master sending %s", info);
    (*slaves_)[i].socket->Send(strlen(info)+1, info);
  }
}

bool Master::WaitForNodes()
{
	logger_->info("Waiting for nodes.");
  char msg[1024];
  unsigned finished = 0;
  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    int size = (int) (*slaves_)[i].socket->Recv(1024, msg);
    logger_->info("Received from slave %s", msg);
    if (strcmp(msg, "empty") == 0) ++finished;
    else if (strcmp(msg, "ready")) {
      logger_->info("Error! Received from node %d msg: %s.", i, msg);
      throw NodeFailException("Node failing while master waiting for it.");
    }
  }
  logger_->info("Number of finished: %d numslaves %d", finished, (int)slaves_->size());
  return (finished != slaves_->size()) ;
}

void Master::MakeNodeConnections()
{
	logger_->info("Making connections.");
  vector<vector<int> >* tour = MakeTournament();
  for (unsigned int i = 0; i < tour->size(); ++i) {
    for (unsigned int j = 0; j < (*tour)[i].size()/2; ++j) {
      MakeConnection((*tour)[i][2*j], (*tour)[i][2*j+1]);
    }
    WaitForNodes();
    logger_->info("Connections %d done.", i);
  }
  delete tour;
}

vector<vector<int> >* Master::MakeTournament()
{
  vector<vector<int> >* fordulo;
  unsigned num_round = slaves_->size()%2 ? slaves_->size() : slaves_->size() - 1;
  fordulo = new vector<vector<int> >(num_round);
  for (unsigned int i = 0; i < num_round; ++i) {
    (*fordulo)[2*i % (num_round)].push_back(i);
    (*fordulo)[2*i % (num_round)].push_back(num_round);
    for (unsigned int j = 0; j < i; ++j) {
      (*fordulo)[(i+j)%(num_round)].push_back(j);
      (*fordulo)[(i+j)%(num_round)].push_back(i);
    }
  }
  return fordulo;
}

void Master::MakeConnection(int i, int j)
{
  if (j >= (int) slaves_->size()) {
    (*slaves_)[i].socket->Send(6, "relax");
    return;
  }
  char msg_i[1024] = "Accept";
  sprintf(msg_i + 6, " %d %s %d", j, (*slaves_)[j].ip, (*slaves_)[j].port);
  char msg_j[1024] = "Connect";
  sprintf(msg_j + 7, " %d %s %d", i, (*slaves_)[i].ip, (*slaves_)[i].port);
  // Maybe this is a string, that's why the +1
  (*slaves_)[i].socket->Send(strlen(msg_i)+1, msg_i);
  (*slaves_)[j].socket->Send(strlen(msg_j)+1, msg_j);
}

void Master::RunThreads()
{
  logger_->info("Sending to nodes start threading.");
  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    (*slaves_)[i].socket->Send(7, "thread");
  }
}

void Master::Final()
{
	logger_->info("Stop nodes.");
  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    (*slaves_)[i].socket->Send(5, "exit");
    // close socket TODO
  }
}

void Master::setInnerMaster(InnerMaster* innerMaster) {
	innerMaster_ = innerMaster;
}

void Master::sendMessageForAllNodes(char* msg) {
  logger_->info("Sending msg to nodes.");

  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    (*slaves_)[i].socket->Send(strlen(msg)+1, msg);
  }
}

long Master::getNumNodes() {
  return numNodes_;
}




