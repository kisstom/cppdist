/*
 * inode_factory.h
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#ifndef INODE_FACTORY_H_
#define INODE_FACTORY_H_

#include <tr1/unordered_map>

using std::tr1::unordered_map;
using std::string;

#include "node.h"

class INodeFactory {
public:
	virtual Node* createNodeFromConfig(unordered_map<string, string>* params) = 0;
	virtual ~INodeFactory() {}
};


#endif /* INODE_FACTORY_H_ */
