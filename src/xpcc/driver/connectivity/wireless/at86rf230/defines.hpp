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

#ifndef DEFINES_HPP_
#define DEFINES_HPP_


#define DUMMY_READ         (0x00) /**<  Dummy value for the SPI. */

#define TRX_CMD_RW         (0xC0) /**<  Register Write (short mode). */
#define TRX_CMD_RR         (0x80) /**<  Register Read (short mode). */
#define TRX_CMD_FW         (0x60) /**<  Frame Transmit Mode (long mode). */
#define TRX_CMD_FR         (0x20) /**<  Frame Receive Mode (long mode). */
#define TRX_CMD_SW         (0x40) /**<  SRAM Write. */
#define TRX_CMD_SR         (0x00) /**<  SRAM Read. */
#define TRX_CMD_RADDRM     (0x7F) /**<  Register Address Mask. */

namespace xpcc {
namespace rf230 {

enum Ranges {
	RF230_MIN_CHANNEL = 11,      ///< Min channel val for this radio
	RF230_MAX_CHANNEL = 26,
};

enum class TracStatus
	: uint8_t {
	SUCCESS = 0,
	SUCCESS_DATA_PENDING = 1,
	SUCCESS_WAIT_FOR_ACK = 2,
	CHANNEL_ACCESS_FAILURE = 3,
	NO_ACK = 5,
	INVALID = 7
};


enum Timings
	: uint32_t {
		TIME_TO_ENTER_P_ON = 510, /**<  Transition time from VCC is applied to P_ON. */
		TIME_P_ON_TO_TRX_OFF = 880, /**<  Transition time from P_ON to TRX_OFF. */
		TIME_SLEEP_TO_TRX_OFF = 880, /**<  Transition time from SLEEP to TRX_OFF. */
		TIME_RESET = 6, /**<  Time to hold the RST pin low during reset */
		TIME_ED_MEASUREMENT = 140, /**<  Time it takes to do a ED measurement. */
		TIME_CCA = 140, /**<  Time it takes to do a CCA. */
		TIME_PLL_LOCK = 150, /**<  Maximum time it should take for the PLL to lock. */
		TIME_FTN_TUNING = 25, /**<  Maximum time it should take to do the filter tuning. */
		TIME_NOCLK_TO_WAKE = 6, /**<  Transition time from *_NOCLK to being awake. */
		TIME_CMD_FORCE_TRX_OFF = 1, /**<  Time it takes to execute the FORCE_TRX_OFF command. */
		TIME_TRX_OFF_TO_PLL_ACTIVE = 180, /**<  Transition time from TRX_OFF to: RX_ON, PLL_ON, TX_ARET_ON and RX_AACK_ON. */
		TIME_STATE_TRANSITION_PLL_ACTIVE = 1, /**<  Transition time from PLL active state to another. */
};

enum class TRXState
	: uint8_t {
		P_ON = 0,
		/** Constant BUSY_RX for sub-register @ref SR_TRX_STATUS */
		BUSY_RX = 1,
		/** Constant BUSY_TX for sub-register @ref SR_TRX_STATUS */
		BUSY_TX = 2,
		/** Constant RX_ON for sub-register @ref SR_TRX_STATUS */
		RX_ON = 6,
		/** Constant TRX_OFF for sub-register @ref SR_TRX_STATUS */
		TRX_OFF = 8,
		/** Constant PLL_ON for sub-register @ref SR_TRX_STATUS */
		PLL_ON = 9,
		/** Constant SLEEP for sub-register @ref SR_TRX_STATUS */
		SLEEP = 15,
		/** Constant BUSY_RX_AACK for sub-register @ref SR_TRX_STATUS */
		BUSY_RX_AACK = 17,
		/** Constant BUSY_TX_ARET for sub-register @ref SR_TRX_STATUS */
		BUSY_TX_ARET = 18,
		/** Constant RX_AACK_ON for sub-register @ref SR_TRX_STATUS */
		RX_AACK_ON = 22,
		/** Constant TX_ARET_ON for sub-register @ref SR_TRX_STATUS */
		TX_ARET_ON = 25,
		/** Constant RX_ON_NOCLK for sub-register @ref SR_TRX_STATUS */
		RX_ON_NOCLK = 28,
		/** Constant RX_AACK_ON_NOCLK for sub-register @ref SR_TRX_STATUS */
		RX_AACK_ON_NOCLK = 29,
		/** Constant BUSY_RX_AACK_NOCLK for sub-register @ref SR_TRX_STATUS */
		BUSY_RX_AACK_NOCLK = 30,
};

enum TRXCmd
	: uint8_t {
		CMD_NOP = 0,
		/** Constant CMD_TX_START for sub-register @ref TRX_CMD */
		CMD_TX_START = 2,
		/** Constant CMD_FORCE_TRX_OFF for sub-register @ref TRX_CMD */
		CMD_FORCE_TRX_OFF = 3,
		/** Constant CMD_RX_ON for sub-register @ref TRX_CMD */
		CMD_RX_ON = 6,
		/** Constant CMD_TRX_OFF for sub-register @ref TRX_CMD */
		CMD_TRX_OFF = 8,
		/** Constant CMD_PLL_ON for sub-register @ref TRX_CMD */
		CMD_PLL_ON = 9,
		/** Constant CMD_RX_AACK_ON for sub-register @ref TRX_CMD */
		CMD_RX_AACK_ON = 22,
		/** Constant CMD_TX_ARET_ON for sub-register @ref TRX_CMD */
		CMD_TX_ARET_ON = 25,
};

enum CLKMClkCmd
	: uint8_t {
		no_clock = 0,
		/** Constant CLKM_1MHz for sub-register @ref CLKM_CTRL */
		CLKM_1MHz = 1,
		/** Constant CLKM_2MHz for sub-register @ref CLKM_CTRL */
		CLKM_2MHz = 2,
		/** Constant CLKM_4MHz for sub-register @ref CLKM_CTRL */
		CLKM_4MHz = 3,
		/** Constant CLKM_8MHz for sub-register @ref CLKM_CTRL */
		CLKM_8MHz = 4,
		/** Constant CLKM_16MHz for sub-register @ref CLKM_CTRL */
		CLKM_16MHz = 5,
};

enum CLKMDrvCmd
	: uint8_t {
		CLKM_2mA = 0,
		/** Constant CLKM_4mA for sub-register @ref PAD_IO_CLKM */
		CLKM_4mA = 1,
		/** Constant CLKM_6mA for sub-register @ref PAD_IO_CLKM */
		CLKM_6mA = 2,
		/** Constant CLKM_8mA for sub-register @ref PAD_IO_CLKM */
		CLKM_8mA = 3,
};

enum IRQStatus
	: uint8_t {
		IRQ_BAT_LOW_MASK = 0x80, /**< Mask for the BAT_LOW interrupt. */
		IRQ_TRX_UR_MASK = 0x40, /**< Mask for the TRX_UR interrupt. */
		IRQ_TRX_END_MASK = 0x08, /**< Mask for the TRX_END interrupt. */
		IRQ_RX_START_MASK = 0x04, /**< Mask for the RX_START interrupt. */
		IRQ_PLL_UNLOCK_MASK = 0x02, /**< Mask for the PLL_UNLOCK interrupt. */
		IRQ_PLL_LOCK_MASK = 0x01 /**< Mask for the PLL_LOCK interrupt. */
};

}

}
#endif /* DEFINES_HPP_ */
