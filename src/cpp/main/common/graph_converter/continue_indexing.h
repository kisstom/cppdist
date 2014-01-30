#include <iostream>
#include <tr1/unordered_map>

using namespace std;

class Indexer {
  public:
    Indexer() {MAX_ROW_LEN = 15000000;}
    void run();
    void reindex(string str);
    void flush(char* output);
    void init(char* input);
  private:
    FILE *input_;
    long index_;
    tr1::unordered_map<long, long> mapper_;
    int MAX_ROW_LEN;
};
