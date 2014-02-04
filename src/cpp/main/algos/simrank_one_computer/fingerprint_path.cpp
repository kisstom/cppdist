/*
 * fingerprint_path.cpp
 *
 *  Created on: 2014.02.03.
 *      Author: mrita
 */

# include "fingerprint_path.h"

#include <iostream>

using namespace std;

long FingerprintPath::getRandomNeighbor(long node, short i, short j){
  cout<<" getRandomNeighbor:  "<<matrix<< endl;
  long numneighbor = matrix->neighborhoodSize(node);
 cout<<" neighbor size ok"<< endl;
  if (numneighbor == 0) return -1;
  int hashValue = random->get(i, j, node);
cout<<" random get ok"<< endl;
  int index = hashValue % numneighbor;  
  if (index < 0) index+=numneighbor;
  return matrix->getEdgeAtPos(node, index);
}
