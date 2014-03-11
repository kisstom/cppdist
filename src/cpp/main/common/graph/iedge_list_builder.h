/*
 * IEdgeListBuilder.h
 *
 *  Created on: 2014.03.07.
 *      Author: kisstom
 */

#ifndef IEDGELISTBUILDER_H_
#define IEDGELISTBUILDER_H_

#include <string>
#include <stdlib.h>
#include <vector>
#include "edgelist_container.h"

using std::string;
using std::vector;

class IEdgeListBuilder {
public:
  virtual void buildFromFile(string fname) = 0;
  void setContainer(EdgelistContainer* container) {
    container_ = container;
  }

  EdgelistContainer* getContainer() {
    return container_;
  }

  virtual ~IEdgeListBuilder() {}
protected:
  EdgelistContainer* container_;
};


#endif /* IEDGELISTBUILDER_H_ */
