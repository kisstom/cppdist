/*
 * convert_edge_list_to_base.h
 *
 *  Created on: 2014.06.19.
 *      Author: kisstom
 */

#ifndef CONVERT_EDGE_LIST_TO_BASE_H_
#define CONVERT_EDGE_LIST_TO_BASE_H_

#include <stdio.h>
#include "vector"
#include "tr1/unordered_map"

using std::tr1::unordered_map;
using std::vector;

class ConvertEdgeListToBase {
public:
  void read(char* inpF, char* outpF);
};


#endif /* CONVERT_EDGE_LIST_TO_BASE_H_ */
