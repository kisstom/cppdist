/*
 * node.h
 *
 *  Created on: 2013.08.12.
 *      Author: kisstom
 */

#ifndef NODE_H_
#define NODE_H_

#include <string>
#include "runnable.h"
#include "../../common/components/sender_buffer.h"
#include "algo_base/algo_base.h"

class AlgoBase;

using std::string;


class Node {
public:
	virtual void beforeIteration(string msg) = 0;
	virtual bool afterIteration() = 0;
	virtual void sender() = 0;

	virtual void initFromMaster(string) = 0;
	virtual void final() = 0;
	virtual void setPartitionIndex(int);
	virtual void setSenderBuffer(SenderBuffer*);
	virtual void setAlgo(AlgoBase*);
	virtual ~Node() {}
protected:
  int partIndex_;
  SenderBuffer* senderBuffer_;
  AlgoBase* algo_;
};

#endif /* NODE_H_ */
