#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/time.h>
#include <cmath>
#include "../../../../main/common/components/socket/socket.h"
#include "../../../../main/common/components/socket/zmq_sockets/zmq.hpp"


int digi(int index) {
  int digits = 0;
  if (index == 0) {
    digits = 1;
  } else {
    digits = (int) log10(index) + 1;
  }
  return digits;
}

int main(int argc, char* argv[]) {

  int master_port_ = atoi(argv[1]);
  int expectedMsgs = atoi(argv[2]);
  int msgSize = atoi(argv[3]);
  int triggerPort = atoi(argv[4]);

  int retval = 0;
  char* msg = new char[msgSize + 1];
  char finish[1024];
  sprintf(finish, "%d", expectedMsgs - 1);
  int finishSize = digi(expectedMsgs - 1);

  try {
    SocketConnection* serverConn = SocketConnection::Connect("10.1.1.107", master_port_);

    char msg[1024];
    int recvMsgSize = 0;

    fprintf(stderr, "Starting loop.\n");
    printf("FINISH: %s %d\n", finish, finishSize);
    fflush(stdout);

    char ip[1024];
    zmq::context_t context (1);
    zmq::socket_t instrSocket(context, ZMQ_SUB);
    instrSocket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    fprintf(stderr, "Waiting for trigger on port %d.\n", triggerPort);
    sprintf(ip, "tcp://10.1.1.107:%d", triggerPort);
    instrSocket.connect(ip);
    zmq::message_t m(20);
    instrSocket.recv(&m);

    int start, end;
    struct timeval tp;
    gettimeofday(&tp, NULL);
    start = tp.tv_usec + tp.tv_sec * 1000000;

    while (true) {
      recvMsgSize = serverConn->Recv(msgSize, msg);
      msg[recvMsgSize] = '\0';
      printf("%s\n", msg);
      fflush(stdout);

      if (strcmp(msg + (recvMsgSize - finishSize), finish) == 0) {
        printf("Break at msg %s\n", msg);
        break;
      }
    }

    gettimeofday(&tp, NULL);
    end = tp.tv_usec + tp.tv_sec * 1000000;
    printf("Times passed ... %f \n", (end - start) / 1000000.0);
    fflush(stdout);

  } catch (SocketError& e) {
   return 1;
  }
}
