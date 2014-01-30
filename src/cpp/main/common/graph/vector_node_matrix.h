#ifndef VECTOR_NODE_MATRIX_H
#define VECTOR_NODE_MATRIX_H

#include "../util/log.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <tr1/unordered_map>

using namespace std;

class VectorNodeMatrix {
 public:
    VectorNodeMatrix() {
      MAX_ROW_LEN = 15000000;
      start_edge_ = new vector<long>;
      edge_list_ = new vector<long>;
    }
    vector<long>* GetEdgeStart() {return start_edge_;}
    vector<long>* GetEdgeList() {return edge_list_;}
    void init(const string& fname, FILE* logfile, long numnode, long numedge, long first_node);
    void printM ();
    void WriteToText(vector<double>* rvector, FILE* out);
    void WriteToBin(vector<double>* rvector, FILE* out);

    void SetStartEdges(vector<long>* edge_start) {start_edge_ = edge_start;}
    void SetEdgeList(vector<long>* edge_list) {edge_list_ = edge_list;}

    static void split(char* line, vector<long>& edges);
    ~VectorNodeMatrix();
 private:
    void init_vars(long numnode, long numedge, long first_node);
    vector<long>* start_edge_;
    vector<long>* edge_list_;
    long numnode_;
    long numedge_;
    long first_node_;
    int MAX_ROW_LEN;
};

#endif
