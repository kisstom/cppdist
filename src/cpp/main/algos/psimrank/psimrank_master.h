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
  PSimrankMaster(long);
  bool nextIter();
  void addInfoForNodes(char* ss);
  void init();
private:
  void setPrime();
  //void setRandomState();
  long generateRandomCoeff();
  PSimrankRandomGeneratorIFace* randomGenerator_;

  mpz_t prime_;
  //gmp_randstate_t r_state_;
  int iterNum_;
  long seed_;
  log4cpp::Category* logger_;
};


#endif /* PSIMRANK_MASTER_H_ */
