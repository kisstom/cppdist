#include "vector_node_matrix.h"
#include <limits.h>
using namespace std;


void VectorNodeMatrix::printM () {
  for (long node = 0; node < numnode_; ++node) {
    if ((*start_edge_)[node] != (*start_edge_)[node + 1]) {
      long x = (*start_edge_)[node];
      printf("%ld", (*edge_list_)[x]); 
    }

    for (long i = (*start_edge_)[node] + 1; i < (*start_edge_)[node + 1]; ++i) {
      printf(" %ld", (*edge_list_)[i]);
    }
    printf("\n");
  }
}

void VectorNodeMatrix::init_vars(long numnode, long numedge, long first_node) {
}

VectorNodeMatrix::~VectorNodeMatrix() {
  delete edge_list_;
  delete start_edge_;
}

void VectorNodeMatrix::split(char* line, vector<long>& edges) {
  edges.clear();
  stringstream ss(stringstream::in | stringstream::out);
  ss << line;
  long edge;

  while (ss.good()) {
    ss >> edge;
    edges.push_back(edge);
  }
}

void VectorNodeMatrix::init(const string& fname, FILE* logfile, long numnode, long numedge, long first_node) {
  fprintf(logfile, "Allocating memory.");
  numnode_ = numnode; //atoi(line);
  numedge_ = numedge; //atoi(line);
  first_node_ = first_node;//atoi(line);

  //edge_list_ = new vector<long>(numedge_);
  //start_edge_ = new vector<long>(numnode_ + 1);

  edge_list_->resize(numedge_);
  start_edge_->resize(numnode_ + 1);

  fprintf(logfile, "Memory allocated.");

  FILE* file = fopen(fname.c_str(), "r");
  if (file ==  NULL) printf("Error opening file.");
  fprintf(logfile, "Successfully opend file %s.", fname.c_str());

  char* line = new char[MAX_ROW_LEN];
  vector<long> edges;

  long current_num_edge = 0;
  long node_index = -1;
  long last_num_edge = 0;
  while  (!feof(file) && !ferror(file)) {
  	// After the last line adds a node that is used in iterating
    ++node_index;
    last_num_edge = current_num_edge;
    (*start_edge_)[node_index] = current_num_edge;

    if (fgets(line, MAX_ROW_LEN, file)==NULL) break;
    if (strlen(line) == 1) continue;
    line[strlen(line)-1] = '\0';
    split(line, edges);

    for (unsigned i = 0; i < edges.size(); ++i) {
      (*edge_list_)[current_num_edge++] = edges[i];
    }
    if (last_num_edge / 1000000 < current_num_edge / 1000000) {
      log_info(logfile, "%ld num of edges stored in new matrix.", current_num_edge);
    }
  }
  delete[] line;
  fclose(file);
}

void VectorNodeMatrix::WriteToBin (vector<double>* rank_vector, FILE* file) {
  for (int i = 0; i < (int)rank_vector->size(); ++i)  {
    fwrite(&(rank_vector->at(i)), sizeof(double), 1, file);
  }
}

void VectorNodeMatrix::WriteToText(vector<double>* rank_vector, FILE* file) {
  for (int i = 0; i < (int)rank_vector->size(); ++i) {
    fprintf(file, "%ld %.12f\n", first_node_ + i, rank_vector->at(i));
  }
}
