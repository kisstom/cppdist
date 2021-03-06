/*
 * mock_node.h
 *
 *  Created on: 2013.08.14.
 *      Author: kisstom
 */

#ifndef MOCK_NODE_H_
#define MOCK_NODE_H_


#include "gmock/gmock.h"
#include "../../main/algos/algo_components/node.h"


// generated by:
// . ../../path_config.sh; "$GMOCK"/scripts/generator/gmock_gen.py test/gmock_example_maxfreq.h FrequencyCounter
//

class MockNode : public Node {
 public:
  MOCK_METHOD1(beforeIteration, void(string));
  MOCK_METHOD0(afterIteration, bool());
  MOCK_METHOD0(sender, void());
  MOCK_METHOD1(initFromMaster, void(string));
  MOCK_METHOD3(initData, void(string, long, long));
  MOCK_METHOD1(setPartitionIndex, void(int));
  MOCK_METHOD0(final, void());
};


#endif /* MOCK_NODE_H_ */
