/*
 * read_from_file.h
 *
 *  Created on: 2014.01.30.
 *      Author: mrita
 */

#ifndef READ_FROM_FILE_H_
#define READ_FROM_FILE_H_

#include <log4cpp/Category.hh>

#include "../../common/graph/edgelist_container.h"
#include "../../common/graph/edge_list_builder.h"

class ReadFromFile {
  private:
    EdgelistContainer* matrix;
    log4cpp::Category* logger;
   
  
  public:
    ReadFromFile();
    EdgelistContainer* read(string inFileName);
     
};

#endif /* READ_FROM_FILE_H_ */
