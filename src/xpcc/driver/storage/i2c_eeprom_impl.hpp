// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------

#ifndef XPCC__I2C_EEPROM_HPP
	#error	"Don't include this file directly, use 'i2c_eeprom.hpp' instead!"
#endif

// ----------------------------------------------------------------------------
template <typename I2cMaster>
xpcc::I2cEeprom<I2cMaster>::I2cEeprom(uint8_t address)
:	address(address), state(xpcc::I2c::AdapterState::Idle)
{
	initialize(0, 0, 0, 0);
}

// // MARK: - write operations
template <typename I2cMaster>
bool
xpcc::I2cEeprom<I2cMaster>::writeByte(uint16_t address, uint8_t data)
{
	while(I2cMaster::isBusy()) {
		xpcc::TickerTask::yield();
	}

	int i = 0;
	if(0)
		buffer[i++] = address >> 8;
	buffer[i++] = address;
	buffer[i++] = data;
	initialize(buffer, i, 0, 0);
	
	return I2cMaster::startBlocking(this);
}

template <typename I2cMaster>
bool
xpcc::I2cEeprom<I2cMaster>::write(uint16_t address, const uint8_t *data, uint8_t bytes)
{
	uint8_t i;
	uint8_t n;

	Timeout<> t(50);

	while(I2cMaster::isBusy()) {
		xpcc::TickerTask::yield();
	}

	if(!isAvailable()) {
		return false;
	}

	while(bytes > 0) {
		i = 0;
		if(0)
			buffer[i++] = address >> 8;
		buffer[i++] = address;

		n = 16 - (address & 0x0F);
		if(bytes < n)
			n = bytes;

		//XPCC_LOG_DEBUG .printf("write addr:%d len:%d ptr:%x\n", address, n, data);
		//XPCC_LOG_DEBUG .dump_buffer((uint8_t*)data, n);

		//XPCC_LOG_DEBUG .printf("busy %d\n", (state == xpcc::I2c::AdapterState::Busy));

		initialize(buffer, i, data, n, 0, 0);
		bool res = I2cMaster::startBlocking(this);
		if(!res)
			return false;

		while(!isAvailable()) {
			TickerTask::yield();
			if(t.isExpired()) {
				return false;
			}
		}

		bytes -= n;
		address += n;
		data += n;
	}

	return true;
}

template <typename I2cMaster> template <typename T>
bool
xpcc::I2cEeprom<I2cMaster>::write(uint16_t address, const T& data)
{
	//XPCC_LOG_DEBUG .printf("write a:%d L:%d\n", address, sizeof(T));
	return write(address, reinterpret_cast<const uint8_t *>(&data), sizeof(T));
}

// MARK: - read operations
template <typename I2cMaster>
bool
xpcc::I2cEeprom<I2cMaster>::readByte(uint16_t address, uint8_t &data)
{
	while(I2cMaster::isBusy()) {
		xpcc::TickerTask::yield();
	}
	int i = 0;
	if(0)
		buffer[i++] = address >> 8;
	buffer[i++] = address;
	initialize(buffer, i, &data, 1);
	
	return I2cMaster::startBlocking(this);
}

template <typename I2cMaster>
bool
xpcc::I2cEeprom<I2cMaster>::read(uint16_t address, uint8_t *data, uint8_t bytes)
{
	while(I2cMaster::isBusy()) {
		xpcc::TickerTask::yield();
	}

	int i = 0;
	if(0)
		buffer[i++] = address >> 8;
	buffer[i++] = address;
	initialize(buffer, i, data, bytes);
	
	return I2cMaster::startBlocking(this);
}

template <typename I2cMaster> template <typename T>
bool
xpcc::I2cEeprom<I2cMaster>::read(uint16_t address, T& data)
{
	return read(address, reinterpret_cast<uint8_t *>(&data), sizeof(T));
}

// MARK: - available
template <typename I2cMaster>
bool
xpcc::I2cEeprom<I2cMaster>::isAvailable()
{
	initialize(0, 0, 0, 0);
	
	if (I2cMaster::startBlocking(this))
	{
		return (state == xpcc::I2c::AdapterState::Idle);
	}
	return false;
}

// MARK: - i2c delegates
template <typename I2cMaster>
bool
xpcc::I2cEeprom<I2cMaster>::initialize(const uint8_t* writeBuffer, uint8_t writeSize, uint8_t* readBuffer, uint8_t readSize)
{
	if (state != xpcc::I2c::AdapterState::Busy)
	{
		this->address = address;
		this->readBuffer = readBuffer;
		this->readSize = readSize;
		this->writeBuffer = writeBuffer;
		this->writeSize = writeSize;
		isReading = writeSize ? false : true;
		twoBuffers = false;
		return true;
	}
	return false;
}

template <typename I2cMaster>
bool
xpcc::I2cEeprom<I2cMaster>::initialize(const uint8_t* auxWriteBuffer, uint8_t auxWriteSize, const uint8_t* writeBuffer, uint8_t writeSize, uint8_t* readBuffer, uint8_t readSize)
{
	if (state != xpcc::I2c::AdapterState::Busy)
	{
		this->address = address;
		this->readBuffer = readBuffer;
		this->readSize = readSize;
		this->auxWriteBuffer = auxWriteBuffer;
		this->auxWriteSize = auxWriteSize;
		this->writeBuffer = writeBuffer;
		this->writeSize = writeSize;
		isReading = writeSize ? false : true;
		twoBuffers = true;
		return true;
	}
	return false;
}

template <typename I2cMaster>
bool
xpcc::I2cEeprom<I2cMaster>::attaching()
{
	if (state == xpcc::I2c::AdapterState::Busy)
		return false;
	state = xpcc::I2c::AdapterState::Busy;
	return true;
}

template <typename I2cMaster>
xpcc::I2cDelegate::Starting
xpcc::I2cEeprom<I2cMaster>::starting()
{
	Starting s;
	s.address = address;
	if (isReading) {
		s.next = readSize ? Operation::Read : Operation::Stop;
	}
	else {
		s.next = writeSize ? Operation::Write : Operation::Stop;
	}
	isReading = !isReading;
	return s;
}

template <typename I2cMaster>
xpcc::I2cDelegate::Writing
xpcc::I2cEeprom<I2cMaster>::writing()
{
	Writing w;
	if (twoBuffers)
	{
		w.buffer = auxWriteBuffer;
		w.size = auxWriteSize;
		w.next = OperationAfterWrite::Write;
		twoBuffers = false;
	}
	else {
		w.buffer = writeBuffer;
		w.size = writeSize;
		w.next = readSize ? OperationAfterWrite::Restart : OperationAfterWrite::Stop;
	}
	return w;
}

template <typename I2cMaster>
xpcc::I2cDelegate::Reading
xpcc::I2cEeprom<I2cMaster>::reading()
{
	Reading r;
	r.buffer = readBuffer;
	r.size = readSize;
	r.next = OperationAfterRead::Stop;
	return r;
}

template <typename I2cMaster>
void
xpcc::I2cEeprom<I2cMaster>::stopped(DetachCause cause)
{
	isReading = false;
	twoBuffers = false;
	state = (cause == DetachCause::NormalStop) ? AdapterState::Idle : AdapterState::Error;
}
