#ifndef MYCOMPARE_H_
#define MYCOMPARE_H_

#include<iostream>

using std::pair;

class MyCompare
{

public:

  bool operator() (const pair<long, double>& lhs, const pair<long, double>&rhs) const
  {
  	// temporal change
    return (lhs.second < rhs.second);
  }
};

#endif
