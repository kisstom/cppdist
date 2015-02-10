
#include <iostream>
#include <cstdio>
#include "../io/outpartition_hash_computer.h"
#include "../util/logger_factory.h"
#include <cstdlib>

int main (int argc, char* argv[]) {
  LoggerFactory::initLogger("INFO", "CONSOLE", "");
  log4cpp::Category* logger = &log4cpp::Category::getInstance(std::string("OutpartitionHashComputerJob"));

  string inputFile(argv[1]);
  string cfg(argv[2]);
  int numslaves, rowlen, partIndex;

  sscanf(argv[3], "%d", &numslaves);
  sscanf(argv[4], "%d", &rowlen);
  sscanf(argv[5], "%d", &partIndex);

  OutpartitionHashComputer computer(inputFile, cfg, numslaves, rowlen, partIndex);
  computer.run();


  FILE* hashFile = fopen(argv[6], "w");
  if (hashFile == NULL) {
    logger->error("Error opening file %s", argv[6]);
    exit(1);
  }

  vector<short>* hashes = computer.getOutPartitionHashes();
  for (int i = 0; i < (int) hashes->size(); ++i) {
    fprintf(hashFile, "%hd\n", (*hashes)[i]);
  }

  fclose(hashFile);

  FILE* flagFile = fopen(argv[7], "w");
  if (flagFile == NULL) {
    logger->error("Error opening file %s", argv[7]);
    exit(1);
  }

  vector<bool>* flags = computer.getUpdateFlags();
  bool b;
  for (int i = 0; i < (int) flags->size(); ++i) {
    b = (*flags)[i];
    fprintf(flagFile, "%d\n", b);
  }

  fclose(flagFile);
}
