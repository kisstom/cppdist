#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>

using namespace std;
class Inverse {
  public:
    Inverse(char * input, int nnode);
    void writeToFile(char *output);
    void run();
  private:
    void mytime();
    int nnode_;
    FILE* input_;
    FILE* output_;
    vector<vector<long> > inverse_;
    int MAX_ROW_LEN;
};
