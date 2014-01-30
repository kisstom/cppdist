#include <iostream>
#include <string>
#include <tr1/unordered_map>

using namespace std;
class Joiner {
  public:
    void readUrl2Topic(char*);
    void merge(char*, char*);
  private:
    tr1::unordered_map<string, string> url2topic;
};
