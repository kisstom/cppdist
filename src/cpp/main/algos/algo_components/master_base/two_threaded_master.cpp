#include "two_threaded_master.h"

TwoThreadedMaster::TwoThreadedMaster(int master_port, vector<Slave>* slaves, long numNodes):
MasterBase(master_port, slaves, numNodes) {}

void TwoThreadedMaster::MakeNodeConnections()
{
  logger_->info("Making connections.");
  vector<vector<int> >* tour = MakeTournament();
  for (unsigned int i = 0; i < tour->size(); ++i) {
    for (unsigned int j = 0; j < (*tour)[i].size()/2; ++j) {
      MakeConnection((*tour)[i][2*j], (*tour)[i][2*j+1]);
    }
    WaitForNodes();
    logger_->info("Connections %d done.", i);
  }
  delete tour;
}

vector<vector<int> >* TwoThreadedMaster::MakeTournament()
{
  vector<vector<int> >* fordulo;
  unsigned num_round = slaves_->size()%2 ? slaves_->size() : slaves_->size() - 1;
  fordulo = new vector<vector<int> >(num_round);
  for (unsigned int i = 0; i < num_round; ++i) {
    (*fordulo)[2*i % (num_round)].push_back(i);
    (*fordulo)[2*i % (num_round)].push_back(num_round);
    for (unsigned int j = 0; j < i; ++j) {
      (*fordulo)[(i+j)%(num_round)].push_back(j);
      (*fordulo)[(i+j)%(num_round)].push_back(i);
    }
  }
  return fordulo;
}

void TwoThreadedMaster::MakeConnection(int i, int j)
{
  if (j >= (int) slaves_->size()) {
    (*slaves_)[i].socket->Send(6, "relax");
    return;
  }
  char msg_i[1024] = "Accept";
  sprintf(msg_i + 6, " %d %s %d", j, (*slaves_)[j].ip, (*slaves_)[j].port);
  char msg_j[1024] = "Connect";
  sprintf(msg_j + 7, " %d %s %d", i, (*slaves_)[i].ip, (*slaves_)[i].port);
  // Maybe this is a string, that's why the +1
  (*slaves_)[i].socket->Send(strlen(msg_i)+1, msg_i);
  (*slaves_)[j].socket->Send(strlen(msg_j)+1, msg_j);
}

