#include "../../../main/common/random/hash_pseudo_random.h"
#include "../../../main/common/util/util.h"

int main(int argc, char* argv[]) {
  Util util;

  int seed = util.stringToInt(string(argv[1]));
  HashPseudoRandom random(seed);
  printf("%d\n", random.get(0, 0, 0) % 3);
  printf("%d\n", random.get(0, 0, 6) % 2);
  printf("%d\n", random.get(0, 0, 7) % 2);
}
