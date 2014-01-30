#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <set>
#include <tr1/unordered_map>
#include "dmoz_reader.h"


using namespace std;

int main (int argc, char* argv[]) {
  FILE* needed = fopen(argv[1], "r");
  set<pair<long, long> > needed_pairs;
  tr1::unordered_map<long, string> id2topic;
  long id1, id2;
  while (fscanf(needed, "%ld %ld", &id1, &id2) != EOF) {
    needed_pairs.insert(make_pair<long, long>(id1, id2));
  }
  fclose(needed);
  FILE* ids_with_topic_file = fopen(argv[2], "r");

  char topic[102400];
  long id;

  int n = 0;
  while (fscanf(ids_with_topic_file, "%ld %s", &id, topic) != EOF) {
    id2topic[id] = string(topic);
    ++n;
  }
  printf("Number of topics read: %d\n", n);
  // Do the distance calculation.
  double distance;
  FILE * outfile = fopen(argv[3], "w");
  printf("Needed pairs size: %d\n", (int)needed_pairs.size());
  int count = 0;
  for (set<pair<long, long> >::iterator it = needed_pairs.begin();
      it != needed_pairs.end(); ++it) {
      ++count;
      if (count % 100000 == 0) printf("%d pair computed.\n", count);
      if (id2topic.find(it->first) == id2topic.end() || 
              id2topic.find(it->second) == id2topic.end()) continue;
      distance = DmozReader::ComputeDistanceFromRoot(id2topic[it->first], id2topic[it->second]);
      fprintf(outfile, "%ld %ld %lf\n", it->first, it->second, distance);
  }
  fclose(outfile);
  fclose(ids_with_topic_file);
}
