/*
 * IMasterBuilder.h
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#ifndef IMASTERBUILDER_H_
#define IMASTERBUILDER_H_

#include "master.h"
#include "inner_master_factory.h"

#include <tr1/unordered_map>

using std::tr1::unordered_map;
using std::string;

class IMasterBuilder {
public:
	Master* buildFromConfig(unordered_map<string, string>* params);
	virtual ~IMasterBuilder() {}
protected:
  virtual Master* createMaster(unordered_map<string, string>*) = 0;

};

#endif /* IMASTERBUILDER_H_ */
