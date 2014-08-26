/*
 * serializer.h
 *
 *  Created on: 2013.08.08.
 *      Author: kisstom
 */

#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <string.h>
#include <stdio.h>

#include "log4cpp/Category.hh"

class Serializer {
public:
	Serializer();

	template <class T>
	int store(char* buf, T a);

	template <class T>
	int store(char* buf, T* bytes, int size);

	bool hasNext(char *);
	int setFinish(char*);
	int setBreak(char*);

	char endOfRecord_[1];
	char endOfMessage_[1];

  const char TRANSMISSION_END;
  const char TRANSMISSION_BLOCK_END;
private:
	log4cpp::Category* logger_;
};

inline Serializer::Serializer() : TRANSMISSION_END(4), TRANSMISSION_BLOCK_END(23) {
	endOfRecord_[0] = TRANSMISSION_BLOCK_END;
	endOfMessage_[0] = TRANSMISSION_END;
	logger_ = &log4cpp::Category::getInstance(std::string("Serializer"));
}

inline int Serializer::setBreak(char* buf) {
	memcpy(buf, endOfRecord_, 1);
	return 1;
}

inline int Serializer::setFinish(char* buf) {
	memcpy(buf, endOfMessage_, 1);
	return 1;
}

template <class T>
inline int Serializer::store(char* buf, T a) {
  memcpy(buf, reinterpret_cast<char*>(&(a)), sizeof(a));
  return sizeof(a);
}

template <class T>
inline int Serializer::store(char* buf, T* a, int size) {
  memcpy(buf, reinterpret_cast<char*>(a), size);
  return size;
}

inline bool Serializer::hasNext(char *buf) {
	return *buf == TRANSMISSION_BLOCK_END;
}

#endif /* SERIALIZER_H_ */
