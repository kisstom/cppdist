#include "../../../../main/common/components/socket/socket.h"
#include "../../../../main/common/components/socket/zmq_sockets/zmq.hpp"
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <cmath>

using std::stringstream;
using std::string;


int digi(int index) {
  int digits = 0;
  if (index == 0) {
    digits = 1;
  } else {
    digits = (int) log10(index) + 1;
  }
  return digits;
}


char* createMsgAtSize(int size, int index) {

  char* message = new char[size + 1];
  int digits = digi(index);

  for (int i = 0; i < size - digits; ++i) {
    message[i] = 'V';
  }
  sprintf(message + size - digits, "%d", index);
  message[size] = '\0';
  return message;
}


int main(int argc, char* argv[]) {
  try {

    int master_port_ = atoi(argv[1]);
    int numMessages = atoi(argv[2]);
    int msgSize = atoi(argv[3]);
    int numReceivers = atoi(argv[4]);
    int triggerPort = atoi(argv[5]);

    vector<char*> msgs(numMessages, NULL);
    for (int i = 0; i < numMessages; ++i) {
      //msgs[i] = new char[msgSize];
      msgs[i] = createMsgAtSize(msgSize, i);
      //sprintf(msgs[i], "Message_%d", i);
    }

    ServerSocket* master_socket = ServerSocket::Create(master_port_);
    vector<SocketConnection*> sockets(numReceivers, NULL);
    for (int i = 0; i < numReceivers; ++i) {
      sockets[i] = master_socket->Accept();
    }

    char ip[1024];
    zmq::context_t context (1);
    zmq::socket_t instrSocket(context, ZMQ_SUB);
    instrSocket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    fprintf(stderr, "Waiting for trigger on port %d.\n", triggerPort);
    sprintf(ip, "tcp://localhost:%d", triggerPort);
    instrSocket.connect(ip);
    zmq::message_t m(20);
    instrSocket.recv(&m);

    int start, end;
    struct timeval tp;

    gettimeofday(&tp, NULL);
    start = tp.tv_usec + tp.tv_sec * 1000000;

    fprintf(stderr, "Startint msg sending");
    for (int i = 0; i < numMessages; ++i) {
      for (int j = 0; j < numReceivers; ++j) {
        sockets[j]->Send(strlen(msgs[i]), msgs[i]);
      }
    }

    gettimeofday(&tp, NULL);
    end = tp.tv_usec + tp.tv_sec * 1000000;
    std::cout << "Times passed ... " << (end - start) / 1000000.0 << "\n";


  } catch (SocketError& e) {
    fprintf(stderr, "ERROR: %s\n", e.what());
    return 2;
  }
  return 0;
}
