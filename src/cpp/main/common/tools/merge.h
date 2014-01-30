#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Merge {
  public:
    Merge(vector<string> to_merge, string output);
    void openFiles();
    void printMinIndex();
    int getIndex();
    void init();
    int min();
    void merge();
    bool hasNext();
    void readRowLastPrinted();
    void close();
  private:
    vector<long> minIndex_;
    vector<double> value_;
    vector<FILE*> mergeFiles_;
    int lastPrinted_;
    vector<bool> isClosed_;
    FILE* outfile_;
    long numRowPrinted_;
};

