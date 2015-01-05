/*
 * sender_buffer.h
 *
 *  Created on: 2013.08.01.
 *      Author: kisstom
 */

#ifndef SENDER_BUFFER_H_
#define SENDER_BUFFER_H_

#include <vector>
#include "serializer.h"
#include "socket/isocket/isocket_manager.h"
#include "log4cpp/Category.hh"
#include <gtest/gtest_prod.h>

using std::vector;

class SenderBuffer {
public:
	SenderBuffer();
	~SenderBuffer();
	template <class T>
	void store(int index, T a);

	template <class T>
	void store(int index, T* bytes, unsigned size);

	int getBufferSize(int);
	int getBufferNum();
	void setBreak(int);
	void setFinish(int);

	void resizeBufferNum(int size);
	void resizeBuffers(int size);
	void emptyBuffer(int);

	bool canAdd(int, int);
	void setSocketManager(ISocketManager*);
private:

	vector<char*> pack_;
	ISocketManager* socketManager_;
	Serializer serializer_;
	vector<int> pack_size_;
	int send_limit_;
	log4cpp::Category* logger_;

	FRIEND_TEST(SenderBufferTest, test);
	FRIEND_TEST(SenderBufferTest, testSep);
	FRIEND_TEST(SenderBufferTest, testEmpty);
	FRIEND_TEST(SimpleMockTestSetup, testSetup);
	friend class SimpleMockTestSetup;
	friend class SimpleMockTestMultiSetup;
};

inline SenderBuffer::SenderBuffer() {
	logger_ = &log4cpp::Category::getInstance(std::string("SenderBuffer"));
}

inline SenderBuffer::~SenderBuffer() {
	for (int i = 0; i < (int) pack_.size(); ++i) {
		delete[] pack_[i];
	}
}

inline void SenderBuffer::emptyBuffer(int index) {
	socketManager_->sendToNode(pack_size_[index], pack_[index], index);
	pack_size_[index] = 0;
}

inline void SenderBuffer::resizeBufferNum(int size) {
	logger_->info("Resizing sender buffer num to %d.", size);
	pack_.resize(size);
	pack_size_.resize(size);
}

// TODO constructor should delete objects
inline void SenderBuffer::resizeBuffers(int size) {
	logger_->info("Resizing buffers to size %d.", size);
	for (int i = 0; i < (int) pack_.size(); ++i) {
		pack_[i] = new char[size];
		pack_size_[i] = 0;
	}
	send_limit_ = size;
}

inline int SenderBuffer::getBufferSize(int index) {
	return pack_size_[index];
}

template <class T>
inline void SenderBuffer::store(int index, T a) {
	pack_size_[index] += serializer_.store(pack_[index] + pack_size_[index], a);
}

template <class T>
inline void SenderBuffer::store(int index, T* bytes, unsigned size) {
  pack_size_[index] += serializer_.store(pack_[index] + pack_size_[index], bytes, size);
}

inline void SenderBuffer::setBreak(int index) {
	pack_size_[index] += serializer_.setBreak(pack_[index] + pack_size_[index]);
}

inline void SenderBuffer::setFinish(int index) {
	pack_size_[index] += serializer_.setFinish(pack_[index] + pack_size_[index]);
}

inline int SenderBuffer::getBufferNum() {
	return pack_.size();
}

inline bool SenderBuffer::canAdd(int index, int size) {
	return size + pack_size_[index] <= send_limit_;
}

inline void SenderBuffer::setSocketManager(ISocketManager* manager) {
	socketManager_ = manager;
}

#endif /* SENDER_BUFFER_H_ */
