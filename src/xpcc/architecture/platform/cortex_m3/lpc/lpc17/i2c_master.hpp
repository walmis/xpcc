// coding: utf-8
/* Copyright (c) 2011, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#pragma once

#include <xpcc/architecture/platform.hpp>
#include <xpcc/architecture/peripheral/i2c.hpp>
#include <xpcc/architecture/driver/atomic.hpp>

#define DMB() asm volatile("DMB" ::: "memory")

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/*******************************************************************//**
 * I2C Control Set register description
 *********************************************************************/
#define I2C_I2CONSET_AA				((0x04)) /*!< Assert acknowledge flag */
#define I2C_I2CONSET_SI				((0x08)) /*!< I2C interrupt flag */
#define I2C_I2CONSET_STO			((0x10)) /*!< STOP flag */
#define I2C_I2CONSET_STA			((0x20)) /*!< START flag */
#define I2C_I2CONSET_I2EN			((0x40)) /*!< I2C interface enable */

/*******************************************************************//**
 * I2C Control Clear register description
 *********************************************************************/
/** Assert acknowledge Clear bit */
#define I2C_I2CONCLR_AAC			((1<<2))
/** I2C interrupt Clear bit */
#define I2C_I2CONCLR_SIC			((1<<3))
/** START flag Clear bit */
#define I2C_I2CONCLR_STAC			((1<<5))
/** I2C interface Disable bit */
#define I2C_I2CONCLR_I2ENC			((1<<6))

/********************************************************************//**
 * I2C Status Code definition (I2C Status register)
 *********************************************************************/
/* Return Code in I2C status register */
#define I2C_STAT_CODE_BITMASK		((0xF8))

/* I2C return status code definitions ----------------------------- */

/** No relevant information */
#define I2C_I2STAT_NO_INF						((0xF8))

/* Master transmit mode -------------------------------------------- */
/** A start condition has been transmitted */
#define I2C_I2STAT_M_TX_START					((0x08))
/** A repeat start condition has been transmitted */
#define I2C_I2STAT_M_TX_RESTART					((0x10))
/** SLA+W has been transmitted, ACK has been received */
#define I2C_I2STAT_M_TX_SLAW_ACK				((0x18))
/** SLA+W has been transmitted, NACK has been received */
#define I2C_I2STAT_M_TX_SLAW_NACK				((0x20))
/** Data has been transmitted, ACK has been received */
#define I2C_I2STAT_M_TX_DAT_ACK					((0x28))
/** Data has been transmitted, NACK has been received */
#define I2C_I2STAT_M_TX_DAT_NACK				((0x30))
/** Arbitration lost in SLA+R/W or Data bytes */
#define I2C_I2STAT_M_TX_ARB_LOST				((0x38))

/* Master receive mode -------------------------------------------- */
/** A start condition has been transmitted */
#define I2C_I2STAT_M_RX_START					((0x08))
/** A repeat start condition has been transmitted */
#define I2C_I2STAT_M_RX_RESTART					((0x10))
/** Arbitration lost */
#define I2C_I2STAT_M_RX_ARB_LOST				((0x38))
/** SLA+R has been transmitted, ACK has been received */
#define I2C_I2STAT_M_RX_SLAR_ACK				((0x40))
/** SLA+R has been transmitted, NACK has been received */
#define I2C_I2STAT_M_RX_SLAR_NACK				((0x48))
/** Data has been received, ACK has been returned */
#define I2C_I2STAT_M_RX_DAT_ACK					((0x50))
/** Data has been received, NACK has been return */
#define I2C_I2STAT_M_RX_DAT_NACK				((0x58))

/* Slave receive mode -------------------------------------------- */
/** Own slave address has been received, ACK has been returned */
#define I2C_I2STAT_S_RX_SLAW_ACK				((0x60))

/** Arbitration lost in SLA+R/W as master */
#define I2C_I2STAT_S_RX_ARB_LOST_M_SLA			((0x68))
/** Own SLA+W has been received, ACK returned */
//#define I2C_I2STAT_S_RX_SLAW_ACK				((0x68))

/** General call address has been received, ACK has been returned */
#define I2C_I2STAT_S_RX_GENCALL_ACK				((0x70))

/** Arbitration lost in SLA+R/W (GENERAL CALL) as master */
#define I2C_I2STAT_S_RX_ARB_LOST_M_GENCALL		((0x78))
/** General call address has been received, ACK has been returned */
//#define I2C_I2STAT_S_RX_GENCALL_ACK				((0x78))

/** Previously addressed with own SLV address;
 * Data has been received, ACK has been return */
#define I2C_I2STAT_S_RX_PRE_SLA_DAT_ACK			((0x80))
/** Previously addressed with own SLA;
 * Data has been received and NOT ACK has been return */
#define I2C_I2STAT_S_RX_PRE_SLA_DAT_NACK		((0x88))
/** Previously addressed with General Call;
 * Data has been received and ACK has been return */
#define I2C_I2STAT_S_RX_PRE_GENCALL_DAT_ACK		((0x90))
/** Previously addressed with General Call;
 * Data has been received and NOT ACK has been return */
#define I2C_I2STAT_S_RX_PRE_GENCALL_DAT_NACK	((0x98))
/** A STOP condition or repeated START condition has
 * been received while still addressed as SLV/REC
 * (Slave Receive) or SLV/TRX (Slave Transmit) */
#define I2C_I2STAT_S_RX_STA_STO_SLVREC_SLVTRX	((0xA0))

/** Slave transmit mode */
/** Own SLA+R has been received, ACK has been returned */
#define I2C_I2STAT_S_TX_SLAR_ACK				((0xA8))

/** Arbitration lost in SLA+R/W as master */
#define I2C_I2STAT_S_TX_ARB_LOST_M_SLA			((0xB0))
/** Own SLA+R has been received, ACK has been returned */
//#define I2C_I2STAT_S_TX_SLAR_ACK				((0xB0))

/** Data has been transmitted, ACK has been received */
#define I2C_I2STAT_S_TX_DAT_ACK					((0xB8))
/** Data has been transmitted, NACK has been received */
#define I2C_I2STAT_S_TX_DAT_NACK				((0xC0))
/** Last data byte in I2DAT has been transmitted (AA = 0);
 ACK has been received */
#define I2C_I2STAT_S_TX_LAST_DAT_ACK			((0xC8))

/** Time out in case of using I2C slave mode */
#define I2C_SLAVE_TIME_OUT						0x10000UL

/********************************************************************//**
 * I2C Data register definition
 *********************************************************************/
/** Mask for I2DAT register*/
#define I2C_I2DAT_BITMASK			((0xFF))

/** Idle data value will be send out in slave mode in case of the actual
 * expecting data requested from the master is greater than its sending data
 * length that can be supported */
#define I2C_I2DAT_IDLE_CHAR			(0xFF)

/********************************************************************//**
 * I2C Monitor mode control register description
 *********************************************************************/
#define I2C_I2MMCTRL_MM_ENA			((1<<0))		/**< Monitor mode enable */
#define I2C_I2MMCTRL_ENA_SCL		((1<<1))		/**< SCL output enable */
#define I2C_I2MMCTRL_MATCH_ALL		((1<<2))		/**< Select interrupt register match */
#define I2C_I2MMCTRL_BITMASK		((0x07))		/**< Mask for I2MMCTRL register */

/********************************************************************//**
 * I2C Data buffer register description
 *********************************************************************/
/** I2C Data buffer register bit mask */
#define I2DATA_BUFFER_BITMASK		((0xFF))

/********************************************************************//**
 * I2C Slave Address registers definition
 *********************************************************************/
/** General Call enable bit */
#define I2C_I2ADR_GC				((1<<0))
/** I2C Slave Address registers bit mask */
#define I2C_I2ADR_BITMASK			((0xFF))

/********************************************************************//**
 * I2C Mask Register definition
 *********************************************************************/
/** I2C Mask Register mask field */
#define I2C_I2MASK_MASK(n)			((n&0xFE))

/********************************************************************//**
 * I2C SCL HIGH duty cycle Register definition
 *********************************************************************/
/** I2C SCL HIGH duty cycle Register bit mask */
#define I2C_I2SCLH_BITMASK			((0xFFFF))

/********************************************************************//**
 * I2C SCL LOW duty cycle Register definition
 *********************************************************************/
/** I2C SCL LOW duty cycle Register bit mask */
#define I2C_I2SCLL_BITMASK			((0xFFFF))

/* I2C status values */
#define I2C_SETUP_STATUS_ARBF   (1<<8)	/**< Arbitration false */
#define I2C_SETUP_STATUS_NOACKF (1<<9)	/**< No ACK returned */
#define I2C_SETUP_STATUS_DONE   (1<<10)	/**< Status DONE */

/*********************************************************************//**
 * I2C monitor control configuration defines
 **********************************************************************/
#define I2C_MONITOR_CFG_SCL_OUTPUT	I2C_I2MMCTRL_ENA_SCL		/**< SCL output enable */
#define I2C_MONITOR_CFG_MATCHALL	I2C_I2MMCTRL_MATCH_ALL		/**< Select interrupt register match */

/* ---------------- CHECK PARAMETER DEFINITIONS ---------------------------- */
/* Macros check I2C slave address */
#define PARAM_I2C_SLAVEADDR_CH(n)	((n>=0) && (n<=3))

/** Macro to determine if it is valid SSP port number */
#define PARAM_I2Cx(n)	((((uint32_t *)n)==((uint32_t *)LPC_I2C0)) \
|| (((uint32_t *)n)==((uint32_t *)LPC_I2C1)) \
|| (((uint32_t *)n)==((uint32_t *)LPC_I2C2)))

/* Macros check I2C monitor configuration type */
#define PARAM_I2C_MONITOR_CFG(n) ((n==I2C_MONITOR_CFG_SCL_OUTPUT) || (I2C_MONITOR_CFG_MATCHALL))

/**
 * @}
 */

namespace xpcc
{

namespace lpc17
{
#define ERROR(x) XPCC_LOG_ERROR<<x<<endl
#define SERIAL_DEBUGGING 0
#if SERIAL_DEBUGGING
#	define DEBUG_STREAM XPCC_LOG_ERROR
#	define DEBUG(x) XPCC_LOG_ERROR << x << "\n"
#else
#	define DEBUG_STREAM XPCC_LOG_OFF
#	define DEBUG(x)
#endif

#define I2Cx (reinterpret_cast<LPC_I2C_TypeDef*>(P_I2C))



template <int P_I2C>
class I2cMaster : ::xpcc::I2cMaster
{
public:
//	static const TypeId::I2cMaster{{ id }}Sda Sda;
//	static const TypeId::I2cMaster{{ id }}Scl Scl;
	/**
	 * Set up the I2C module for master operation.
	 *
	 * @param	rate
	 *		`Standard` or `Fast`, `High` datarate is not supported
	 */
	template<DataRate rate=DataRate::Standard>
	static ALWAYS_INLINE void
	initialize()
	{

		uint32_t target_clock = 100000;
		if(rate == DataRate::Fast) {
			target_clock = 400000;
		}

		if (I2Cx==LPC_I2C0) {
			CLKPwr::setClkPower(CLKPwr::PType::PCI2C0, true);
			CLKPwr::setClkDiv(CLKPwr::ClkType::I2C0, CLKPwr::ClkDiv::DIV_2);
		}
		else if (I2Cx==LPC_I2C1) {
			CLKPwr::setClkPower(CLKPwr::PType::PCI2C1, true);
			CLKPwr::setClkDiv(CLKPwr::ClkType::I2C1, CLKPwr::ClkDiv::DIV_2);
		}
		else if (I2Cx==LPC_I2C2) {
			CLKPwr::setClkPower(CLKPwr::PType::PCI2C2, true);
			CLKPwr::setClkDiv(CLKPwr::ClkType::I2C2, CLKPwr::ClkDiv::DIV_2);
		}

		uint32_t temp;
		if (I2Cx == LPC_I2C0)
		{
			temp = CLKPwr::getPCLK(CLKPwr::ClkType::I2C0) / target_clock;
		}
		else if (I2Cx == LPC_I2C1)
		{
			temp = CLKPwr::getPCLK(CLKPwr::ClkType::I2C1) / target_clock;
		}
		else if (I2Cx == LPC_I2C2)
		{
			temp = CLKPwr::getPCLK(CLKPwr::ClkType::I2C2) / target_clock;
		}

		reset();

		/* Set the I2C clock value to register */
		I2Cx->I2SCLH = (uint32_t)(temp / 2);
		I2Cx->I2SCLL = (uint32_t)(temp - I2Cx->I2SCLH);

		I2Cx->I2CONCLR = (I2C_I2CONCLR_AAC | I2C_I2CONCLR_STAC | I2C_I2CONCLR_I2ENC | I2C_I2CONCLR_SIC);

		//enable i2c operation
		I2Cx->I2CONSET = I2C_I2CONSET_I2EN;

		intCmd(true);
	}

	static bool
	start(xpcc::I2cTransaction *d) {
		if(!attachDelegate(d)) {
			return false;
		}

		return true;
	}

	static Error
	getErrorState() {
		return error;
	}

	static void
	reset(DetachCause cause=DetachCause::SoftwareReset) {
		xpcc::atomic::Lock l;
		DMB();
		if (delegate != 0) {
			I2cTransaction* old = delegate;
			//attach new delegate in the chain
			delegate = delegate->next;

			old->next = 0;
			{
				xpcc::atomic::Unlock u;
				old->stopped(cause);
			}

			if(delegate) {
				i2stop();

				newSession = true;
				i2start();
				//XPCC_LOG_DEBUG .printf("d %x-> %x\n", old, delegate);
			} else {
				i2stop();
			}

		} else {
			i2stop();
		}
	}

	static void IRQ() {
		uint8_t returnCode = intStatus();

		// there's no relevant information
		if (returnCode == I2C_I2STAT_NO_INF) {
			intClear();
			return;
		}
		//DEBUG_STREAM << "\n--- interrupt ";
		//XPCC_LOG_ERROR .printf("%x\n", returnCode);

		switch (returnCode) {
			case I2C_I2STAT_M_TX_START:
			case I2C_I2STAT_M_TX_RESTART: {
				//clear START bit
				I2Cx->I2CONCLR = I2C_I2CONCLR_STAC;

				//DEBUG_STREAM .printf("Start %x\n", delegate);
				// REPEATED START has been transmitted
				xpcc::I2cTransaction::Starting s = delegate->starting();

				uint8_t address;
				DEBUG_STREAM .printf("Start/next %d\n", s.next);
				switch (s.next) {
					default:
					case xpcc::I2c::Operation::Stop:
						address = ((s.address<<1) & 0xfe) | xpcc::I2c::WRITE;
						initializeStopAfterAddress();
						break;
					case xpcc::I2c::Operation::Restart:
						address = ((s.address<<1) & 0xfe) | xpcc::I2c::WRITE;
						initializeRestartAfterAddress();
						break;
					case xpcc::I2c::Operation::Write:
						address = ((s.address<<1) & 0xfe) | xpcc::I2c::WRITE;
						initializeWrite(delegate->writing());
						break;
					case xpcc::I2c::Operation::Read:
						address = ((s.address<<1) & 0xfe) | xpcc::I2c::READ;
						initializeRead(delegate->reading());
						break;
				}
				DEBUG("send address " << address);
				i2write(address);

				intClear();
		}
		break;

			/* SLA+W has been transmitted, ACK has been received ----------------------*/
		case I2C_I2STAT_M_TX_SLAW_ACK:
			/* Data has been transmitted, ACK has been received */
		case I2C_I2STAT_M_TX_DAT_ACK:
			// Data byte has been transmitted and ACK received
			if (writeBytesLeft != 0) {
				DEBUG("write " << *writePointer);
				i2write(*writePointer++);
				--writeBytesLeft;

				intClear();
				//TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);

				//DEBUG('A');
			} else {
				switch (nextOperation) {
				case xpcc::I2c::Operation::Write:
					DEBUG("Write " << *writePointer);
					initializeWrite(delegate->writing());
					i2write(*writePointer++);

					--writeBytesLeft;

					intClear();
					//DEBUG('A');
					break;

				case xpcc::I2c::Operation::Restart:
					DEBUG("Restart");

					I2Cx->I2CONSET = I2C_I2CONSET_STA;
					I2Cx->I2CONCLR = I2C_I2CONCLR_AAC | I2C_I2CONCLR_SIC;

					break;

				default:
					DEBUG("Stop");

					//i2stop();
					reset(xpcc::I2c::DetachCause::NormalStop);

					break;
				}
			}
			break;

			/* Data has been received, ACK has been returned ----------------------*/
		case I2C_I2STAT_M_RX_DAT_ACK:
			*readPointer++ = i2read();
			DEBUG("I2C_I2STAT_M_RX_DAT_ACK " << *(readPointer-1));
			--readBytesLeft;

			// Fall through...
		case I2C_I2STAT_M_RX_SLAR_ACK:
			// SLA+R has been transmitted and ACK received
			// See if last expected byte will be received ...
			DEBUG("SLAR_DAT_ACK " << readBytesLeft);
			if (readBytesLeft > 1) {
				// Send ACK after reception
				//I2Cx->I2CONSET = I2C_I2CONSET_AA;
				i2ack();
				//TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
				DEBUG("tACK");
			} else {
				// Send NACK after next reception
				i2nack();
				//TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
				DEBUG("tNACK");
			}

			intClear();
			break;

		case I2C_I2STAT_M_RX_DAT_NACK:
			// Data byte has been received and NACK transmitted
			// => Transfer finished
			*readPointer++ = i2read();
			--readBytesLeft;

			DEBUG("RX_DAT_NACK " << readBytesLeft << " " << *(readPointer-1));

			switch (nextOperation) {
			case xpcc::I2c::Operation::Restart:
				DEBUG("Restart");
				i2start();
				break;

			default:
			case xpcc::I2c::Operation::Stop:
				DEBUG("Stop");
				//i2stop();
				reset(xpcc::I2c::DetachCause::NormalStop);
				break;
			}

			break;

		case I2C_I2STAT_M_TX_SLAW_NACK:	// SLA+W transmitted, NACK received
		case I2C_I2STAT_M_RX_SLAR_NACK:	// SLA+R transmitted, NACK received
			if(newSession) {
				error = xpcc::I2cMaster::Error::AddressNack;
				XPCC_LOG_DEBUG << "Error::AddressNack " << delegate << endl;
				newSession = false;
			}
		case I2C_I2STAT_M_TX_DAT_NACK:	// data transmitted, NACK received
			if (newSession) {
				error = xpcc::I2cMaster::Error::DataNack;
				XPCC_LOG_DEBUG << "Error::DataNack " << delegate << endl;
				newSession = false;
			}
		case I2C_I2STAT_M_TX_ARB_LOST:	// RX or TX arbitration lost in SLA or NACK
			if (newSession) {
				XPCC_LOG_DEBUG << "Error::ArbitrationLost" << endl;
				error = xpcc::I2cMaster::Error::ArbitrationLost;
				newSession = false;
				i2reset();
			}

		default:
			if (newSession) {
				error = xpcc::I2cMaster::Error::Unknown;
				XPCC_LOG_DEBUG << "Error::Unknown " << endl;
				newSession = false;
				i2reset();
			}

			reset(xpcc::I2c::DetachCause::ErrorCondition);
			break;
		}
	}

	static inline bool isBusy() {
		return delegate != 0;
	}

	static void i2reset() {
		I2Cx->I2CONCLR = (I2C_I2CONCLR_AAC | I2C_I2CONCLR_STAC | I2C_I2CONCLR_I2ENC | I2C_I2CONCLR_SIC);
		//enable i2c operation
		I2Cx->I2CONSET = I2C_I2CONSET_I2EN;
	}

private:
	static bool attachDelegate(xpcc::I2cTransaction *d) {
		xpcc::atomic::Lock l;

		if(delegate == d)
			return false;

		if(d->attaching()) {
			DMB();
			if(!delegate) {
				//ERR << 'a';
				delegate = d;
				delegate->next = 0;
				newSession = true;
				i2start();
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

	static xpcc::I2c::Operation nextOperation;

	// buffer management
	static uint8_t *readPointer;
	static const uint8_t *writePointer;
	static std::size_t readBytesLeft;
	static std::size_t writeBytesLeft;
	static volatile bool newSession;

	// delegating
	static xpcc::I2cTransaction* volatile delegate;
	volatile static xpcc::I2cMaster::Error error;

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

	static ALWAYS_INLINE
	void i2stop()
	{
		/* Make sure start bit is not active */
//		if (I2Cx->I2CONSET & I2C_I2CONSET_STA)
//		{
//			I2Cx->I2CONCLR = I2C_I2CONCLR_STAC;
//		}
		I2Cx->I2CONSET = I2C_I2CONSET_STO | I2C_I2CONSET_AA;
		I2Cx->I2CONCLR = I2C_I2CONCLR_SIC;
	}

	static inline void i2write(uint8_t byte) {
		I2Cx->I2DAT = byte;
	}

	static inline uint8_t i2read() {
		return (I2Cx->I2DAT & I2C_I2DAT_BITMASK);
	}

	static inline void i2ack() {
		I2Cx->I2CONSET = I2C_I2CONSET_AA;
	}

	static inline void i2nack() {
		I2Cx->I2CONCLR = I2C_I2CONSET_AA;
	}

	static inline void intClear() {
		I2Cx->I2CONCLR = I2C_I2CONCLR_SIC;
	}

	static inline uint32_t intStatus() {
		return (I2Cx->I2STAT & I2C_STAT_CODE_BITMASK);
	}

	static ALWAYS_INLINE
	void intCmd(bool enable) {
		if (enable) {
			if (I2Cx == LPC_I2C0) {
				NVIC_EnableIRQ(I2C0_IRQn);
			} else if (I2Cx == LPC_I2C1) {
				NVIC_EnableIRQ(I2C1_IRQn);
			} else if (I2Cx == LPC_I2C2) {
				NVIC_EnableIRQ(I2C2_IRQn);
			}
		} else {
			if (I2Cx == LPC_I2C0) {
				NVIC_DisableIRQ(I2C0_IRQn);
			} else if (I2Cx == LPC_I2C1) {
				NVIC_DisableIRQ(I2C1_IRQn);
			} else if (I2Cx == LPC_I2C2) {
				NVIC_DisableIRQ(I2C2_IRQn);
			}
		}
	}

	static ALWAYS_INLINE
	void i2start()
	{
		DEBUG("callStarting");
		error = xpcc::I2cMaster::Error::NoError;

		I2Cx->I2CONCLR = I2C_I2CONCLR_SIC;
		I2Cx->I2CONSET = I2C_I2CONSET_STA;

	}
};

template <int P_I2C>
volatile bool I2cMaster<P_I2C>::newSession;

template <int P_I2C>
uint8_t * I2cMaster<P_I2C>::readPointer;

template <int P_I2C>
const uint8_t * I2cMaster<P_I2C>::writePointer;

template <int P_I2C>
std::size_t I2cMaster<P_I2C>::readBytesLeft;

template <int P_I2C>
std::size_t I2cMaster<P_I2C>::writeBytesLeft;

template <int P_I2C>
xpcc::I2c::Operation I2cMaster<P_I2C>::nextOperation;

template <int P_I2C>
volatile xpcc::I2cMaster::Error I2cMaster<P_I2C>::error(xpcc::I2cMaster::Error::NoError);

typedef I2cMaster<(int)LPC_I2C0> I2cMaster0;
typedef I2cMaster<(int)LPC_I2C1> I2cMaster1;
typedef I2cMaster<(int)LPC_I2C2> I2cMaster2;

}

} // namespace xpcc

#undef DEBUG
#undef DEBUG_STREAM
