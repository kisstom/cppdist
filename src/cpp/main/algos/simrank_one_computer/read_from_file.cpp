/*
 * read_from_file.cpp
 *
 *  Created on: 2014.01.30.
 *      Author: mrita
 */

#include "read_from_file.h"


ReadFromFile::ReadFromFile(){
  logger = &log4cpp::Category::getInstance(std::string("ReadGraphFromFile"));
}


EdgelistContainer* ReadFromFile::read(string inFileName){
  logger->info("Reading data.");
  matrix = new EdgelistContainer();
  matrix->initContainers();
  matrix->setMinnode(0);
  
  EdgeListBuilder builder;
  builder.setContainer(matrix);
  builder.buildFromFile(inFileName);
  matrix->setMinnode(0);
  
  logger->info("matrix data read");
  return matrix;
}
