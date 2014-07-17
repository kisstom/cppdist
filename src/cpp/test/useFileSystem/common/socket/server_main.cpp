/*
 * test_server.cpp
 *
 *  Created on: 2014.06.17.
 *      Author: kisstom
 */

#include "../../../../main/common/components/socket/socket.h"
#include <stdio.h>
#include <sstream>
#include <string>

using std::stringstream;
using std::string;

int main(int argc, char* argv[]) {
  try {
    int master_port_;
    sscanf(argv[1], "%d", &master_port_);

    ServerSocket* master_socket = ServerSocket::Create(master_port_);
    SocketConnection *client = master_socket->Accept();

    char tmp[1024];
    stringstream ss(argv[2]);
    std::string token;
    while(std::getline(ss, token, ',')) {
      client->Send((int) token.size() + 1, token.c_str());
      client->Recv(1024, tmp);
    }

    client->Send(7, "finish");
  } catch (SocketError& e) {
    fprintf(stderr, "ERROR: %s\n", e.what());
    return 1;
  }
  return 0;
}

