#include "../io/sparse_rating_splitter.h"

int main(int agc, char* argv[]) {
  Util util;

  SparseRatingSplitter splitter(string(argv[1]), util.stringToLong(argv[2]));
  FILE* inputF;
  FILE* configF;

  inputF = fopen(argv[3], "r");
  configF = fopen(argv[4], "w");

  if (NULL == inputF || NULL == configF) {
    fprintf(stderr, "Error opening files: %s %s\n", argv[3], argv[4]);
    return 1;
  }

  splitter.setPartiConfig(configF);
  splitter.process(inputF);

  fclose(inputF);
  fclose(configF);
}
