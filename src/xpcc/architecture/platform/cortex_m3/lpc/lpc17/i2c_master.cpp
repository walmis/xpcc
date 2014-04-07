// coding: utf-8
/* Copyright (c) 2011, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */


/* To debug the internal state of the driver, you can instantiate a
 * xpcc::IOStream, which will then be used to dump state data of the operations
 * via the serial port.
 * Be advised, that a typical I2C read/write operation can take 10 to 100 longer
 * because the strings have to be copied during the interrupt!
 *
 * You can enable serial debugging with this define.
 */


#include "i2c_master.hpp"


//	uint16_t sr1 = I2C{{ id }}->SR1;
//
//	if (sr1 & I2C_SR1_SB)
//	{
//
//		DEBUG_STREAM("startbit set");
//
//		xpcc::I2cDelegate::Starting s = delegate->starting();
//		uint8_t address;
//
//		switch (s.next)
//		{
//			case xpcc::I2c::Operation::Read:
//				address = (s.address & 0xfe) | xpcc::I2c::READ;
//				initializeRead(delegate->reading());
//				if (readBytesLeft <= 2)
//				{
//					DEBUG_STREAM("NACK");
//					I2C{{ id }}->CR1 &= ~I2C_CR1_ACK;
//				}
//				else
//				{
//					DEBUG_STREAM("ACK");
//					I2C{{ id }}->CR1 |= I2C_CR1_ACK;
//				}
//				if (readBytesLeft == 2)
//				{
//					DEBUG_STREAM("POS");
//					I2C{{ id }}->CR1 |= I2C_CR1_POS;
//				}
//				DEBUG_STREAM("read op: reading=" << readBytesLeft);
//				break;
//
//			case xpcc::I2c::Operation::Write:
//				address = (s.address & 0xfe) | xpcc::I2c::WRITE;
//				initializeWrite(delegate->writing());
//				DEBUG_STREAM("write op: writing=" << writeBytesLeft);
//				break;
//
//			case xpcc::I2c::Operation::Restart:
//				address = (s.address & 0xfe) | xpcc::I2c::WRITE;
//				initializeRestartAfterAddress();
//				DEBUG_STREAM("restart op");
//				break;
//
//			default:
//			case xpcc::I2c::Operation::Stop:
//				address = (s.address & 0xfe) | xpcc::I2c::WRITE;
//				initializeStopAfterAddress();
//				DEBUG_STREAM("stop op");
//				break;
//		}
//
//		I2C{{ id }}->DR = address;
//	}
//
//
//
//	else if (sr1 & I2C_SR1_ADDR)
//	{
//		// EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
//		DEBUG_STREAM("address sent");
//		DEBUG_STREAM("writeBytesLeft=" << writeBytesLeft);
//		DEBUG_STREAM("readBytesLeft=" << readBytesLeft);
//
//		if (writeBytesLeft > 0 || readBytesLeft > 3)
//		{
//			DEBUG_STREAM("enable buffers");
//			I2C{{ id }}->CR2 |= I2C_CR2_ITBUFEN;
//		}
//		if (!readBytesLeft && !writeBytesLeft)
//		{
//			checkNextOperation = CHECK_NEXT_OPERATION_YES;
//		}
//
//
//		DEBUG_STREAM("clearing ADDR");
//		uint16_t sr2 = I2C{{ id }}->SR2;
//		(void) sr2;
//
//
//		if (readBytesLeft == 1)
//		{
//			DEBUG_STREAM("STOP");
//			I2C{{ id }}->CR1 |= I2C_CR1_STOP;
//
//#if WAIT_FOR_STOP_LOW
//			DEBUG_STREAM("waiting for stop");
//			while (I2C{{ id }}->CR1 & I2C_CR1_STOP)
//				;
//#endif
//
//			uint16_t dr = I2C{{ id }}->DR;
//			*readPointer++ = dr & 0xff;
//			readBytesLeft = 0;
//			checkNextOperation = CHECK_NEXT_OPERATION_YES_NO_STOP_BIT;
//		}
//	}
//
//
//
//	else if (sr1 & I2C_SR1_TXE)
//	{
//		// EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR
//		// EV8: TxE=1, shift register not empty, data register empty, cleared by writing DR
//		if (writeBytesLeft > 0)
//		{
//			DEBUG_STREAM("tx more bytes");
//			I2C{{ id }}->DR = *writePointer++; // write data
//			writeBytesLeft--;
//
//			DEBUG_STREAM("TXE: writeBytesLeft=" << writeBytesLeft);
//
//			checkNextOperation = CHECK_NEXT_OPERATION_NO_WAIT_FOR_BTF;
//		}
//		// no else!
//		if (writeBytesLeft == 0)
//		{
//			// disable TxE, and wait for EV8_2
//			DEBUG_STREAM("last byte transmitted, wait for btf");
//			I2C{{ id }}->CR2 &= ~I2C_CR2_ITBUFEN;
//		}
//	}
//
//
//
//	else if (sr1 & I2C_SR1_RXNE)
//	{
//		if (readBytesLeft > 3)
//		{
//			// EV7: RxNE=1, cleared by reading DR register
//			uint16_t dr = I2C{{ id }}->DR;
//			*readPointer++ = dr & 0xff;
//			readBytesLeft--;
//
//			DEBUG_STREAM("RXNE: readBytesLeft=" << readBytesLeft);
//		}
//
//		if (readBytesLeft <= 3)
//		{
//			// disable RxNE, and wait for BTF
//			DEBUG_STREAM("fourth last byte received, wait for btf");
//			I2C{{ id }}->CR2 &= ~I2C_CR2_ITBUFEN;
//		}
//	}
//
//
//
//	if (sr1 & I2C_SR1_BTF)
//	{
//		// EV8_2
//		DEBUG_STREAM("BTF");
//
//		if (readBytesLeft == 2)
//		{
//			// EV7_1: RxNE=1, cleared by reading DR register, programming STOP=1
//			DEBUG_STREAM("STOP");
//			I2C{{ id }}->CR1 |= I2C_CR1_STOP;
//
//			DEBUG_STREAM("reading data1");
//			uint16_t dr = I2C{{ id }}->DR;
//			*readPointer++ = dr & 0xff;
//
//#if WAIT_FOR_STOP_LOW
//			DEBUG_STREAM("waiting for stop");
//			while (I2C{{ id }}->CR1 & I2C_CR1_STOP)
//				;
//#endif
//
//			DEBUG_STREAM("reading data2");
//			dr = I2C{{ id }}->DR;
//			*readPointer++ = dr & 0xff;
//
//			readBytesLeft = 0;
//			checkNextOperation = CHECK_NEXT_OPERATION_YES_NO_STOP_BIT;
//		}
//
//		if (readBytesLeft == 3)
//		{
//			// EV7_1: RxNE=1, cleared by reading DR register, programming ACK=0
//			I2C{{ id }}->CR1 &= ~I2C_CR1_ACK;
//			DEBUG_STREAM("NACK");
//
//			uint16_t dr = I2C{{ id }}->DR;
//			*readPointer++ = dr & 0xff;
//			readBytesLeft--;
//
//			DEBUG_STREAM("BTF: readBytesLeft=2");
//		}
//
//		if (checkNextOperation == CHECK_NEXT_OPERATION_NO_WAIT_FOR_BTF
//			&& writeBytesLeft == 0)
//		{
//			// EV8_2: TxE=1, BTF = 1, Program Stop request.
//			// TxE and BTF are cleared by hardware by the Stop condition
//			DEBUG_STREAM("BTF, write=0");
//			checkNextOperation = CHECK_NEXT_OPERATION_YES;
//		}
//	}
//
//
//
//	if (checkNextOperation > CHECK_NEXT_OPERATION_NO_WAIT_FOR_BTF)
//	{
//		switch (nextOperation)
//		{
//			case xpcc::I2c::Operation::Write:
//				if (checkNextOperation != CHECK_NEXT_OPERATION_YES_NO_STOP_BIT)
//				{
//					initializeWrite(delegate->writing());
//					// reenable TXE
//					I2C{{ id }}->CR2 |= I2C_CR2_ITBUFEN;
//					DEBUG_STREAM("write op");
//				}
//				break;
//
//			case xpcc::I2c::Operation::Restart:
//				callStarting();
//				DEBUG_STREAM("restart op");
//				break;
//
//			default:
//				if (checkNextOperation != CHECK_NEXT_OPERATION_YES_NO_STOP_BIT)
//				{
//					I2C{{ id }}->CR1 |= I2C_CR1_STOP;
//					DEBUG_STREAM("STOP");
//				}
//
//				DEBUG_STREAM("disable interrupts");
//				I2C{{ id }}->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN);
//				if (delegate) delegate->stopped(xpcc::I2c::DetachCause::NormalStop);
//				delegate = 0;
//				DEBUG_STREAM("write finished");
//				break;
//		}
//		checkNextOperation = CHECK_NEXT_OPERATION_NO;
//	}



// ----------------------------------------------------------------------------

//extern "C"
//void I2C0_IRQHandler() {
//	XPCC_LOG_DEBUG .printf("a");
//	I2C_Handler<xpcc::lpc17::I2cMaster0>();
//}
//
//extern "C"
//void I2C1_IRQHandler() {
//	I2C_Handler<xpcc::lpc17::I2cMaster1>();
//}
//
//extern "C"
//void I2C2_IRQHandler() {
//	I2C_Handler<xpcc::lpc17::I2cMaster2>();
//}

// ----------------------------------------------------------------------------



