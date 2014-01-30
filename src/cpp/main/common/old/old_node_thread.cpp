/*
 * old_node_thread.cpp
 *
 *  Created on: 2013.08.12.
 *      Author: kisstom
 */

#include <cstring>
#include "./old_node_thread.h"

NodeThread::NodeThread(AbstractNode* _runnable, char* msg)
{
  node = _runnable;
  strcpy(msg_, msg);
}

void NodeThread::main()
{
  if (!strcmp(msg_, "sender"))
  {
  	node->Sender();
  }
  if (!strcmp(msg_, "receiver"))
  {
  	node->Receiver();
  }
}


