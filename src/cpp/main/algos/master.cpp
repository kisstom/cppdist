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

Master::Master(int master_port, vector<Slave>* slaves)
{
  master_port_ = master_port;
  slaves_ = slaves;
  logger_ = &log4cpp::Category::getInstance(std::string("Master"));
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
  //log_info(logfile_, "Sending nodes die.\n");
  for (unsigned int i = 0; i < slaves_->size(); ++i) {
    (*slaves_)[i].socket->Send(4, "die");
  }
  //log_info(logfile_, "Exiting.");
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
		MakeNodeConnections();
	} catch (ConnectionError& e) {
    logger_->info("Error: %s. Killing nodes.", e.what());
    KillNodes();
    return false;
  }
	logger_->info("Master setup finished.");
	return true;
}

void Master::run()
{
  bool cont = true;
  try {
    // Varunk.
    // WaitForNodes();
    while (cont) {
      // Megmondjuk a node-oknak hogy futtassak
      // sender, receiver fuggvenyeiket 2 kulon szalban.
      RunThreads();
      // Bevarjuk oket.
      cont = WaitForNodes();
      if (!cont) break;

      cont = innerMaster_->nextIter();
      if (!cont) break;
    }
  } catch (ConnectionError& e) {
    //log_err(logfile_, "Error: %s.\n", e.what());
    KillNodes();
    return;
  }
  // Stopping nodes.
  Final();
  // TODO waiting for full destroy of nodes
  //WaitForNodes();
}

void Master::WaitForAccepts()
{
	logger_->info("Waiting for %d accepts.", (int)slaves_->size());
  char instr[1024], ip[1024];
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
    if (!success) throw NodeFailException("Not valid port received from node.");
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
    //innerMaster_->addInfoForNodes(&ss);
    strcpy(info, ss.str().c_str());
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
    //log_dbg(logfile_, "Sending to %d relax.", i);
    return;
  }
  char msg_i[1024] = "Accept";
  sprintf(msg_i + 6, " %d %s %d", j, (*slaves_)[j].ip, (*slaves_)[j].port);
  char msg_j[1024] = "Connect";
  sprintf(msg_j + 7, " %d %s %d", i, (*slaves_)[i].ip, (*slaves_)[i].port);
  (*slaves_)[i].socket->Send(strlen(msg_i)+1, msg_i);
  (*slaves_)[j].socket->Send(strlen(msg_j)+1, msg_j);
  //log_dbg(logfile_, "Sending to %d %s.", i, msg_i);
  //log_dbg(logfile_, "Sending to %d %s.", j, msg_j);
}

void Master::RunThreads()
{
  //log_info(logfile_, "Sending to nodes start threading.");
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
  //log_info(logfile_, "Master finished.");
}

void Master::setInnerMaster(InnerMaster* innerMaster) {
	innerMaster_ = innerMaster;
}




