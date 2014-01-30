#include "host_graph_creator.h"
#include <string.h>
#include <stdio.h>
#include <sstream>


void HostGraphCreator::init(char* host_to_node, char* host_to_id,
    char* graph_f, char* out_f) {
  strcpy(host_to_node_f_, host_to_node);
  strcpy(host_to_id_f_, host_to_id);
  strcpy(graph_f_, graph_f);
  strcpy(out_f_, out_f);
}

void HostGraphCreator::run() {
  reindexHost();
  readGraph();
  printHostGraph();
}

void HostGraphCreator::reindexHost() {
  printf("Reindexing hosts...\n");
  FILE* f = fopen(host_to_node_f_, "r");
  FILE* host_id_f = fopen(host_to_id_f_, "w");

  if (NULL == f) {
    printf("ERROR opening file %s\n", host_to_node_f_);
    return;
  }

  char host[100000];
  long node_id;
  long size = 0;
  size_t success_read;
  string str_host;

  char* line = new char[100000];
  while (fgets(line, 100000, f) != NULL) {
    success_read = sscanf(line, "%ld %s\n", &node_id, host);
    if (success_read != 2) continue;

    str_host = string(host);
    if (host_to_id_.find(str_host) == host_to_id_.end()) {
      host_to_id_[str_host] = size;
      fprintf(host_id_f, "%s %ld\n", host, size);
      ++size;
    }
    node_to_host_id_[node_id] = host_to_id_[str_host];
  }

  delete[] line;
  fclose(f);
  fclose(host_id_f);
}

void HostGraphCreator::processLine(char* line, long host_id) {
  stringstream ss(stringstream::in | stringstream::out);
  ss << line;
  long edge, link;

  while (ss.good()) {
    ss >> edge;
    if (node_to_host_id_.find(edge) != node_to_host_id_.end()) {
      link = node_to_host_id_[edge];
    } else {
      continue;
    }

    host_graph[host_id].insert(link);
  }
}

void HostGraphCreator::currentDateTime() {
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  // Visit
  // http://www.cplusplus.com/reference/clibrary/ctime/strftime/
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  printf("%s ", buf);
}


void HostGraphCreator::readGraph() {
  printf("Reading graph...\n");
  char* line = new char[row_len_];
  FILE* f = fopen(graph_f_, "r");
  if (NULL == f) {
    printf("ERROR opening file %s\n", graph_f_);
    return;
  }

  long numline = 0, host_id;
  while (fgets(line, row_len_, f) != NULL) {
    if (node_to_host_id_.find(numline) == node_to_host_id_.end()) {
      printf("Host not found for %ld\n", numline);
      ++numline;
      continue;
    }

    if (line[strlen(line) - 1] == '\n') {
      line[strlen(line) - 1] = '\0';
    }

    host_id = node_to_host_id_[numline];
    if (host_graph.find(host_id) == host_graph.end()) {
      host_graph[host_id] = set<long>();
    }
      
    processLine(line, host_id);
    ++numline;
    if (numline % 1000000 == 0) {
      currentDateTime();
      printf("%ld row already processed.\n", numline);
      fflush(stdout);
    } 
  }
  delete[] line; 
  fclose(f);
}

void HostGraphCreator::printHostGraph() {
  FILE* f = fopen(out_f_, "w");
  if (NULL == f) {
    printf("ERROR opening file %s\n", out_f_);
    return;
  }

  printf("Printing host graph.\n");
  for (tr1::unordered_map<long, set<long> >::iterator it = host_graph.begin(); it !=
      host_graph.end(); ++it) {
    fprintf(f, "%ld", it->first);
    for (set<long>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {
      fprintf(f, " %ld", *jt);
    }
    fprintf(f, "\n");
  }
  fclose(f);
}
