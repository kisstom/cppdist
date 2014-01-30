#include "serialize.h"

void SerializeResult::SerializeAll(vector<string> names, string out_fname) {
  FILE* outfile = fopen(out_fname.c_str(), "wb");
  for (unsigned i = 0; i < names.size(); ++i) {
    printf("%s\n", names[i].c_str());
    Serialize(names[i], outfile);
  }
  fclose(outfile);
}

void SerializeResult::Serialize (string fname, FILE* to) {
  FILE* infile = fopen(fname.c_str(), "r");
  long node;
  double feat_value;
  while (fscanf(infile, "%ld %lf\n", &node, &feat_value) != EOF) {
    fwrite(&feat_value, sizeof(double), 1, to);
  }
  fclose(infile);
}

void SerializeResult::Deserialize(string fname, string to)  {
   FILE* infile = fopen(fname.c_str(), "rb");
   FILE* outfile = fopen(to.c_str(), "w");
   double feat_value = 0.0;
   long node = 0;
   while (1 == 1) {
     fread(&feat_value, sizeof(double), 1, infile);
     if (feof(infile)) break;
     fprintf(outfile, "%ld %.20lf\n", node, feat_value);
     ++node;
   }
   fclose(outfile);
   fclose(infile);
   printf("DESERIALIZE %ld successfully read\n", node);
}
