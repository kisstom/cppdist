#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <tr1/unordered_map>
#include "dmoz_reader.h"

using namespace std;

int main (int argc, char* argv[]) {
  DmozReader dreader;

  char* file = argv[1];
  FILE* input = fopen(file, "r");
  dreader.Init(input);

  dreader.Read();

  dreader.Flush(stdout);
}
