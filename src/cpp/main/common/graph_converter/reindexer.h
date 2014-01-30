#include <iostream>
#include <tr1/unordered_map>

using namespace std;

class Reindexer {
  public:
    Reindexer() {
      MAX_ROW_LEN = 15000000;
    }
    void readIndices(char *indices);
    void readOldPrintNew(char* old, char* newgraph);
    long ReindexNode(string str);
  private:
    tr1::unordered_map<long, long> mapper_;
    int MAX_ROW_LEN;
};
