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
#include "../common/components/socket/socket.h"
#include "../common/util/log.h"
#include "inner_master.h"
#include "runnable.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"

using std::vector;

struct Slave {
  int port;
  char path[1024];
  char ip[1024];
  SocketConnection* socket;
  long minNode;
  long numNode;
};

class Master : public Runnable {
  public:
    Master(int, vector<Slave>*, char* logfile);
    ~Master();
    void run();
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
    bool setUp();
  private:
    InnerMaster* innerMaster_;
    int master_port_;
    char master_host_[1024];
    //char file_format_;
    vector<Slave>* slaves_;
    ServerSocket* master_socket_;
    char logfile_[1024];
    log4cpp::Category* logger_;
};

#endif

