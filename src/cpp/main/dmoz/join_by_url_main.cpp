#include <iostream>
#include "join_by_url.h"

int main(int argc, char* argv[]) {
  Joiner joiner;
  joiner.readUrl2Topic(argv[1]);
  joiner.merge(argv[2], argv[3]);
}
