#ifndef PARTITION_CFG_HANDLER_H_
#define PARTITION_CFG_HANDLER_H_

class PartitionCFGHandler {
public:
  virtual long getMinNode(int);
  virtual long getNextMinNode(int);
  virtual long getNumNode(int);

  virtual long getMinUserNode(int);
  virtual long getNumUserNode(int);

  virtual long getMinItemNode(int);
  virtual long getNumItemNode(int);
  virtual ~PartitionCFGHandler() {}
};


#endif  // PARTITION_CFG_HANDLER_H_
