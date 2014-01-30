#include <iostream>
#include <tr1/unordered_map>
#include <set>

using namespace std;

class HostGraphCreator {
  public:
    HostGraphCreator() {row_len_ = 15000000;}
    void run();
    void init(char*, char*, char*, char*);
  private:
    void reindexHost();
    void readGraph();
    void printHostGraph();
    void processLine(char*line, long host_id);
    void currentDateTime();
    int row_len_;
    char host_to_node_f_[1024];
    char host_to_id_f_[1024];
    char graph_f_[1024];
    char out_f_[1024];

    tr1::unordered_map<long, long> node_to_host_id_;
    tr1::unordered_map<string, long> host_to_id_;
    tr1::unordered_map<long, set<long> > host_graph;

};



