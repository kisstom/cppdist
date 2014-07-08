/*
 * psimrank_master.h
 *
 *  Created on: 2014.06.12.
 *      Author: kisstom
 */

#ifndef PSIMRANK_MASTER_H_
#define PSIMRANK_MASTER_H_

#include "../inner_master.h"
#include "../master.h"
#include "gmp.h"
#include "gmpxx.h"
#include "psimrank_random_generator_iface.h"

class PSimrankMaster : public InnerMaster {
public:
  PSimrankMaster();
  bool nextIter();
  void setRandomGenerator(PSimrankRandomGeneratorIFace*);
private:
  long generateRandomPrime();
  long generateRandomAdd();

  PSimrankRandomGeneratorIFace* randomGenerator_;

  int iterNum_;
  log4cpp::Category* logger_;
};


#endif /* PSIMRANK_MASTER_H_ */
