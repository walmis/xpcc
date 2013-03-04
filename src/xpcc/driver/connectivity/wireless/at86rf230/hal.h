#pragma once
/* Copyright (c) 2013, Valmantas Palikša <walmis@balticum-tv.lt>
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


#include <stdint.h>
#include <stdbool.h>
#include <xpcc/architecture.hpp>

#include <xpcc/driver/connectivity/wireless/mac802.15.4/mac.hpp>

#include "defines.hpp"

#define BAT_LOW_MASK       ( 0x80 ) /**< Mask for the BAT_LOW interrupt. */
#define TRX_UR_MASK        ( 0x40 ) /**< Mask for the TRX_UR interrupt. */
#define TRX_END_MASK       ( 0x08 ) /**< Mask for the TRX_END interrupt. */
#define RX_START_MASK      ( 0x04 ) /**< Mask for the RX_START interrupt. */
#define PLL_UNLOCK_MASK    ( 0x02 ) /**< Mask for the PLL_UNLOCK interrupt. */
#define PLL_LOCK_MASK      ( 0x01 ) /**< Mask for the PLL_LOCK interrupt. */

#define MIN_FRAME_LENGTH   ( 0x03 ) /**< A frame should be at least 3 bytes. */
#define MAX_FRAME_LENGTH   ( 0x7F ) /**< A frame should no more than 127 bytes. */
/** \} */
/*============================ TYPDEFS =======================================*/

/**
 * \name Transaction status codes
 * \{
 */
namespace xpcc {
namespace rf230 {

template<typename Spi, typename rst, typename cs, typename slp_tr>
class Hal {

public:

	template<int address, int mask, int position, typename type = int>
	class Register {
	public:
		inline __attribute__((always_inline)) void set(type value) {
			subregisterWrite(address, mask, position, (int)value);
		}
		inline __attribute__((always_inline)) type read() {
			return (type)subregisterRead(address, mask, position);
		}
		inline __attribute__((always_inline)) void operator =(type rhs) {
			set(rhs);
		}
		inline __attribute__((always_inline)) operator type() {
			return (type)read();
		}

	};

	struct Reg {
		static Register<0x01, 0x80, 7> CCA_DONE;
		static Register<0x01, 0x40, 6> CCA_STATUS;
		static Register<0x01, 0x20, 5> reserved_01_3;
		static Register<0x01, 0x1f, 0, TRXState> TRX_STATUS;
		static Register<0x02, 0xe0, 5, TracStatus> TRAC_STATUS;
		static Register<0x02, 0x1f, 0, TRXCmd> TRX_CMD;
		static Register<0x03, 0xc0, 6> PAD_IO;
		static Register<0x03, 0x30, 4> PAD_IO_CLKM;
		static Register<0x03, 0x08, 3> CLKM_SHA_SEL;
		static Register<0x03, 0x07, 0> CLKM_CTRL;
		static Register<0x05, 0x80, 7> TX_AUTO_CRC_ON;
		static Register<0x06, 0x80, 7> RX_CRC_VALID;
		static Register<0x05, 0x70, 4> reserved_05_2;
		static Register<0x05, 0x0f, 0> TX_PWR;
		static Register<0x06, 0xe0, 5> reserved_06_1;
		static Register<0x06, 0x1f, 0> RSSI;
		static Register<0x07, 0xff, 0> ED_LEVEL;
		static Register<0x08, 0x80, 7> CCA_REQUEST;
		static Register<0x08, 0x60, 5> CCA_MODE;
		static Register<0x08, 0x1f, 0> CHANNEL;
		static Register<0x09, 0xf0, 4> CCA_CS_THRES;
		static Register<0x09, 0x0f, 0> CCA_ED_THRES;
		static Register<0x0e, 0xff, 0> IRQ_MASK;
		static Register<0x0f, 0x80, 7> IRQ_7_BAT_LOW;
		static Register<0x0f, 0x40, 6> IRQ_6_TRX_UR;
		static Register<0x0f, 0x20, 5> IRQ_5;
		static Register<0x0f, 0x10, 4> IRQ_4;
		static Register<0x0f, 0x08, 3> IRQ_3_TRX_END;
		static Register<0x0f, 0x04, 2> IRQ_2_RX_START;
		static Register<0x0f, 0x02, 1> IRQ_1_PLL_UNLOCK;
		static Register<0x0f, 0x01, 0> IRQ_0_PLL_LOCK;
		static Register<0x0f, 0xff, 0> IRQ_STATUS;
		static Register<0x10, 0x80, 7> AVREG_EXT;
		static Register<0x10, 0x40, 6> AVDD_OK;
		static Register<0x10, 0x30, 4> AVREG_TRIM;
#define AVREG_1_80V              (0)
#define AVREG_1_75V              (1)
#define AVREG_1_84V              (2)
#define AVREG_1_88V              (3)
		static Register<0x10, 0x08, 3> DVREG_EXT;
		static Register<0x10, 0x04, 2> DVDD_OK;
		static Register<0x10, 0x03, 0> DVREG_TRIM;
#define DVREG_1_80V              (0)
#define DVREG_1_75V              (1)
#define DVREG_1_84V              (2)
#define DVREG_1_88V              (3)

		static Register<0x11, 0xc0, 6> reserved_11_1;
		static Register<0x11, 0x20, 5> BATMON_OK;
		static Register<0x11, 0x10, 4> BATMON_HR;
		static Register<0x11, 0x0f, 0> BATMON_VTH;
		static Register<0x12, 0xf0, 4> XTAL_MODE;
		static Register<0x12, 0x0f, 0> XTAL_TRIM;
		static Register<0x18, 0x80, 7> FTN_START;
		static Register<0x18, 0x40, 6> reserved_18_2;
		static Register<0x18, 0x3f, 0> FTNV;
		static Register<0x1a, 0x80, 7> PLL_CF_START;
		static Register<0x1a, 0x70, 4> reserved_1a_2;
		static Register<0x1a, 0x0f, 0> PLL_CF;
		static Register<0x1b, 0x80, 7> PLL_DCU_START;
		static Register<0x1b, 0x40, 6> reserved_1b_2;
		static Register<0x1b, 0x3f, 0> PLL_DCUW;
		static Register<0x1c, 0xff, 0> PART_NUM;
#define RF230                    (2)
		static Register<0x1d, 0xff, 0> VERSION_NUM;
		static Register<0x1e, 0xff, 0> MAN_ID_0;
		static Register<0x1f, 0xff, 0> MAN_ID_1;
		static Register<0x20, 0xff, 0> SHORT_ADDR_0;
		static Register<0x21, 0xff, 0> SHORT_ADDR_1;
		static Register<0x22, 0xff, 0> PAN_ID_0;
		static Register<0x23, 0xff, 0> PAN_ID_1;
		static Register<0x24, 0xff, 0> IEEE_ADDR_0;
		static Register<0x25, 0xff, 0> IEEE_ADDR_1;
		static Register<0x26, 0xff, 0> IEEE_ADDR_2;
		static Register<0x27, 0xff, 0> IEEE_ADDR_3;
		static Register<0x28, 0xff, 0> IEEE_ADDR_4;
		static Register<0x29, 0xff, 0> IEEE_ADDR_5;
		static Register<0x2a, 0xff, 0> IEEE_ADDR_6;
		static Register<0x2b, 0xff, 0> IEEE_ADDR_7;
		static Register<0x2c, 0xf0, 4> MAX_FRAME_RETRIES;
		static Register<0x2c, 0x0e, 1> MAX_CSMA_RETRIES;
		static Register<0x2c, 0x01, 0> reserved_2c_3;
		static Register<0x2d, 0xff, 0> CSMA_SEED_0;
		static Register<0x2e, 0xc0, 6> MIN_BE;
		static Register<0x2e, 0x20, 5> AACK_SET_PD;
		static Register<0x2e, 0x30, 4> reserved_2e_2;
		static Register<0x2e, 0x08, 3> I_AM_COORD;
		static Register<0x2e, 0x07, 0> CSMA_SEED_1;
	};



	static void init(void);

	static uint8_t registerRead(uint8_t address);
	static void registerWrite(uint8_t address, uint8_t value);

	static bool isSleeping();

	/*----------------------------------------------------------------------------*/
	/** \brief  This function reads the value of a specific subregister.
	 *
	 *  \see Look at the at86rf230_registermap.h file for register and subregister
	 *       definitions.
	 *
	 *  \param  address  Main register's address.
	 *  \param  mask  Bit mask of the subregister.
	 *  \param  position   Bit position of the subregister
	 *  \retval Value of the read subregister.
	 */
	static uint8_t subregisterRead(uint8_t address, uint8_t mask,
			uint8_t position);

	static void reset();


	/*----------------------------------------------------------------------------*/
	/** \brief  This function writes a new value to one of the radio transceiver's
	 *          subregisters.
	 *
	 *  \see Look at the at86rf230_registermap.h file for register and subregister
	 *       definitions.
	 *
	 *  \param  address  Main register's address.
	 *  \param  mask  Bit mask of the subregister.
	 *  \param  position  Bit position of the subregister
	 *  \param  value  Value to write into the subregister.
	 */
	static void subregisterWrite(uint8_t address, uint8_t mask,
			uint8_t position, uint8_t value);

	/*----------------------------------------------------------------------------*/
	/** \brief  This function will upload a frame from the radio transceiver's frame
	 *          buffer.
	 *
	 *          If the frame currently available in the radio transceiver's frame buffer
	 *          is out of the defined bounds. Then the frame length, lqi value and crc
	 *          be set to zero. This is done to indicate an error.
	 */
	static uint8_t frameLength();

	static uint8_t frameRead(Frame& frame);
	/*----------------------------------------------------------------------------*/
	/** \brief  This function will download a frame to the radio transceiver's frame
	 *          buffer.
	 *
	 *  \param  data        Pointer to data that is to be written to frame buffer.
	 *  \param  len         Length of data. The maximum length is 127 bytes.
	 */
	static void frameWrite(uint8_t* data, uint8_t len);
	/*----------------------------------------------------------------------------*/
	/** \brief Read SRAM
	 *
	 * This function reads from the SRAM of the radio transceiver.
	 *
	 * \param address Address in the TRX's SRAM where the read burst should start
	 * \param length Length of the read burst
	 * \param data Pointer to buffer where data is stored.
	 */
	static void sramRead(uint8_t address, uint8_t length, uint8_t *data);

	/*----------------------------------------------------------------------------*/
	/** \brief Write SRAM
	 *
	 * This function writes into the SRAM of the radio transceiver.
	 *
	 * \param address Address in the TRX's SRAM where the write burst should start
	 * \param length  Length of the write burst
	 * \param data    Pointer to an array of bytes that should be written
	 */
	static void sramWrite(uint8_t address, uint8_t length, uint8_t* data);

	static TracStatus getTracStatus();

	static TRXState getTrxState();

	static RadioStatus setTrxState(TRXState state);

};

//#include "hal_impl.hpp"

/*
 * hal_impl.hpp
 *
 *  Created on: Feb 2, 2013
 *      Author: walmis
 */

template<typename Spi, typename rst, typename cs, typename slp_tr>
inline void Hal<Spi, rst, cs, slp_tr>::sramRead(uint8_t address,
		uint8_t length, uint8_t* data) {
	uint8_t dummy;

	cs::set(0); /* Initiate the SPI transaction. */

	/*Send SRAM read command.*/
	dummy = Spi::write(TRX_CMD_SR);

	/*Send address where to start reading.*/
	dummy = Spi::write(address);

	/*Upload the chosen memory area.*/
	do {
		*data++ = Spi::write(0);
	} while (--length > 0);

	cs::set(1);
}

/**< Mask for the PLL_LOCK interrupt. */
template<typename Spi, typename rst, typename cs, typename slp_tr>
inline void Hal<Spi, rst, cs, slp_tr>::frameWrite(uint8_t* data, uint8_t len) {
	len &= TRX_CMD_RADDRM;
	xpcc::atomic::Lock lock;
	cs::set(0); /* Initiate the SPI transaction. */
	/*SEND FRAME WRITE COMMAND AND FRAME LENGTH.*/
	Spi::write(TRX_CMD_FW);
	Spi::write(len);
	/* Download to the Frame Buffer. */
	do {
		Spi::write(*data++);
		--len;
	} while (len > 0);
	cs::set(1); /* Terminate SPI transaction. */
}

/**< Mask for the PLL_LOCK interrupt. */
template<typename Spi, typename rst, typename cs, typename slp_tr>
inline void Hal<Spi, rst, cs, slp_tr>::sramWrite(uint8_t address,
		uint8_t length, uint8_t* data) {
	uint8_t dummy;
	xpcc::atomic::Lock lock;
	cs::set(0);
	/*Send SRAM write command.*/
	Spi::write(TRX_CMD_SW);
	/*Send address where to start writing to.*/
	Spi::write(address);
	/*Upload the chosen memory area.*/
	do {
		Spi::write(*data++);
	} while (--length > 0);
	cs::set(1);
}

/**< Mask for the PLL_LOCK interrupt. */
template<typename Spi, typename rst, typename cs, typename slp_tr>
inline TracStatus Hal<Spi, rst, cs, slp_tr>::getTracStatus() {
	return Reg::TRAC_STATUS;
}

/**< Mask for the PLL_LOCK interrupt. */
template<typename Spi, typename rst, typename cs, typename slp_tr>
inline TRXState Hal<Spi, rst, cs, slp_tr>::getTrxState() {
	return Reg::TRX_STATUS;
}

/**< Mask for the PLL_LOCK interrupt. */
template<typename Spi, typename rst, typename cs, typename slp_tr>
uint8_t Hal<Spi, rst, cs, slp_tr>::frameRead(Frame& frame) {
	uint8_t i, len;
	//XPCC_LOG_DEBUG .printf("Hal::read Frame %x\n", &frame);
	xpcc::atomic::Lock lock;
	cs::set(0);
	/*Send frame read command and read the length.*/
	Spi::write(TRX_CMD_FR);
	len = Spi::write(0);

	frame.rx_flag = true;
	uint8_t* frm_data = frame.data;

	uint8_t read = 0;

	// check the length of the frame to make sure its within the correct size limits
	if ((len >= MIN_FRAME_LENGTH)&& (len <= MAX_FRAME_LENGTH) && frame.buffer_size >= len) {

		for (i=0; i < len; i++)
		{
			*frm_data++ = Spi::write(0);
			read++;
		}

		frame.data_len = len;
		//LQI byte
		frame.lqi = Spi::write(0);
		//fifo_put(&frame_buf, data);

	}
	cs::set(1);
	return read;
}

/**< Mask for the PLL_LOCK interrupt. */
template<typename Spi, typename rst, typename cs, typename slp_tr>
inline void Hal<Spi, rst, cs, slp_tr>::subregisterWrite(uint8_t address,
		uint8_t mask, uint8_t position, uint8_t value) {
	/* Read current register value and mask area outside the subregister. */
	uint8_t register_value = registerRead(address);
	register_value &= ~mask;
	/* Start preparing the new subregister value. shift in place and mask. */
	value <<= position;
	value &= mask;
	value |= register_value; /* Set the new subregister value. */
	/* Write the modified register value. */
	registerWrite(address, value);
}

/**< Mask for the PLL_LOCK interrupt. */
template<typename Spi, typename rst, typename cs, typename slp_tr>
inline void Hal<Spi, rst, cs, slp_tr>::reset() {
	rst::set(0);
	slp_tr::set(0);
	xpcc::delay_us(Timings::TIME_RESET);
	rst::set(1);
}

/**< Mask for the PLL_LOCK interrupt. */
template<typename Spi, typename rst, typename cs, typename slp_tr>
inline uint8_t Hal<Spi, rst, cs, slp_tr>::subregisterRead(uint8_t address,
		uint8_t mask, uint8_t position) {
	/* Read current register value and mask out subregister. */
	uint8_t register_value = registerRead(address);
	register_value &= mask;
	register_value >>= position; /* Align subregister value. */
	return register_value;
}

/**< Mask for the PLL_LOCK interrupt. */
template<typename Spi, typename rst, typename cs, typename slp_tr>
inline void Hal<Spi, rst, cs, slp_tr>::registerWrite(uint8_t address,
		uint8_t value) {
	xpcc::atomic::Lock lock;
	/* Add the Register Write command to the address. */
	address = TRX_CMD_RW | (TRX_CMD_RADDRM & address);
	cs::set(0); /* Start the SPI transaction by pulling the Slave Select low. */
	/*Send Register address and write register content.*/
	Spi::write(address);
	Spi::write(value);
	cs::set(1); /* End the transaction by pulling the Slave Slect High. */
}

/**< Mask for the PLL_LOCK interrupt. */
template<typename Spi, typename rst, typename cs, typename slp_tr>
inline uint8_t Hal<Spi, rst, cs, slp_tr>::registerRead(uint8_t address) {
	uint8_t reg_val = 0;
	xpcc::atomic::Lock lock;
	/* Add the register read command to the register address. */
	address &= TRX_CMD_RADDRM;
	address |= TRX_CMD_RR;
	cs::set(0);
	/*Send Register address and read register content.*/
	Spi::write(address);
	reg_val = Spi::write(0x55);
	cs::set(1); /* End the transaction by pulling the Slave Select High. */
	return reg_val;
}

/**< Mask for the PLL_LOCK interrupt. */
template<typename Spi, typename rst, typename cs, typename slp_tr>
inline void Hal<Spi, rst, cs, slp_tr>::init(void) {
	rst::setOutput(0);
	cs::setOutput(1);
	slp_tr::setOutput(0);
}



template<typename Spi, typename rst, typename cs, typename slp_tr>
RadioStatus rf230::Hal<Spi, rst, cs, slp_tr>::setTrxState(TRXState state) {
	int delay;
	TRXState curr_state;
	// if we're sleeping then don't allow transition
	if (slp_tr::read()) {
		return RadioStatus::WRONG_STATE;
	}

	// if we're in a transition state, wait for the state to become stable
	curr_state = getTrxState();
	if ((curr_state == TRXState::BUSY_TX_ARET)
			|| (curr_state == TRXState::BUSY_RX_AACK)
			|| (curr_state == TRXState::BUSY_RX)
			|| (curr_state == TRXState::BUSY_TX)) {
		while (getTrxState() == curr_state);
	}

	// At this point it is clear that the requested new_state is:
	// TRX_OFF, RX_ON, PLL_ON, RX_AACK_ON or TX_ARET_ON.
	// we need to handle some special cases before we transition to the new state
	switch (state) {
	case TRXState::TRX_OFF:
		/* Go to TRX_OFF from any state. */
		slp_tr::set(0);

		xpcc::delay_us(Timings::TIME_NOCLK_TO_WAKE);

		Reg::TRX_CMD = TRXCmd::CMD_FORCE_TRX_OFF;

		xpcc::delay_us(Timings::TIME_CMD_FORCE_TRX_OFF);
		break;

	case TRXState::TX_ARET_ON:
		if (curr_state == TRXState::RX_AACK_ON) {
			/* First do intermediate state transition to PLL_ON, then to TX_ARET_ON. */
			Reg::TRX_CMD = TRXCmd::CMD_PLL_ON;

			xpcc::delay_us(Timings::TIME_STATE_TRANSITION_PLL_ACTIVE);
		}
		break;

	case TRXState::RX_AACK_ON:
		if (curr_state == TRXState::TX_ARET_ON) {
			/* First do intermediate state transition to RX_ON, then to RX_AACK_ON. */
			Reg::TRX_CMD = TRXCmd::CMD_PLL_ON;

			xpcc::delay_us(Timings::TIME_STATE_TRANSITION_PLL_ACTIVE);
		}
		break;

	default:
		break;
	}

	/* Now we're okay to transition to any new state. */
	Reg::TRX_CMD = (TRXCmd)state;

	/* When the PLL is active most states can be reached in 1us. However, from */
	/* TRX_OFF the PLL needs time to activate. */
	delay = (curr_state == TRXState::P_ON) ? Timings::TIME_P_ON_TO_TRX_OFF :
				(curr_state == TRXState::TRX_OFF) ?
							Timings::TIME_TRX_OFF_TO_PLL_ACTIVE :
							Timings::TIME_STATE_TRANSITION_PLL_ACTIVE;

	xpcc::delay_us(delay);

	if (getTrxState() == state) {
		return RadioStatus::SUCCESS;
	}
	XPCC_LOG_DEBUG << "State transition to " << (int)state << " failed\n";
	return RadioStatus::TIMED_OUT;
}

/**< Mask for the PLL_LOCK interrupt. */
template<typename Spi, typename rst, typename cs, typename slp_tr>
inline bool Hal<Spi, rst, cs, slp_tr>::isSleeping() {
	if (slp_tr::read()) {
		return true;
	}
	return false;
}

}

template<typename Spi, typename rst, typename cs, typename slp_tr>
inline uint8_t rf230::Hal<Spi, rst, cs, slp_tr>::frameLength() {
	uint8_t len;
	xpcc::atomic::Lock lock;
	cs::set(0);
	/*Send frame read command and read the length.*/
	Spi::write(TRX_CMD_FR);
	len = Spi::write(0);

	cs::set(1);
	return len;
}

}
