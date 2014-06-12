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

class PSimrankMaster : public InnerMaster {
public:
  PSimrankMaster(Master*);
  bool nextIter();
  void addInfoForNodes(string* ss);

private:
  long generateRandomCoeff();
  Master* master;
  mpz_t prime_;
};


#endif /* PSIMRANK_MASTER_H_ */
