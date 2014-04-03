// coding: utf-8
/* Copyright (c) 2011, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */

/*
 * This driver was not so straight forward to implement, because the official
 * documentation by ST is not so clear about the reading operation.
 * So here is the easier to understand version (# = wait for next interrupt).
 *
 * Writing:
 * --------
 *	- set start bit
 *	#
 *	- check SB bit
 *	- write address
 *	#
 *	- check ADDR bit
 *	- clear ADDR bit
 *	- if no bytes are to be written, check for next operation immidiately
 *	#
 *	- check TXE bit
 *	- write data
 *	- on write of last bytes, disable Buffer interrupt, and wait for BTF interrupt
 *	#
 *	- check BTF bit
 *	- if no bytes left, check for next operation
 *
 * It if important to note, that we wait for the last byte transfer to complete
 * before checking the next operation.
 *
 *
 * Reading is a lot more complicated. In master read mode, the controller can
 * stretch the SCL line low, while there is new received data in the registers.
 *
 * The data and the shift register together hold two bytes, so we have to send
 * NACK and the STOP condition two bytes in advance and then read both bytes!!!
 *
 * 1-byte read:
 * ------------
 *	- set start bit (RESTART!)
 *	#
 *	- check SB bit
 *	- set ACK low
 *	- write address
 *	#
 *	- check ADDR bit
 *	- clear ADDR bit
 *	- set STOP high
 *	- (wait until STOP low)
 *	- read data 1
 *	- check for next operation
 *
 * 2-byte read:
 * ------------
 *	- set start bit (RESTART!)
 *	#
 *	- check SB bit
 *	- set ACK low
 *	- set POS high (must be used ONLY in two byte transfers, clear it afterwards!)
 *	- write address
 *	#
 *	- check ADDR bit
 *	- clear ADDR bit
 *	#
 *	- check BTF bit
 *	- set STOP high
 *	- read data 1
 *	- (wait until STOP low)
 *	- read data 2
 *	- check for next operation
 *
 * 3-byte read:
 * ------------
 *	- set start bit (RESTART!)
 *	#
 *	- check SB bit
 *	- write address
 *	#
 *	- check ADDR bit
 *	- clear ADDR bit
 *	#
 *	- check BTF bit
 *	- set ACK LOW
 *	- read data 1
 *	#
 *	- check BTF bit
 *	- set STOP high
 *	- read data 1
 *	- (wait until STOP low)
 *	- read data 2
 *	- check for next operation
 *
 * N-byte read:
 * -------------
 *	- set start bit (RESTART!)
 *	#
 *	- check SB bit
 *	- write address
 *	#
 *	- check ADDR bit
 *	- enable Buffer Interrupt
 *	- clear ADDR bit
 *	#
 *	- check RXNE bit
 *	- read data < N-3
 *	#
 *	- check RXNE bit
 *	- read data N-3
 *	- disable Buffer Interrupt
 *	#
 *	- check BTF bit
 *	- set ACK low
 *	- read data N-2
 *	#
 *	- check BTF bit
 *	- set STOP high
 *	- read data N-1
 *	- (wait until STOP low)
 *	- read data N
 *	- check for next operation
 *
 * Please read the documentation of the driver before you attempt to fix this
 * driver. I strongly recommend the use of an logic analizer or oscilloscope,
 * to confirm the drivers behavior.
 * The event states are labeled as 'EVn_m' in reference to the manual.
 */

/* To debug the internal state of the driver, you can instantiate a
 * xpcc::IOStream, which will then be used to dump state data of the operations
 * via the serial port.
 * Be advised, that a typical I2C read/write operation can take 10 to 100 longer
 * because the strings have to be copied during the interrupt!
 *
 * You can enable serial debugging with this define.
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
#	define DEBUG(x)
#endif

/*
 * It is mildly recommended to wait for the STOP operation to finish, before
 * reading the shift register, however since no interrupt is generated after a
 * STOP condition, the only solution is to busy wait until the STOP bit is
 * cleared by hardware.
 * Busy waiting in this interrupt is potentially dangerous and can make program
 * behavior unpredictable and debugging very cumbersome.
 * Therefore, this is disabled by default, you _can_ reenable it with this define.
 */
#define WAIT_FOR_STOP_LOW 0

#include "i2c_master_{{ id }}.hpp"

namespace
{
	// parameter advice
	static xpcc::I2c::Operation nextOperation;
	enum CheckNextOperation
	{
		CHECK_NEXT_OPERATION_NO,
		CHECK_NEXT_OPERATION_NO_WAIT_FOR_BTF,
		CHECK_NEXT_OPERATION_YES,
		CHECK_NEXT_OPERATION_YES_NO_STOP_BIT,

	};
	CheckNextOperation checkNextOperation = CHECK_NEXT_OPERATION_NO;

	// buffer management
	static uint8_t *readPointer;
	static const uint8_t *writePointer;
	static std::size_t readBytesLeft = 0;
	static std::size_t writeBytesLeft = 0;

	// delegating
	static xpcc::I2cDelegate *delegate = 0;
	static xpcc::I2cMaster::Error error(xpcc::I2cMaster::Error::NoError);

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

		I2C{{ id }}->CR1 &= ~I2C_CR1_POS;
		I2C{{ id }}->SR1 = 0;
		I2C{{ id }}->SR2 = 0;

		// generate startcondition
		I2C{{ id }}->CR1 |= I2C_CR1_START;
		checkNextOperation = CHECK_NEXT_OPERATION_NO;

		// and enable interrupts
		DEBUG_STREAM("enable interrupts");
		I2C{{ id }}->CR2 &= ~I2C_CR2_ITBUFEN;
		I2C{{ id }}->CR2 |= I2C_CR2_ITEVTEN | I2C_CR2_ITERREN;
	}
}

// ----------------------------------------------------------------------------
extern "C" void
I2C{{ id }}_EV_IRQHandler(void)
{
	DEBUG_STREAM("\n--- interrupt ---");

	uint16_t sr1 = I2C{{ id }}->SR1;

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
					I2C{{ id }}->CR1 &= ~I2C_CR1_ACK;
				}
				else
				{
					DEBUG_STREAM("ACK");
					I2C{{ id }}->CR1 |= I2C_CR1_ACK;
				}
				if (readBytesLeft == 2)
				{
					DEBUG_STREAM("POS");
					I2C{{ id }}->CR1 |= I2C_CR1_POS;
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

		I2C{{ id }}->DR = address;
	}



	else if (sr1 & I2C_SR1_ADDR)
	{
		// EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
		DEBUG_STREAM("address sent");
		DEBUG_STREAM("writeBytesLeft=" << writeBytesLeft);
		DEBUG_STREAM("readBytesLeft=" << readBytesLeft);

		if (writeBytesLeft > 0 || readBytesLeft > 3)
		{
			DEBUG_STREAM("enable buffers");
			I2C{{ id }}->CR2 |= I2C_CR2_ITBUFEN;
		}
		if (!readBytesLeft && !writeBytesLeft)
		{
			checkNextOperation = CHECK_NEXT_OPERATION_YES;
		}


		DEBUG_STREAM("clearing ADDR");
		uint16_t sr2 = I2C{{ id }}->SR2;
		(void) sr2;


		if (readBytesLeft == 1)
		{
			DEBUG_STREAM("STOP");
			I2C{{ id }}->CR1 |= I2C_CR1_STOP;

#if WAIT_FOR_STOP_LOW
			DEBUG_STREAM("waiting for stop");
			while (I2C{{ id }}->CR1 & I2C_CR1_STOP)
				;
#endif

			uint16_t dr = I2C{{ id }}->DR;
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
			I2C{{ id }}->DR = *writePointer++; // write data
			writeBytesLeft--;

			DEBUG_STREAM("TXE: writeBytesLeft=" << writeBytesLeft);

			checkNextOperation = CHECK_NEXT_OPERATION_NO_WAIT_FOR_BTF;
		}
		// no else!
		if (writeBytesLeft == 0)
		{
			// disable TxE, and wait for EV8_2
			DEBUG_STREAM("last byte transmitted, wait for btf");
			I2C{{ id }}->CR2 &= ~I2C_CR2_ITBUFEN;
		}
	}



	else if (sr1 & I2C_SR1_RXNE)
	{
		if (readBytesLeft > 3)
		{
			// EV7: RxNE=1, cleared by reading DR register
			uint16_t dr = I2C{{ id }}->DR;
			*readPointer++ = dr & 0xff;
			readBytesLeft--;

			DEBUG_STREAM("RXNE: readBytesLeft=" << readBytesLeft);
		}

		if (readBytesLeft <= 3)
		{
			// disable RxNE, and wait for BTF
			DEBUG_STREAM("fourth last byte received, wait for btf");
			I2C{{ id }}->CR2 &= ~I2C_CR2_ITBUFEN;
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
			I2C{{ id }}->CR1 |= I2C_CR1_STOP;

			DEBUG_STREAM("reading data1");
			uint16_t dr = I2C{{ id }}->DR;
			*readPointer++ = dr & 0xff;

#if WAIT_FOR_STOP_LOW
			DEBUG_STREAM("waiting for stop");
			while (I2C{{ id }}->CR1 & I2C_CR1_STOP)
				;
#endif

			DEBUG_STREAM("reading data2");
			dr = I2C{{ id }}->DR;
			*readPointer++ = dr & 0xff;

			readBytesLeft = 0;
			checkNextOperation = CHECK_NEXT_OPERATION_YES_NO_STOP_BIT;
		}

		if (readBytesLeft == 3)
		{
			// EV7_1: RxNE=1, cleared by reading DR register, programming ACK=0
			I2C{{ id }}->CR1 &= ~I2C_CR1_ACK;
			DEBUG_STREAM("NACK");

			uint16_t dr = I2C{{ id }}->DR;
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
					I2C{{ id }}->CR2 |= I2C_CR2_ITBUFEN;
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
					I2C{{ id }}->CR1 |= I2C_CR1_STOP;
					DEBUG_STREAM("STOP");
				}

				DEBUG_STREAM("disable interrupts");
				I2C{{ id }}->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN);
				if (delegate) delegate->stopped(xpcc::I2c::DetachCause::NormalStop);
				delegate = 0;
				DEBUG_STREAM("write finished");
				break;
		}
		checkNextOperation = CHECK_NEXT_OPERATION_NO;
	}
}

// ----------------------------------------------------------------------------
extern "C" void
I2C{{ id }}_ER_IRQHandler(void)
{
	DEBUG_STREAM("ERROR!");
	uint16_t sr1 = I2C{{ id }}->SR1;

	if (sr1 & I2C_SR1_BERR)
	{
		DEBUG_STREAM("BUS ERROR");
		I2C{{ id }}->CR1 |= I2C_CR1_STOP;
		error = xpcc::I2cMaster::Error::BusCondition;
	}
	else if (sr1 & I2C_SR1_AF)
	{	// acknowledge fail
		I2C{{ id }}->CR1 |= I2C_CR1_STOP;
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

	if (delegate)
	{
		delegate->stopped(xpcc::I2c::DetachCause::ErrorCondition);
		delegate = 0;
	}

	// Overrun error is not handled here separately

	// Clear flags and interrupts
	I2C{{ id }}->CR1 &= ~I2C_CR1_POS;
	I2C{{ id }}->SR1 = 0;
	I2C{{ id }}->SR2 = 0;
	writeBytesLeft = 0;
	readBytesLeft = 0;
	checkNextOperation = CHECK_NEXT_OPERATION_NO;

	DEBUG_STREAM("disable interrupts");
	I2C{{ id }}->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN);
}

// ----------------------------------------------------------------------------

void
xpcc::lpc17::I2cMaster{{ id }}::reset(DetachCause cause)
{
	readBytesLeft = 0;
	writeBytesLeft = 0;
	if (delegate) delegate->stopped(cause);
	delegate = 0;
}

// MARK: - ownership
bool
xpcc::lpc17::I2cMaster{{ id }}::start(xpcc::I2cDelegate *delegate)
{
	if (!::delegate && delegate && delegate->attaching())
	{
		DEBUG_STREAM("\n###\n");
		::delegate = delegate;
		callStarting();

		return true;
	}
	return false;
}

bool
xpcc::lpc17::I2cMaster{{ id }}::startBlocking(xpcc::I2cDelegate *delegate)
{
	if (!::delegate && delegate && delegate->attaching())
	{
		DEBUG_STREAM("\n###\n");
		::delegate = delegate;
		callStarting();

		// memory barrier, yes you really need it
		__asm__ volatile ("dmb" ::: "memory");

		while(::delegate != 0)
			;

		return true;
	}
	return false;
}

xpcc::I2cMaster::Error
xpcc::lpc17::I2cMaster{{ id }}::getErrorState()
{
	return error;
}

