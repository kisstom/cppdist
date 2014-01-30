#include <iostream>
#include <vector>

using namespace std;

class TournamentMaker {
  public:
    TournamentMaker(int s);
    vector<vector<int> >* create();
    void MakeConnection(vector<vector<pair<int, int> > >& pairs);
  private:
    int size_;
};
