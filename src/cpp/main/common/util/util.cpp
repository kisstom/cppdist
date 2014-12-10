/*
 * util.cpp

 *
 *  Created on: 2013.07.29.
 *      Author: kisstom
 */

#include "util.h"
#include <string>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include "../components/socket/zmq_sockets/zmq.hpp"

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

size_t Util::nextLong(char* line, size_t from, long& element) {
	sscanf(line + from, "%ld", &element);
	for (unsigned i = from; i < strlen(line); ++i) {
		if (line[i] == ' ') {
			return i;
		}
	}
	return string::npos;
}

size_t Util::nextShort(char* line, size_t from, short& element) {
	sscanf(line + from, "%hd", &element);

	for (unsigned i = from; i < strlen(line); ++i) {
		if (line[i] == ' ') {
			return i;
		}
	}
	return string::npos;
}

void Util::readEdges(char* line, vector<long>* edges) {
  size_t from = 0;
  long node;

  if (strlen(line) == 0 || line[0] == '\n') return ;
	from = nextLong(line, from, node);
	edges->push_back(node);

  while (hasNext(line, from)) {
  	from += 1;
  	from = nextLong(line, from, node);
  	edges->push_back(node);
  }
}

bool Util::hasNext(char* line, size_t from) {
	if (from == string::npos) return false;
	for (unsigned i = 0; i < strlen(line + from); ++i) {
		if (line[i + from] == ' ') {
			return true;
		}
	}
	return false;
}

bool Util::search(long elem, vector<long>::iterator it, int size) {
	// TODO: need to be changed to binary search
	vector<long>::iterator res = std::search_n(it, it + size, 1, elem);
	return res != (it + size);
}

void Util::split(char* line, vector<long>& edges) {
  edges.clear();
  stringstream ss(stringstream::in | stringstream::out);
  ss << line;
  long edge;

  while (ss.good()) {
    ss >> edge;
    edges.push_back(edge);
  }
}

vector<string> Util::split(string str, char sep) {
  stringstream ss(str);
  std::string token;
  vector<string> retval;
  while(std::getline(ss, token, sep)) {
    retval.push_back(token);
  }

  return retval;
}

vector<long> Util::convertToLong(vector<string> strs) {
  vector<long> retval;
  long tmp;

  for (int i = 0; i < strs.size(); ++i) {
    sscanf(strs[i].c_str(), "%ld", &tmp);
    retval.push_back(tmp);
  }

  return retval;
}

vector<short> Util::convertToShort(vector<string> strs) {
  vector<short> retval;
  short tmp;

  for (int i = 0; i < strs.size(); ++i) {
    sscanf(strs[i].c_str(), "%hd", &tmp);
    retval.push_back(tmp);
  }

  return retval;
}

void Util::checkParam(vector<string> shouldContain, unordered_map<string, string>* params) {
  for (int i = 0; i < shouldContain.size(); ++i) {
    if (params->find(shouldContain[i]) == params->end()) {
      throw ParamMissException(shouldContain[i] + " is missing.");
    }
  }
}

void Util::checkParam(unordered_map<string, string>* params, int argc, ...) {
  va_list ap;
  va_start(ap, argc);

  for(int i = 0; i < argc; i++) {
    char* a = va_arg(ap, char*);
    string str_par(a);

    if (params->find(str_par) == params->end()) {
      throw ParamMissException(str_par + " is missing.");
    }
  }
  va_end(ap);
}

void Util::printHashSet(unordered_map<long, std::set<long> > hashSet) {
  for (unordered_map<long, std::set<long> >::iterator it = hashSet.begin(); it != hashSet.end(); ++it) {
    printf("%ld ", it->first);
    for (std::set<long>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {
      printf (" %ld", *jt);
    }
    printf("\n");
  }
}

void Util::printHashSet(unordered_map<int, std::set<long> > hashSet) {
  for (unordered_map<int, std::set<long> >::iterator it = hashSet.begin(); it != hashSet.end(); ++it) {
    printf("%d ", it->first);
    for (std::set<long>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {
      printf (" %ld", *jt);
    }
    printf("\n");
  }
}

// Not thread safe
void Util::splitByToken(char* str, vector<long>& edges) {
  char * pch;
  long num = 0;

  pch = strtok (str, " ");
  while (pch != NULL) {
    sscanf(pch, "%ld", &num);
    edges.push_back(num);
    pch = strtok (NULL, " ");
  }
}

int Util::stringToInt(const string& str) {
  stringstream ss(stringstream::in | stringstream::out);
  ss << str;
  int retval;
  ss >> retval;
  return retval;
}

string Util::intToString(const int val) {
  stringstream ss(stringstream::in | stringstream::out);
  ss << val;
  return ss.str();
}

int Util::digi(int index) {
  int digits = 0;
  if (index == 0) {
    digits = 1;
  } else {
    digits = (int) log10(index) + 1;
  }
  return digits;
}

char* Util::createBufferAtSize(int size, int index) {
  char* message = new char[size + 1];
  int digits = digi(index);

  int i;
  for (i = 0; i < size - digits; ++i) {
    message[i] = 'V';
  }

  sprintf(message + size - digits, "%d", index);
  return message;
}

void Util::zmqSocketBlock(int triggerPort) {
  char ip[1024];
  zmq::context_t context (1);
  zmq::socket_t instrSocket(context, ZMQ_SUB);
  instrSocket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  fprintf(stderr, "Waiting for trigger on port %d.\n", triggerPort);
  sprintf(ip, "tcp://localhost:%d", triggerPort);
  instrSocket.connect(ip);
  zmq::message_t m(20);
  instrSocket.recv(&m);
}

void Util::setIpByHost(const char* host, char* ip) {
  struct addrinfo hints, *res;
  int errcode;
  char addrstr[100];
  void *ptr;

  memset (&hints, 0, sizeof (hints));
  hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags |= AI_CANONNAME;

  errcode = getaddrinfo (host, NULL, &hints, &res);
  if (errcode != 0) {
    //perror ("getaddrinfo");
    return;
  }

  //printf ("Host: %s\n", host);
  while (res) {
    inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);

    switch (res->ai_family)
    {
    case AF_INET:
      ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
      break;
    case AF_INET6:
      ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
      break;
    }
    inet_ntop (res->ai_family, ptr, addrstr, 100);
    //printf ("IPv%d address: %s (%s)\n", res->ai_family == PF_INET6 ? 6 : 4,
    //    addrstr, res->ai_canonname);

    sprintf(ip, "%s", addrstr);
    res = res->ai_next;
  }
}

