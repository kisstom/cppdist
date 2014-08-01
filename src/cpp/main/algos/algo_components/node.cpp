/*
 * node.cpp
 *
 *  Created on: 2013.08.14.
 *      Author: kisstom
 */

#include "node.h"
#include "algo.h"
// TODO ezek mind legyenek pure virtualisak

void Node::setPartitionIndex(int partIndex) {
	partIndex_ = partIndex;
}

void Node::setSenderBuffer(SenderBuffer* senderBuffer) {
	senderBuffer_ = senderBuffer;
}

void Node::setAlgo(Algo* algo) {
	algo_ = algo;
}
