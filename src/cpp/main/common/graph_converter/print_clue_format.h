#include <iostream>

using namespace std;

class PrintClueFormat {
  public:
    PrintClueFormat() {
      MAX_ROW_LEN = 15000000;
    }
    void ReadAndPrint(char* input, char* output, long numnodes);
    int MAX_ROW_LEN;
};
