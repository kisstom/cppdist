/*
 * bitprop_master.h
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#ifndef BITPROP_MASTER_H_
#define BITPROP_MASTER_H_

#include "../algo_components/inner_master.h"

class BitpropMaster: public InnerMaster {
public:
  BitpropMaster(short);
  bool nextIter();
private:
  short neighborhoodDistance;
  short neighborhoodSize;
};


#endif /* BITPROP_MASTER_H_ */
