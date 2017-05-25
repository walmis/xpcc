// coding: utf-8
/* Copyright (c) 2011, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#pragma once

#include "../../../stm32.hpp"
#include <xpcc/architecture/peripheral/i2c.hpp>
#include <xpcc/architecture/driver/atomic.hpp>
#include "../gpio/gpio.hpp"
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
		//uint32_t rawRate = frequency;
		//static_assert(rawRate <= 400000, "The STM32 does not support High I2C baudrate.");
		//static_assert(peripheralFrequency/2 >= rawRate, "The APB1 frequency needs to be at least 2x higher than I2C baudrate.");

		// set the prescaler rate register
		// ccrPrescaler = peripheralFrequency / (2 * I2CFrequency)
		//uint16_t prescaler;

		delegate = 0;

		// Enable clock
		switch(i2cid) {
		case 1:
			RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
			NVIC_EnableIRQ(I2C1_ER_IRQn);
			NVIC_EnableIRQ(I2C1_EV_IRQn);

			tx_stream = new dma::DMAStream(dma::Stream::DMA1_1);
			rx_stream = new dma::DMAStream(dma::Stream::DMA1_0);

			rx_chan = dma::Channel::Channel_1;
			tx_chan = dma::Channel::Channel_0;

			break;
		case 2:
			RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
			NVIC_EnableIRQ(I2C2_ER_IRQn);
			NVIC_EnableIRQ(I2C2_EV_IRQn);

			tx_stream = new dma::DMAStream(dma::Stream::DMA1_7);
			rx_stream = new dma::DMAStream(dma::Stream::DMA1_3);

			rx_chan = dma::Channel::Channel_7;
			tx_chan = dma::Channel::Channel_7;
			break;
		case 3:
			RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
			NVIC_EnableIRQ(I2C3_ER_IRQn);
			NVIC_EnableIRQ(I2C3_EV_IRQn);

			tx_stream = new dma::DMAStream(dma::Stream::DMA1_4);
			rx_stream = new dma::DMAStream(dma::Stream::DMA1_2);

			rx_chan = dma::Channel::Channel_3;
			tx_chan = dma::Channel::Channel_3;
			break;
		}

		setIrqPriority(0xF);

		I2Cx->CR1 = I2C_CR1_SWRST; 		// reset module
		I2Cx->CR1 = 0;


		// DEBUG_STREAM("ccrPrescaler=" << prescaler);
		setBusFrequency(frequency);
		busFrequency = frequency;

		dma::Config cfg;

		cfg.channel(tx_chan)
				->peripheralDataSize(dma::PeripheralDataSize::Byte)
				->memoryDataSize(dma::MemoryDataSize::Byte)
				->memoryInc(dma::MemoryInc::Enable)
				->periphBaseAddress((uint32_t)&I2Cx->DR)
				->priority(dma::Prioriy::High)
				->xferDirection(dma::XferDir::MemoryToPeripheral);

		tx_stream->init(cfg);
		tx_stream->attachCallback(dma_tx_end);

		cfg.channel(rx_chan)
				->xferDirection(dma::XferDir::PeripheralToMemory);

		rx_stream->init(cfg);
		rx_stream->attachCallback(dma_rx_end);

		I2Cx->FLTR = 0x04;
		I2Cx->FLTR |= I2C_FLTR_ANOFF;

		I2Cx->CR2 |= I2C_CR2_ITERREN | I2C_CR2_DMAEN;

		I2Cx->CR1 |= I2C_CR1_PE; // Enable peripheral
		reset();
	}

	static inline void setIrqPriority(uint8_t prio) {
		switch(i2cid) {
		case 1:
			NVIC_SetPriority(I2C1_ER_IRQn, prio);
			NVIC_SetPriority(I2C1_EV_IRQn, prio);
			NVIC_SetPriority(DMA1_Stream0_IRQn, prio);
			NVIC_SetPriority(DMA1_Stream1_IRQn, prio);
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

	static void setBusFrequency(uint32_t frequency_hz) {
		int peripheralFrequency = Clocks::getPCLK1Frequency();
		uint32_t prescaler;

		I2Cx->CR2 = static_cast<uint32_t>(peripheralFrequency) / 1000000;

		if(frequency_hz >= 400000) {
			I2Cx->TRISE = (static_cast<uint32_t>(peripheralFrequency)/1000000 * 300 / 1000) + 1;

			prescaler = peripheralFrequency / (3 * frequency_hz);
			I2Cx->CCR = prescaler | I2C_CCR_FS;

		} else {
			I2Cx->TRISE = (static_cast<uint32_t>(peripheralFrequency) / 1000000) + 1;
			prescaler = peripheralFrequency / (2 * frequency_hz);
			I2Cx->CCR = prescaler;
		}
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
		tx_stream->disable();
		rx_stream->disable();

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

		rx_stream->disable();
		tx_stream->disable();

		I2Cx->CR2 &= ~I2C_CR2_ITEVTEN;

		//busReset();

		if (delegate) {
			{
				xpcc::atomic::Lock l;

				if(delegate->busSpeed != 0) {
					setBusFrequency(busFrequency); //restore bus frequency
				}

				I2cTransaction* old = 0;

				delegate->error = error;
				old = delegate;
				//attach new delegate in the chain
				delegate = delegate->next;

				old->next = 0;
				old->stopped(cause);
			}

			if(delegate) {
				//i2stop();

				//newSession = true;
				//i2start();
				callStarting();
				//XPCC_LOG_DEBUG .printf("d %x-> %x\n", old, delegate);
			}
		}
	}
	// end documentation inherited

	static void dma_tx_end() {

		if(tx_stream->isError()) {
			XPCC_LOG_DEBUG .printf("dtxe err %x\n", tx_stream->getInterruptFlags());
		}

		tx_stream->disable();

		if(nextOperation == Operation::Write) {
			checkNextOperation = CHECK_NEXT_OPERATION_YES;
			checkNextOp();

		} else {

			/* Enables interrupts to catch BTF event meaning transmission part complete.
			 Interrupt handler will decide to generate STOP or to begin receiving part
			 of R/W transaction itself.*/
			I2Cx->CR2 |= I2C_CR2_ITEVTEN;
			//XPCC_LOG_DEBUG .printf("t\n");
		}
	}

	static void dma_rx_end() {
		if(rx_stream->isError()) {
			XPCC_LOG_DEBUG .printf("drxe err %x\n", tx_stream->getInterruptFlags());
		}

		rx_stream->disable();

		I2Cx->CR2 &= ~I2C_CR2_LAST;
		I2Cx->CR1 &= ~I2C_CR1_ACK;
		I2Cx->CR1 |= I2C_CR1_STOP;

		//XPCC_LOG_DEBUG .printf("r\n");

		checkNextOperation = CHECK_NEXT_OPERATION_YES_NO_STOP_BIT;

		checkNextOp();

	}

	// parameter advice
	static void handleIRQ() {
		DEBUG_STREAM("\n--- interrupt ---");
		uint16_t sr1 = I2Cx->SR1;

		if(sr1 == 0 || !delegate) {
			return;
		}

		if (sr1 & I2C_SR1_SB)
		{
			// EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
			DEBUG_STREAM("startbit set");
			//XPCC_LOG_DEBUG .printf("+\n");

			xpcc::I2cTransaction::Starting s = delegate->starting();
			uint8_t address;

			switch (s.next)
			{
				case xpcc::I2c::Operation::Read:
					address = ((s.address<<1) & 0xfe) | xpcc::I2c::READ;
					initializeRead(delegate->reading());


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
		}
		else if (sr1 & I2C_SR1_ADDR) {
			// EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
			DEBUG_STREAM("address sent");
			DEBUG_STREAM("writeBytesLeft=" << writeBytesLeft);
			DEBUG_STREAM("readBytesLeft=" << readBytesLeft);

//			XPCC_LOG_DEBUG .printf("ADDR %d %d\n", tx_stream->getCurrDataCounter(),
//					rx_stream->getCurrDataCounter());
			I2Cx->CR2 &= ~I2C_CR2_ITEVTEN;

			if(tx_stream->getCurrDataCounter()) {
			    tx_stream->enable();

			} else if(rx_stream->getCurrDataCounter()) {
				rx_stream->enable();
				I2Cx->CR2 |= I2C_CR2_LAST;

				/* Needed in receiver mode. */
				if (rx_stream->getCurrDataCounter() < 2) {
					I2Cx->CR1 &= ~I2C_CR1_ACK;
				} else {
					I2Cx->CR1 |= I2C_CR1_ACK;
				}
			}

			// Only after setting ACK, read SR2 to clear the ADDR (next byte will start arriving)
			DEBUG_STREAM("clearing ADDR");
			(void)I2Cx->SR2;

			if(!tx_stream->getCurrDataCounter() && !rx_stream->getCurrDataCounter()) {
				checkNextOperation = CHECK_NEXT_OPERATION_YES;
				checkNextOp();
			}

		}

		if (sr1 & I2C_SR1_BTF)
		{
			// EV8_2
			DEBUG_STREAM("BTF");
			//XPCC_LOG_DEBUG << "BTF\n";

		    /* Catches BTF event after the end of transmission.*/

		    I2Cx->CR2 &= ~I2C_CR2_ITEVTEN;
		    I2Cx->DR; //this will clear BTF interrupt

		    checkNextOperation = CHECK_NEXT_OPERATION_YES;
		    checkNextOp();
		}


	}

	static void checkNextOp() {
		if (checkNextOperation > CHECK_NEXT_OPERATION_NO_WAIT_FOR_BTF)
		{
			switch (nextOperation)
			{
				case xpcc::I2c::Operation::Write:
					initializeWrite(delegate->writing());
					tx_stream->enable();
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
					I2Cx->CR2 &= ~(I2C_CR2_ITEVTEN /*| I2C_CR2_ITERREN*/);
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
		//XPCC_LOG_DEBUG << "ERROR!\n";

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

		checkNextOperation = CHECK_NEXT_OPERATION_NO;

		DEBUG_STREAM("disable interrupts");
		I2Cx->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN /*| I2C_CR2_ITERREN*/);

		reset(xpcc::I2c::DetachCause::ErrorCondition);
	}

//private:
	static CheckNextOperation checkNextOperation;
	// buffer management
	static xpcc::I2c::Operation nextOperation;

	static xpcc::I2cMaster::Error error;

	static dma::DMAStream* tx_stream;
	static dma::DMAStream* rx_stream;
	static dma::Channel rx_chan;
	static dma::Channel tx_chan;

	static uint32_t busFrequency;

	// delegating
	static xpcc::I2cTransaction * volatile delegate;

	static inline void
	initializeWrite(xpcc::I2cTransaction::Writing w)
	{
		tx_stream->memoryTargetConfig((uint32_t)w.buffer, dma::Memory::Memory_0);
		tx_stream->setCurrDataCounter(w.size);
		rx_stream->setCurrDataCounter(0);

		nextOperation = static_cast<xpcc::I2c::Operation>(w.next);
	}

	static inline void
	initializeRead(xpcc::I2cTransaction::Reading r)
	{

		rx_stream->memoryTargetConfig((uint32_t)r.buffer, dma::Memory::Memory_0);
		rx_stream->setCurrDataCounter(r.size);
		tx_stream->setCurrDataCounter(0);

		nextOperation = static_cast<xpcc::I2c::Operation>(r.next);
	}

	static inline void
	initializeStopAfterAddress()
	{
		nextOperation = xpcc::I2c::Operation::Stop;
	}

	static inline void
	initializeRestartAfterAddress()
	{
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


		//XPCC_LOG_DEBUG .printf("--CR1 %x %d\n", I2Cx->CR1, deadlockPreventer);
		// If the bus is busy during a starting condition, we generate an error and detach the transaction
		// Before a restart condition the clock line is pulled low, and this check would trigger falsely.
		if ((I2Cx->SR2 & I2C_SR2_BUSY) and (nextOperation != xpcc::I2c::Operation::Restart))
		{
			// we wait a short amount of time for the bus to become free.
//			deadlockPreventer = 10000;
//			while ((I2Cx->SR2 & I2C_SR2_BUSY) and (deadlockPreventer-- > 0))
//				;
//			if((I2Cx->SR2 & I2C_SR2_BUSY)) {
//				XPCC_LOG_DEBUG .printf("bus bsy\n");
//			}

			if (I2Cx->SR2 & I2C_SR2_BUSY)
			{
				//XPCC_LOG_DEBUG .printf("Busy\n");
				// either SDA or SCL is low, which leads to irrecoverable deadlock.
				// Call error handler manually to detach the transaction object and resolve the deadlock.
				// Further transactions may not succeed either, but will not lead to a deadlock.
				error = xpcc::I2cMaster::Error::BusCondition;
				reset(DetachCause::ErrorCondition);
				return;
			}
		}

		DEBUG_STREAM("callStarting");
		error = xpcc::I2cMaster::Error::NoError;
		checkNextOperation = CHECK_NEXT_OPERATION_NO;

		//I2Cx->CR1 &= ~I2C_CR1_POS;
		//I2Cx->SR1 = 0;
		//I2Cx->SR2 = 0;
		if(delegate->busSpeed != 0) {
			setBusFrequency(delegate->busSpeed * 1000);
		}

		// and enable interrupts
		DEBUG_STREAM("enable interrupts");
		//I2Cx->CR2 &= ~I2C_CR2_ITBUFEN;
		I2Cx->CR2 |= I2C_CR2_ITERREN | I2C_CR2_DMAEN | I2C_CR2_ITEVTEN;

		// generate startcondition
		I2Cx->CR1 |= I2C_CR1_START;
	}

	static bool attachDelegate(xpcc::I2cTransaction *d) {
		if(delegate == d)
			return false;


		if(d->attaching()) {
			//DMB();
			xpcc::atomic::Lock l;
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
xpcc::I2cMaster::Error I2cMaster<i2cid>::error;
template <int i2cid>
dma::DMAStream* I2cMaster<i2cid>::tx_stream;
template <int i2cid>
dma::DMAStream* I2cMaster<i2cid>::rx_stream;
template <int i2cid>
dma::Channel I2cMaster<i2cid>::rx_chan;
template <int i2cid>
dma::Channel I2cMaster<i2cid>::tx_chan;
template <int i2cid>
uint32_t I2cMaster<i2cid>::busFrequency;

typedef I2cMaster<1> I2cMaster1;
typedef I2cMaster<2> I2cMaster2;
typedef I2cMaster<3> I2cMaster3;


#undef SERIAL_DEBUGGING
} // namespace stm32

} // namespace xpcc

