#ifndef MASTER_BASE_H_
#define MASTER_BASE_H_

#include <iostream>
#include <vector>
#include "../../../common/components/socket/socket.h"
#include "../../../common/util/log.h"
#include "../inner_master.h"
#include "../runnable.h"
//#include "slave.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"

using std::vector;

class InnerMaster;

class MasterBase : public Runnable {
  public:
    MasterBase(int, vector<Slave>*, long, bool);
    virtual ~MasterBase();
    virtual void run();
    virtual void WaitForAccepts();
    virtual void SendInfoToNodes();
    virtual bool WaitForNodes();

    virtual void MakeNodeConnections() = 0;
    virtual bool setUp();

    virtual void RunThreads();
    virtual void Final();
    virtual void InitServer();
    virtual void KillNodes();
    virtual void setInnerMaster(InnerMaster*);
    virtual void sendMessageForAllNodes(char* msg);
    virtual long getNumNodes();
    virtual void SetUpClientManager();
  protected:
    long numNodes_;
    bool isMulti;
    InnerMaster* innerMaster_;
    int master_port_;
    char master_host_[1024];
    vector<Slave>* slaves_;
    ServerSocket* master_socket_;
    log4cpp::Category* logger_;
};


#endif  // MASTER_BASE_H_
