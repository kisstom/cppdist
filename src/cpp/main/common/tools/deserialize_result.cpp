#include <iostream>
#include "../util/ConfigParser.h"
#include "serialize.h"

int main (int argc, char* argv[]) {
  SerializeResult sr;
  sr.Deserialize(string(argv[1]), string(argv[2]));
}
