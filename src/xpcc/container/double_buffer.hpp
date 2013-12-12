/*
 * double_buffer.hpp
 *
 *  Created on: Nov 22, 2013
 *      Author: walmis
 */

#ifndef DOUBLE_BUFFER_HPP_
#define DOUBLE_BUFFER_HPP_

namespace xpcc {

template<int bufferSize>
class DoubleBuffer {
public:
	struct Buffer {
		Buffer() {
			dataLength = 0;
			readPosition = 0;
		}
		uint8_t data[bufferSize];
		uint32_t dataLength;
		uint16_t readPosition;
	};

	DoubleBuffer() {
		activeBuffer = &a;
	}

	bool getNextByte(uint8_t& byte) {

		if (activeBuffer->dataLength == 0)
			return false;

		byte = activeBuffer->data[activeBuffer->readPosition++];
		if (activeBuffer->readPosition == activeBuffer->dataLength) {
			swapBuffer();
		}

		return true;

	}

	Buffer* getFreeBuffer() {
		if (!activeBuffer->dataLength)
			return activeBuffer;
		if (!a.dataLength)
			return &a;
		if (!b.dataLength)
			return &b;

		return 0;
	}

	uint32_t availData() {
		return (a.dataLength - a.readPosition) + (b.dataLength - b.readPosition);
	}

private:
	void resetBuffer() {
		activeBuffer->dataLength = 0;
		activeBuffer->readPosition = 0;
	}

	void swapBuffer() {
		auto tmp = activeBuffer;

		if (activeBuffer == &a) {
			activeBuffer = &b;
		} else {
			activeBuffer = &a;
		}

		tmp->readPosition = 0;
		tmp->dataLength = 0;
	}

	Buffer* activeBuffer;

	Buffer a;
	Buffer b;
};

}

#endif /* DOUBLE_BUFFER_HPP_ */
