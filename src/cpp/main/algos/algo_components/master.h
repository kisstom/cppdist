/*
 * master.h
 *
 *  Created on: 2013.08.13.
 *      Author: kisstom
 */

#ifndef MASTER_H
#define MASTER_H

#include <iostream>
#include <vector>
#include "../../common/components/socket/socket.h"
#include "../../common/util/log.h"
#include "inner_master.h"
#include "runnable.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"

using std::vector;

class InnerMaster;

struct Slave {
  int port;
  //char path[1024];
  char ip[1024];
  SocketConnection* socket;
  long minNode;
  long numNode;
};

class Master : public Runnable {
  public:
    Master(int, vector<Slave>*, long);
    ~Master();
    void run();
    bool setUp();
    void WaitForAccepts();
    void SendInfoToNodes();
    bool WaitForNodes();
    void MakeNodeConnections();
    vector<vector<int> >* MakeTournament();
    void MakeConnection(int i, int j);
    void RunThreads();
    void Final();
    void InitServer();
    void KillNodes();
    void setInnerMaster(InnerMaster*);
    void sendMessageForAllNodes(char* msg);
    long getNumNodes();
    void SetUpClientManager();
  private:
    long numNodes_;
    InnerMaster* innerMaster_;
    int master_port_;
    char master_host_[1024];
    vector<Slave>* slaves_;
    ServerSocket* master_socket_;
    log4cpp::Category* logger_;
};

#endif

