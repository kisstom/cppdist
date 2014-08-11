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
private:
	log4cpp::Category* logger_;
};

inline Serializer::Serializer() {
	endOfRecord_[0] = '\30';
	endOfMessage_[0] = '\4';
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
  return sizeof(a);
}

inline bool Serializer::hasNext(char *buf) {
	return *buf == '\30';
}

#endif /* SERIALIZER_H_ */
