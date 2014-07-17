/*
 * test_client.cpp
 *
 *  Created on: 2014.06.17.
 *      Author: kisstom
 */
#include <iostream>
#include <stdio.h>
#include "../../../../main/common/components/socket/socket.h"

int main(int argc, char* argv[]) {
  int master_port_;
  sscanf(argv[1], "%d", &master_port_);

  FILE* outputFile = NULL;
  int retval = 0;
  try {
    SocketConnection* serverConn = SocketConnection::Connect("localhost", master_port_);
    outputFile = fopen(argv[2], "w");

    char msg[1024];
    char finish[1024] = "finish";
    int count = 0;
    while (true) {
      serverConn->Recv(1024, msg);
      serverConn->Send(3, "ok");
      if (strcmp(finish, msg) == 0) {
        break;
      }
      fprintf(outputFile, "%s\n", msg);
      ++count;

      if (count > 10) {
        return 1;
      }
    }

  } catch (SocketError& e) {
    retval = 1;
  }

  if (NULL != outputFile) {
    fclose(outputFile);
  }

  return retval;
}


