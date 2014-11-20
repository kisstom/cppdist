#include "master_socket_manager.h"

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

MasterSocketManager::MasterSocketManager() {
  self_socket_ = NULL;
  master_socket_ = NULL;
  logger_ = &log4cpp::Category::getInstance(std::string("MasterSocketManager"));
}

void MasterSocketManager::initClient(int slave_port) {
  logger_->info("Creating client at port %d", slave_port);
  self_socket_ = ServerSocket::Create(slave_port);
  slave_port_ = slave_port;
  logger_->info("Created client at port %d.", slave_port_);
}

void MasterSocketManager::connectToMaster(char* master_host, int master_port) {
  logger_->info("Connecting to master on host %s and port %d", master_host, master_port);
  setIp();
  master_socket_ = SocketConnection::Connect(string(master_host), master_port);
  char init_instr[1024];
  sprintf(init_instr, "%d,%s", slave_port_, ip_);
  master_socket_->Send(strlen(init_instr)+1, init_instr);
}

void MasterSocketManager::setIp() {
    static struct ifreq ifreqs[32];
    struct ifconf ifconf;
    memset(&ifconf, 0, sizeof(ifconf));
    ifconf.ifc_req = ifreqs;
    ifconf.ifc_len = sizeof(ifreqs);

    int sd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sd >= 0);

    int r = ioctl(sd, SIOCGIFCONF, (char *)&ifconf);
    assert(r == 0);

    //bool found = false;
    for(uint i = 0; i < ifconf.ifc_len/sizeof(struct ifreq); ++i)
    {
        if (strstr(ifreqs[i].ifr_name, "eth") != NULL) {
          strcpy(ip_, inet_ntoa(((struct sockaddr_in *)&ifreqs[i].ifr_addr)->sin_addr));
          //found = true;
          break;
        }
    }

    close(sd);
}

void MasterSocketManager::sendReadyToMaster(){
  //logger_->info("Sending ready to master.");
  master_socket_->Send(6, "ready");
}

void MasterSocketManager::sendEmptyToMaster(){
  logger_->info("Sending empty to master.");
  master_socket_->Send(6, "empty");
}

void MasterSocketManager::sendFailToMaster(){
  logger_->info("Sending fail to master.");
  master_socket_->Send(5, "fail");
}
