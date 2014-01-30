#include <iostream>
#include <string>
#include <tr1/unordered_map>

using namespace std;
class HashJoiner {
  public:
    void readHash2Topic(char*);
    void merge(char*, char*);
  private:
    tr1::unordered_map<long, string> hash2topic;
};
