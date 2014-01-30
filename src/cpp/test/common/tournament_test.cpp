#include <gtest/gtest.h>
#include "../../main/common/util/tournament_maker.h"

TEST(MakeTournament, 31) {
  TournamentMaker tc(31);
  vector<vector<int> >* tour = tc.create();
  for (unsigned i = 0; i < tour->size(); ++i) {
    for (unsigned j = 0; j < (*tour)[i].size(); ++j) {
      //printf("%d %d\n", i, (*tour)[i][j]);
    }
  }
}

TEST(MakeConnection, 31) {
  TournamentMaker tc(31);
  vector<vector<pair<int, int> > > pairs;
  tc.MakeConnection(pairs);
  /*for (unsigned i = 0; i < pairs.size(); ++i) {
    printf("\n");
    for (unsigned j = 0; j < pairs[i].size(); ++j) {
      printf("%d %d\n", pairs[i][j].first, pairs[i][j].second);
    }
  }*/
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
