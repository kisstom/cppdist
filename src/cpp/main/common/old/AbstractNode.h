#ifndef ABSTRACT_NODE_H
#define ABSTRACT_NODE_H

#include <iostream>

class AbstractNode {
  public:
    virtual void Sender () = 0;
    virtual void Receiver () = 0;
};

#endif
