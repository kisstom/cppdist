#include "tournament_maker.h"

TournamentMaker::TournamentMaker (int size) {
  size_ = size;
}

vector<vector<int> >* TournamentMaker::create()
{
  vector<vector<int> >* fordulo;
  int num_round = size_ % 2 ? size_ : size_ - 1;
  fordulo = new vector<vector<int> >(num_round);
  for (int i = 0; i < num_round; ++i) {
    (*fordulo)[2*i % (num_round)].push_back(i);
    (*fordulo)[2*i % (num_round)].push_back(num_round);
    for (int j = 0; j < i; ++j) {
      (*fordulo)[(i+j)%(num_round)].push_back(j);
      (*fordulo)[(i+j)%(num_round)].push_back(i);
    }
  }
  return fordulo;
}

void TournamentMaker::MakeConnection(vector< vector<pair<int, int> > >& pairs)
{
  vector<vector<int> >* tour = create();
  pairs.resize(tour->size());
  for (unsigned int i = 0; i < tour->size(); ++i) {
    for (unsigned int j = 0; j < (*tour)[i].size()/2; ++j) {
      pairs[i].push_back(make_pair<int, int>((*tour)[i][2*j], (*tour)[i][2*j+1]));
    }
  }
  delete tour;
}

