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
  PSimrankMaster();
  bool nextIter();
  void addInfoForNodes(char* ss);
  void setMaster(Master*);
private:
  void setPrime();
  long generateRandomCoeff();
  Master* master_;
  mpz_t prime_;
  int iterNum_;
  log4cpp::Category* logger_;
};


#endif /* PSIMRANK_MASTER_H_ */
