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
 *	- if no bytes are to be written, check for next operation immediately
 *	#
 *	- check TXE bit
 *	- write data
 *	- on write of last bytes, disable Buffer interrupt, and wait for BTF interrupt
 *	#
 *	- check BTF bit
 *	- if no bytes left, check for next operation
 *
 * It is important to note, that we wait for the last byte transfer to complete
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
 * driver. I strongly recommend the use of an logic analyzer or oscilloscope,
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
	initialize(uint32_t frequency = 400000)
	{
		int peripheralFrequency = Clocks::getPCLK1Frequency();
		uint32_t rawRate = frequency;
		//static_assert(rawRate <= 400000, "The STM32 does not support High I2C baudrate.");
		//static_assert(peripheralFrequency/2 >= rawRate, "The APB1 frequency needs to be at least 2x higher than I2C baudrate.");

		// set the prescaler rate register
		// ccrPrescaler = peripheralFrequency / (2 * I2CFrequency)
		uint16_t prescaler = peripheralFrequency / (2 * rawRate);

		delegate = 0;

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

		setIrqPriority(0xF);

		I2Cx->CR1 = I2C_CR1_SWRST; 		// reset module
		I2Cx->CR1 = 0;

		I2Cx->CR2 = static_cast<uint32_t>(peripheralFrequency) / 1000000;
		I2Cx->CCR = prescaler;

		// DEBUG_STREAM("ccrPrescaler=" << prescaler);
		if(frequency > 200000) {
			I2Cx->TRISE = (static_cast<uint32_t>(peripheralFrequency)/1000000 * 500 / 1000) + 1;
		} else {
			I2Cx->TRISE = (static_cast<uint32_t>(peripheralFrequency) / 1000000) + 1;
		}

		//I2Cx->FLTR = 0x02;
		I2Cx->FLTR |= I2C_FLTR_ANOFF;

		I2Cx->CR1 |= I2C_CR1_PE; // Enable peripheral
		I2Cx->CR2 |= I2C_CR2_ITERREN;

		reset();
	}

	static inline void setIrqPriority(uint8_t prio) {
		switch(i2cid) {
		case 1:
			NVIC_SetPriority(I2C1_ER_IRQn, prio);
			NVIC_SetPriority(I2C1_EV_IRQn, prio);
			break;
		case 2:
			NVIC_SetPriority(I2C2_ER_IRQn, prio);
			NVIC_SetPriority(I2C2_EV_IRQn, prio);
			break;
		case 3:
			NVIC_SetPriority(I2C3_ER_IRQn, prio);
			NVIC_SetPriority(I2C3_EV_IRQn, prio);
			break;
		}
	}

	// start documentation inherited
	static bool
	start(xpcc::I2cTransaction *delegate) {
		return attachDelegate(delegate);
	}

	static Error
	getErrorState() {
		return error;
	}

	static bool
	resetTransaction(I2cTransaction* transaction) {
		xpcc::atomic::Lock l;

		if(delegate == transaction) {
			reset();
			return true;
		} else {
			I2cTransaction* d = delegate;
			while(d) {
				if(d->next == transaction) {
					d->next->stopped(DetachCause::SoftwareReset);
					d->next = d->next->next;
					return true;
				}
				d = d->next;
			}
		}

		return false;
	}

	static void busReset() {
		//XPCC_LOG_ERROR << "I2C BUS RESET\n";

		//save registers
		//uint32_t CR1 = I2Cx->CR1 & 0xFF; //mask only setting bits

		//reset all transactions
//		I2cTransaction* d = delegate;
//		while(d) {
//			d->stopped(DetachCause::SoftwareReset);
//
//			d = d->next;
//		}
//		delegate = 0;

		//soft reset the bus
		uint32_t CR1 = 1;
		uint32_t CR2 = I2Cx->CR2;
		uint32_t CCR = I2Cx->CCR;
		uint32_t TRISE = I2Cx->TRISE;
		uint32_t FLTR = I2Cx->FLTR;

		//reset peripheral
		I2Cx->CR1 = I2C_CR1_SWRST;
		//wait for reset
		while(I2Cx->CR2 != 0);
		I2Cx->CR1 = 0;

		//restore registers
		I2Cx->TRISE = TRISE;
		I2Cx->FLTR = FLTR;
		I2Cx->CCR = CCR;
		I2Cx->CR2 = CR2;
		I2Cx->CR1 = CR1;

	}


	/* resets current transaction,
	 * starts next transaction if available */
	static void
	reset(DetachCause cause=DetachCause::SoftwareReset) {


		readBytesLeft = 0;
		writeBytesLeft = 0;
		//I2Cx->CR1 &= ~I2C_CR1_PE;

		//busReset();

		if (delegate != 0) {
			I2cTransaction* old = 0;
			{
				xpcc::atomic::Lock l;
				delegate->errno = error;
				old = delegate;
				//attach new delegate in the chain
				delegate = delegate->next;
			}

			old->next = 0;
			old->stopped(cause);

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
		//XPCC_LOG_DEBUG << sr1 << endl;

		if(!delegate) {
			XPCC_LOG_ERROR << "I2C IRQ DELEGATE NULL\n";
			busReset();
			return;
		}

		if (sr1 & I2C_SR1_SB)
		{
			// EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
			DEBUG_STREAM("startbit set");

			xpcc::I2cTransaction::Starting s = delegate->starting();
			uint8_t address;

			switch (s.next)
			{
				case xpcc::I2c::Operation::Read:
					address = ((s.address<<1) & 0xfe) | xpcc::I2c::READ;
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
					address = ((s.address<<1) & 0xfe) | xpcc::I2c::WRITE;
					initializeWrite(delegate->writing());
					DEBUG_STREAM("write op: writing=" << writeBytesLeft);
					break;

				case xpcc::I2c::Operation::Restart:
					address = ((s.address<<1) & 0xfe) | xpcc::I2c::WRITE;
					initializeRestartAfterAddress();
					DEBUG_STREAM("restart op");
					break;

				default:
				case xpcc::I2c::Operation::Stop:
					address = ((s.address<<1) & 0xfe) | xpcc::I2c::WRITE;
					initializeStopAfterAddress();
					DEBUG_STREAM("stop op");
					break;
			}

			I2Cx->DR = address;
		} else
		if (sr1 & I2C_SR1_ADDR) {
			// EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
			DEBUG_STREAM("address sent");
			DEBUG_STREAM("writeBytesLeft=" << writeBytesLeft);
			DEBUG_STREAM("readBytesLeft=" << readBytesLeft);

			xpcc::atomic::Lock l;
			// Only after setting ACK, read SR2 to clear the ADDR (next byte will start arriving)
			DEBUG_STREAM("clearing ADDR");
			uint16_t sr2 = I2Cx->SR2;
			(void) sr2;

			//write max 2 bytes immediately
			if(writeBytesLeft > 0) {
				I2Cx->DR = *writePointer++; // write data
				writeBytesLeft--;
				if(writeBytesLeft) {
					I2Cx->DR = *writePointer++; // write data
					writeBytesLeft--;
				}
				if(!writeBytesLeft) {
					checkNextOperation = CHECK_NEXT_OPERATION_NO_WAIT_FOR_BTF;
				}
			} else {
				if (!readBytesLeft)
				{
					checkNextOperation = CHECK_NEXT_OPERATION_YES;
				}
			}
			//if more to write enable BUF interrupt
			//else wait for BTF
			if (writeBytesLeft > 0 || readBytesLeft > 3)
			{
				DEBUG_STREAM("enable buffers");
				I2Cx->CR2 |= I2C_CR2_ITBUFEN;
			}

			if (readBytesLeft == 1)
			{
				// Schedule a Stop
				DEBUG_STREAM("STOP");
				I2Cx->CR1 |= I2C_CR1_STOP;

				// Enable the RXNE: it will trigger as soon as the 1 byte is received to get the result
				I2Cx->CR2 |= I2C_CR2_ITBUFEN;

//				DEBUG_STREAM("waiting for stop");
//				PB15::set();
//				uint_fast32_t deadlockPreventer = 100000;
//				while ((I2Cx->CR1 & I2C_CR1_STOP) && deadlockPreventer-- > 0)
//					;
//				PB15::reset();
//				uint16_t dr = I2Cx->DR;
//				*readPointer++ = dr & 0xff;
//				readBytesLeft = 0;
				checkNextOperation = CHECK_NEXT_OPERATION_NO_WAIT_FOR_BTF;
			}
		}

		if (sr1 & I2C_SR1_TXE)
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

		if (sr1 & I2C_SR1_RXNE)
		{
			if (readBytesLeft > 3)
			{
				// EV7: RxNE=1, cleared by reading DR register
				uint16_t dr = I2Cx->DR;
				*readPointer++ = dr & 0xff;
				readBytesLeft--;

				DEBUG_STREAM("RXNE: readBytesLeft=" << readBytesLeft);
			}

			if(readBytesLeft == 1) {

				uint16_t dr = I2Cx->DR;
				*readPointer++ = dr & 0xff;
				readBytesLeft = 0;

				checkNextOperation = CHECK_NEXT_OPERATION_YES_NO_STOP_BIT;
			} else
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
				xpcc::atomic::Lock l;
				// EV7_1: RxNE=1, cleared by reading DR register, programming STOP=1
				DEBUG_STREAM("STOP");
				I2Cx->CR1 |= I2C_CR1_STOP;

				DEBUG_STREAM("reading data1");
				*readPointer++ = I2Cx->DR & 0xff;

				DEBUG_STREAM("reading data2");
				*readPointer++ = I2Cx->DR & 0xff;

				readBytesLeft = 0;
				checkNextOperation = CHECK_NEXT_OPERATION_YES_NO_STOP_BIT;
			}

			if (readBytesLeft == 3)
			{
				xpcc::atomic::Lock l;
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
				I2Cx->DR; //this will clear BTF interrupt
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
					I2Cx->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN /*| I2C_CR2_ITERREN*/);
					checkNextOperation = CHECK_NEXT_OPERATION_NO;
					reset(xpcc::I2c::DetachCause::NormalStop);
					DEBUG_STREAM("write finished");
					return;
			}
			checkNextOperation = CHECK_NEXT_OPERATION_NO;
		}
	}

	static void handleERR_IRQ() {
		DEBUG_STREAM("ERROR!");

		uint16_t sr1 = I2Cx->SR1;

		if (sr1 & I2C_SR1_BERR)
		{
			XPCC_LOG_ERROR << "I2C BUS ERROR\n";

			busReset();

			//I2Cx->CR1 |= I2C_CR1_STOP;
//			uint_fast32_t deadlockPreventer = 10000;
//			while ((I2Cx->CR1 & I2C_CR1_STOP) && deadlockPreventer-- > 0)
//				;

			error = xpcc::I2cMaster::Error::BusCondition;
		}
		else if (sr1 & I2C_SR1_AF)
		{	// acknowledge fail
			I2Cx->CR1 |= I2C_CR1_STOP;

			uint_fast32_t deadlockPreventer = 1000;
			while ((I2Cx->CR1 & I2C_CR1_STOP) && deadlockPreventer-- > 0)
				;

			//XPCC_LOG_ERROR << "I2C ERR ACK FAIL\n";
			 // may also be ADDRESS_NACK
			error = xpcc::I2cMaster::Error::DataNack;
		}
		else if (sr1 & I2C_SR1_ARLO)
		{	// arbitration lost
			XPCC_LOG_ERROR << "I2C ERR ARBITRATION LOST\n";
			error = xpcc::I2cMaster::Error::ArbitrationLost;
		}
		else
		{
			XPCC_LOG_ERROR << "I2C ERR UNKNOWN\n";
			error = xpcc::I2cMaster::Error::Unknown;
		}
		// Overrun error is not handled here separately

		// Clear flags and interrupts
		I2Cx->CR1 &= ~I2C_CR1_POS;

		I2Cx->SR1 = 0;
		I2Cx->SR2 = 0;

		writeBytesLeft = 0;
		readBytesLeft = 0;
		checkNextOperation = CHECK_NEXT_OPERATION_NO;

		DEBUG_STREAM("disable interrupts");
		I2Cx->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN /*| I2C_CR2_ITERREN*/);

		reset(xpcc::I2c::DetachCause::ErrorCondition);
	}

//private:
	static CheckNextOperation checkNextOperation;
	// buffer management
	static xpcc::I2c::Operation nextOperation;

	static uint8_t *readPointer;
	static const uint8_t *writePointer;
	static std::size_t readBytesLeft;
	static std::size_t writeBytesLeft;
	static xpcc::I2cMaster::Error error;

	// delegating
	static xpcc::I2cTransaction * volatile delegate;

	static inline void
	initializeWrite(xpcc::I2cTransaction::Writing w)
	{
		writePointer = w.buffer;
		writeBytesLeft = w.size;
		readBytesLeft = 0;
		nextOperation = static_cast<xpcc::I2c::Operation>(w.next);
	}

	static inline void
	initializeRead(xpcc::I2cTransaction::Reading r)
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
	//	busReset();
		//I2Cx->CR1 |= I2C_CR1_PE;
//		XPCC_LOG_DEBUG .printf("CR1 %x CR2 %x SR1 %x SR2 %x\n", I2Cx->CR1,
//				I2Cx->CR2, I2Cx->SR1, I2Cx->SR2);


		uint_fast32_t deadlockPreventer = 10000;
		while ((I2Cx->CR1 & I2C_CR1_STOP) and (deadlockPreventer-- > 0))
			;

		if((I2Cx->CR1 & I2C_CR1_STOP)) {
			busReset();
		}

//		//XPCC_LOG_DEBUG .printf("--CR1 %x %d\n", I2Cx->CR1, deadlockPreventer);
//		// If the bus is busy during a starting condition, we generate an error and detach the transaction
//		// Before a restart condition the clock line is pulled low, and this check would trigger falsely.
//		if ((I2Cx->SR2 & I2C_SR2_BUSY) and (nextOperation != xpcc::I2c::Operation::Restart))
//		{
//			// we wait a short amount of time for the bus to become free.
////			deadlockPreventer = 10000;
////			while ((I2Cx->SR2 & I2C_SR2_BUSY) and (deadlockPreventer-- > 0))
////				;
////			if((I2Cx->SR2 & I2C_SR2_BUSY)) {
////				XPCC_LOG_DEBUG .printf("bus bsy\n");
////			}
//
//			if (I2Cx->SR2 & I2C_SR2_BUSY)
//			{
//				//XPCC_LOG_DEBUG .printf("Busy\n");
//				// either SDA or SCL is low, which leads to irrecoverable deadlock.
//				// Call error handler manually to detach the transaction object and resolve the deadlock.
//				// Further transactions may not succeed either, but will not lead to a deadlock.
//				error = xpcc::I2cMaster::Error::BusCondition;
//				reset(DetachCause::ErrorCondition);
//				return;
//			}
//		}

		DEBUG_STREAM("callStarting");
		error = xpcc::I2cMaster::Error::NoError;
		checkNextOperation = CHECK_NEXT_OPERATION_NO;

		I2Cx->CR1 &= ~I2C_CR1_POS;
		I2Cx->SR1 = 0;
		I2Cx->SR2 = 0;

		// and enable interrupts
		DEBUG_STREAM("enable interrupts");
		I2Cx->CR2 &= ~I2C_CR2_ITBUFEN;
		I2Cx->CR2 |= I2C_CR2_ITEVTEN | I2C_CR2_ITERREN;

		// generate startcondition
		I2Cx->CR1 |= I2C_CR1_START;
	}

//private:

	static bool attachDelegate(xpcc::I2cTransaction *d) {
		if(delegate == d)
			return false;

		xpcc::atomic::Lock l;
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
				xpcc::I2cTransaction* p = delegate;
				while(p->next) {
					if(p == d) {
						//the same delegate is already in the chain
						XPCC_LOG_ERROR << "same delegate in chain\n";
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
