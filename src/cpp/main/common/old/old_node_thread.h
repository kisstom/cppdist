/*
 * old_node_thread.h
 *
 *  Created on: 2013.08.12.
 *      Author: kisstom
 */

#ifndef OLD_NODE_THREAD_H_
#define OLD_NODE_THREAD_H_

#include "../thread/thread_manager.h"
#include "AbstractNode.h"

class NodeThread: public ThreadManager{
  public:
    NodeThread(AbstractNode* node, char* msg);
  protected:
    void main();
  private:
    AbstractNode* node;
    char msg_[1024];
};


#endif /* OLD_NODE_THREAD_H_ */
