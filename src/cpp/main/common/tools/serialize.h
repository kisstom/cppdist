#ifndef SERIALIZE_RESULT_H
#define SERIALIZE_RESULT_H

#include <iostream>
#include <stdio.h>

#include <string>
#include <vector>

using namespace std;

class SerializeResult {
  public:
    void Serialize(string fname, FILE* to);
    void SerializeAll(vector<string> names, string out_fname);
    void Deserialize(string fname, string to);
};

#endif
