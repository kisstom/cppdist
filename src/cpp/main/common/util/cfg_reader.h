/*
 * cfg_reader.h
 *
 *  Created on: 2013.08.13.
 *      Author: kisstom
 */

#ifndef CFG_READER_H_
#define CFG_READER_H_

#include <tr1/unordered_map>
#include <vector>

using std::tr1::unordered_map;
using std::string;
using std::vector;

class CfgReader {
public:
  void read(char* file);
  void trim (string& s);
  unordered_map<string, string>* getParams();
  vector<std::pair<string, string> >* getHostAndPort();
private:
  unordered_map<string, string>* params_;
  vector<std::pair<string, string> >* hostAndPort_;
};




#endif /* CFG_READER_H_ */
