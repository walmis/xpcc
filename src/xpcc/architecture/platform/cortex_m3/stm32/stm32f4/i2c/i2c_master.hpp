// coding: utf-8
/* Copyright (c) 2011, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#ifndef XPCC_STM32_I2C_x_HPP
#define XPCC_STM32_I2C_x_HPP

#include "../../../stm32.hpp"
#include <xpcc/architecture/peripheral/i2c.hpp>
#include <xpcc/architecture/driver/atomic.hpp>

namespace xpcc
{

namespace stm32
{

/**
 * I2cMaster implementation of I2Cx module.
 *
 * Interrupts must be enabled.
 * 
 * @author		Georgi Grinshpun
 * @author		Niklas Hauser
 * @ingroup		stm32
 */


#define SERIAL_DEBUGGING 0

#if SERIAL_DEBUGGING
#	include "../uart/uart_5.hpp"
	typedef xpcc::stm32::Uart5 DebugUart;
#	include <xpcc/io/iostream.hpp>
	extern xpcc::IOStream stream;
#	define DEBUG_STREAM(x) //stream << x << "\n"
#	define DEBUG(x) xpcc::stm32::Uart5::write(x)
#else
#	define DEBUG_STREAM(x)
#endif

static I2C_TypeDef* const _i2c[] = {I2C1, I2C2, I2C3};
#define I2Cx (_i2c[i2cid-1])

enum CheckNextOperation
{
	CHECK_NEXT_OPERATION_NO,
	CHECK_NEXT_OPERATION_NO_WAIT_FOR_BTF,
	CHECK_NEXT_OPERATION_YES,
	CHECK_NEXT_OPERATION_YES_NO_STOP_BIT,

};

template <int i2cid>
class I2cMaster : ::xpcc::I2cMaster
{

public:
	/**
	 * Set up the I2C module for master operation.
	 *
	 * @param	rate
	 *		`Standard` or `Fast`, `High` datarate is not supported
	 */
	static ALWAYS_INLINE void
	initialize()
	{
		int peripheralFrequency = Clocks::getPCLK1Frequency();
		uint32_t rawRate = 400000;
		//static_assert(rawRate <= 400000, "The STM32 does not support High I2C baudrate.");
		//static_assert(peripheralFrequency/2 >= rawRate, "The APB1 frequency needs to be at least 2x higher than I2C baudrate.");

		// set the prescaler rate register
		// ccrPrescaler = peripheralFrequency / (2 * I2CFrequency)
		uint16_t prescaler = peripheralFrequency / (2 * rawRate);

		delegate = 0;

		reset();
		// Enable clock
		switch(i2cid) {
		case 1:
			RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
			NVIC_EnableIRQ(I2C1_ER_IRQn);
			NVIC_EnableIRQ(I2C1_EV_IRQn);
			break;
		case 2:
			RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
			NVIC_EnableIRQ(I2C2_ER_IRQn);
			NVIC_EnableIRQ(I2C2_EV_IRQn);
			break;
		case 3:
			RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
			NVIC_EnableIRQ(I2C3_ER_IRQn);
			NVIC_EnableIRQ(I2C3_EV_IRQn);
			break;
		}

		I2Cx->CR1 = I2C_CR1_SWRST; 		// reset module
		I2Cx->CR1 = 0;

		I2Cx->CR2 = static_cast<uint32_t>(peripheralFrequency) / 1000000;
		I2Cx->CCR = prescaler;
		// DEBUG_STREAM("ccrPrescaler=" << prescaler);
		I2Cx->TRISE = 0x09;

		I2Cx->CR1 |= I2C_CR1_PE; // Enable peripheral
	}

	// start documentation inherited
	static bool
	start(xpcc::I2cDelegate *delegate) {
		return attachDelegate(delegate);
	}

	static Error
	getErrorState() {
		return error;
	}

	static void
	reset(DetachCause cause=DetachCause::SoftwareReset) {
		readBytesLeft = 0;
		writeBytesLeft = 0;
		xpcc::atomic::Lock l;

		if (delegate != 0) {
			I2cDelegate* old = delegate;
			//attach new delegate in the chain
			delegate = delegate->next;

			old->next = 0;
			{
				xpcc::atomic::Unlock u;
				old->stopped(cause);
			}

			if(delegate) {
				//i2stop();

				//newSession = true;
				//i2start();
				callStarting();
				//XPCC_LOG_DEBUG .printf("d %x-> %x\n", old, delegate);
			} else {
				//i2stop();
			}

		} else {
			//i2stop();
		}
	}
	// end documentation inherited

	// parameter advice
	static void handleIRQ() {
		DEBUG_STREAM("\n--- interrupt ---");

		uint16_t sr1 = I2Cx->SR1;

		if (sr1 & I2C_SR1_SB)
		{
			// EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
			DEBUG_STREAM("startbit set");

			xpcc::I2cDelegate::Starting s = delegate->starting();
			uint8_t address;

			switch (s.next)
			{
				case xpcc::I2c::Operation::Read:
					address = (s.address & 0xfe) | xpcc::I2c::READ;
					initializeRead(delegate->reading());
					if (readBytesLeft <= 2)
					{
						DEBUG_STREAM("NACK");
						I2Cx->CR1 &= ~I2C_CR1_ACK;
					}
					else
					{
						DEBUG_STREAM("ACK");
						I2Cx->CR1 |= I2C_CR1_ACK;
					}
					if (readBytesLeft == 2)
					{
						DEBUG_STREAM("POS");
						I2Cx->CR1 |= I2C_CR1_POS;
					}
					DEBUG_STREAM("read op: reading=" << readBytesLeft);
					break;

				case xpcc::I2c::Operation::Write:
					address = (s.address & 0xfe) | xpcc::I2c::WRITE;
					initializeWrite(delegate->writing());
					DEBUG_STREAM("write op: writing=" << writeBytesLeft);
					break;

				case xpcc::I2c::Operation::Restart:
					address = (s.address & 0xfe) | xpcc::I2c::WRITE;
					initializeRestartAfterAddress();
					DEBUG_STREAM("restart op");
					break;

				default:
				case xpcc::I2c::Operation::Stop:
					address = (s.address & 0xfe) | xpcc::I2c::WRITE;
					initializeStopAfterAddress();
					DEBUG_STREAM("stop op");
					break;
			}

			I2Cx->DR = address;
		}
		else if (sr1 & I2C_SR1_ADDR) {

			// EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
			DEBUG_STREAM("address sent");
			DEBUG_STREAM("writeBytesLeft=" << writeBytesLeft);
			DEBUG_STREAM("readBytesLeft=" << readBytesLeft);

			if (writeBytesLeft > 0 || readBytesLeft > 3)
			{
				DEBUG_STREAM("enable buffers");
				I2Cx->CR2 |= I2C_CR2_ITBUFEN;
			}
			if (!readBytesLeft && !writeBytesLeft)
			{
				checkNextOperation = CHECK_NEXT_OPERATION_YES;
			}

			DEBUG_STREAM("clearing ADDR");
			uint16_t sr2 = I2Cx->SR2;
			(void) sr2;

			if (readBytesLeft == 1)
			{
				DEBUG_STREAM("STOP");
				I2Cx->CR1 |= I2C_CR1_STOP;

	#if WAIT_FOR_STOP_LOW
				DEBUG_STREAM("waiting for stop");
				while (I2Cx->CR1 & I2C_CR1_STOP)
					;
	#endif

				uint16_t dr = I2Cx->DR;
				*readPointer++ = dr & 0xff;
				readBytesLeft = 0;
				checkNextOperation = CHECK_NEXT_OPERATION_YES_NO_STOP_BIT;
			}
		}

		else if (sr1 & I2C_SR1_TXE)
		{
			// EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR
			// EV8: TxE=1, shift register not empty, data register empty, cleared by writing DR
			if (writeBytesLeft > 0)
			{
				DEBUG_STREAM("tx more bytes");
				I2Cx->DR = *writePointer++; // write data
				writeBytesLeft--;

				DEBUG_STREAM("TXE: writeBytesLeft=" << writeBytesLeft);

				checkNextOperation = CHECK_NEXT_OPERATION_NO_WAIT_FOR_BTF;
			}
			// no else!
			if (writeBytesLeft == 0)
			{
				// disable TxE, and wait for EV8_2
				DEBUG_STREAM("last byte transmitted, wait for btf");
				I2Cx->CR2 &= ~I2C_CR2_ITBUFEN;
			}
		}

		else if (sr1 & I2C_SR1_RXNE)
		{
			if (readBytesLeft > 3)
			{
				// EV7: RxNE=1, cleared by reading DR register
				uint16_t dr = I2Cx->DR;
				*readPointer++ = dr & 0xff;
				readBytesLeft--;

				DEBUG_STREAM("RXNE: readBytesLeft=" << readBytesLeft);
			}

			if (readBytesLeft <= 3)
			{
				// disable RxNE, and wait for BTF
				DEBUG_STREAM("fourth last byte received, wait for btf");
				I2Cx->CR2 &= ~I2C_CR2_ITBUFEN;
			}
		}

		if (sr1 & I2C_SR1_BTF)
		{
			// EV8_2
			DEBUG_STREAM("BTF");

			if (readBytesLeft == 2)
			{
				// EV7_1: RxNE=1, cleared by reading DR register, programming STOP=1
				DEBUG_STREAM("STOP");
				I2Cx->CR1 |= I2C_CR1_STOP;

				DEBUG_STREAM("reading data1");
				uint16_t dr = I2Cx->DR;
				*readPointer++ = dr & 0xff;

	#if WAIT_FOR_STOP_LOW
				DEBUG_STREAM("waiting for stop");
				while (I2Cx->CR1 & I2C_CR1_STOP)
					;
	#endif

				DEBUG_STREAM("reading data2");
				dr = I2Cx->DR;
				*readPointer++ = dr & 0xff;

				readBytesLeft = 0;
				checkNextOperation = CHECK_NEXT_OPERATION_YES_NO_STOP_BIT;
			}

			if (readBytesLeft == 3)
			{
				// EV7_1: RxNE=1, cleared by reading DR register, programming ACK=0
				I2Cx->CR1 &= ~I2C_CR1_ACK;
				DEBUG_STREAM("NACK");

				uint16_t dr = I2Cx->DR;
				*readPointer++ = dr & 0xff;
				readBytesLeft--;

				DEBUG_STREAM("BTF: readBytesLeft=2");
			}

			if (checkNextOperation == CHECK_NEXT_OPERATION_NO_WAIT_FOR_BTF
				&& writeBytesLeft == 0)
			{
				// EV8_2: TxE=1, BTF = 1, Program Stop request.
				// TxE and BTF are cleared by hardware by the Stop condition
				DEBUG_STREAM("BTF, write=0");
				checkNextOperation = CHECK_NEXT_OPERATION_YES;
			}
		}

		if (checkNextOperation > CHECK_NEXT_OPERATION_NO_WAIT_FOR_BTF)
		{
			switch (nextOperation)
			{
				case xpcc::I2c::Operation::Write:
					if (checkNextOperation != CHECK_NEXT_OPERATION_YES_NO_STOP_BIT)
					{
						initializeWrite(delegate->writing());
						// reenable TXE
						I2Cx->CR2 |= I2C_CR2_ITBUFEN;
						DEBUG_STREAM("write op");
					}
					break;

				case xpcc::I2c::Operation::Restart:
					callStarting();
					DEBUG_STREAM("restart op");
					break;

				default:
					if (checkNextOperation != CHECK_NEXT_OPERATION_YES_NO_STOP_BIT)
					{
						I2Cx->CR1 |= I2C_CR1_STOP;
						DEBUG_STREAM("STOP");
					}

					DEBUG_STREAM("disable interrupts");
					I2Cx->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN);
					reset(xpcc::I2c::DetachCause::NormalStop);
					DEBUG_STREAM("write finished");
					break;
			}
			checkNextOperation = CHECK_NEXT_OPERATION_NO;
		}
	}

	static void handleERR_IRQ() {
		DEBUG_STREAM("ERROR!");
		uint16_t sr1 = I2Cx->SR1;

		if (sr1 & I2C_SR1_BERR)
		{
			DEBUG_STREAM("BUS ERROR");
			I2Cx->CR1 |= I2C_CR1_STOP;
			error = xpcc::I2cMaster::Error::BusCondition;
		}
		else if (sr1 & I2C_SR1_AF)
		{	// acknowledge fail
			I2Cx->CR1 |= I2C_CR1_STOP;
			DEBUG_STREAM("ACK FAIL");
			 // may also be ADDRESS_NACK
			error = xpcc::I2cMaster::Error::DataNack;
		}
		else if (sr1 & I2C_SR1_ARLO)
		{	// arbitration lost
			DEBUG_STREAM("ARBITRATION LOST");
			error = xpcc::I2cMaster::Error::ArbitrationLost;
		}
		else
		{
			DEBUG_STREAM("UNKNOWN");
			error = xpcc::I2cMaster::Error::Unknown;
		}

		reset(xpcc::I2c::DetachCause::ErrorCondition);
		// Overrun error is not handled here separately

		// Clear flags and interrupts
		I2Cx->CR1 &= ~I2C_CR1_POS;
		I2Cx->SR1 = 0;
		I2Cx->SR2 = 0;
		writeBytesLeft = 0;
		readBytesLeft = 0;
		checkNextOperation = CHECK_NEXT_OPERATION_NO;

		DEBUG_STREAM("disable interrupts");
		I2Cx->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN);
	}


private:
	static CheckNextOperation checkNextOperation;
	// buffer management
	static xpcc::I2c::Operation nextOperation;

	static uint8_t *readPointer;
	static const uint8_t *writePointer;
	static std::size_t readBytesLeft;
	static std::size_t writeBytesLeft;
	static xpcc::I2cMaster::Error error;

	// delegating
	static xpcc::I2cDelegate * volatile delegate;

	static inline void
	initializeWrite(xpcc::I2cDelegate::Writing w)
	{
		writePointer = w.buffer;
		writeBytesLeft = w.size;
		readBytesLeft = 0;
		nextOperation = static_cast<xpcc::I2c::Operation>(w.next);
	}

	static inline void
	initializeRead(xpcc::I2cDelegate::Reading r)
	{
		readPointer = r.buffer;
		readBytesLeft = r.size;
		writeBytesLeft = 0;
		nextOperation = static_cast<xpcc::I2c::Operation>(r.next);
	}

	static inline void
	initializeStopAfterAddress()
	{
		writeBytesLeft = 0;
		readBytesLeft = 0;
		nextOperation = xpcc::I2c::Operation::Stop;
	}

	static inline void
	initializeRestartAfterAddress()
	{
		writeBytesLeft = 0;
		readBytesLeft = 0;
		nextOperation = xpcc::I2c::Operation::Restart;
	}

	static inline void
	callStarting()
	{
		DEBUG_STREAM("callStarting");

		I2Cx->CR1 &= ~I2C_CR1_POS;
		I2Cx->SR1 = 0;
		I2Cx->SR2 = 0;

		// generate startcondition
		I2Cx->CR1 |= I2C_CR1_START;
		checkNextOperation = CHECK_NEXT_OPERATION_NO;

		// and enable interrupts
		DEBUG_STREAM("enable interrupts");
		I2Cx->CR2 &= ~I2C_CR2_ITBUFEN;
		I2Cx->CR2 |= I2C_CR2_ITEVTEN | I2C_CR2_ITERREN;
	}

private:

	static bool attachDelegate(xpcc::I2cDelegate *d) {
		xpcc::atomic::Lock l;

		if(delegate == d)
			return false;

		if(d->attaching()) {
			//DMB();
			if(!delegate) {
				//ERR << 'a';
				delegate = d;
				delegate->next = 0;
				//newSession = true;
				//i2start();
				callStarting();
				return true;
			} else {
				xpcc::I2cDelegate* p = delegate;
				while(p->next) {
					if(p == d) {
						//the same delegate is already in the chain
						//XPCC_LOG_ERROR << "same delegate in chain\n";
						d->stopped(DetachCause::SoftwareReset);
						return false;
					}
					p = p->next;
				}

				//add the new delegate to the end of the list
				d->next = 0;
				p->next	= d;

				//XPCC_LOG_ERROR .printf("s2 %x %x\n", delegate, d);
				//intCmd(true);
				return true;
			}
		}
		d->stopped(DetachCause::SoftwareReset);
		return false;
	}


	static void
	initializeWithPrescaler(uint16_t baud);
};

template <int i2cid>
CheckNextOperation I2cMaster<i2cid>::checkNextOperation;
template <int i2cid>
xpcc::I2c::Operation I2cMaster<i2cid>::nextOperation;
// buffer management
template <int i2cid>
uint8_t *I2cMaster<i2cid>::readPointer;
template <int i2cid>
const uint8_t *I2cMaster<i2cid>::writePointer;
template <int i2cid>
std::size_t I2cMaster<i2cid>::readBytesLeft;
template <int i2cid>
std::size_t I2cMaster<i2cid>::writeBytesLeft;
template <int i2cid>
xpcc::I2cMaster::Error I2cMaster<i2cid>::error;

typedef I2cMaster<1> I2cMaster1;
typedef I2cMaster<2> I2cMaster2;
typedef I2cMaster<3> I2cMaster3;


#undef SERIAL_DEBUGGING
} // namespace stm32

} // namespace xpcc

#endif // XPCC_STM32_I2C_x_HPP
