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

#include "hal.h"
#include <xpcc/debug.hpp>
#include <xpcc/workflow.hpp>

namespace xpcc {
namespace rf230 {

typedef void (*FrameHandler)();

struct Stats {
	uint32_t tx_bytes;
	uint32_t rx_bytes;
	uint32_t tx_frames;
	uint32_t rx_frames;
};

template<typename Spi, typename rst, typename cs, typename slp_tr>
class Driver {
typedef Hal<Spi, rst, cs, slp_tr> HAL;
public:
	void init();

	RadioStatus setChannel(uint8_t channel);
	uint8_t getChannel();

	void setShortAddress(uint16_t address);
	uint16_t getShortAddress();

	void setPanId(uint16_t panId) {
		HAL::Reg::PAN_ID_0 = panId;
		HAL::Reg::PAN_ID_1 = panId >> 8;
	}

	uint16_t getPanId() {
		return HAL::Reg::PAN_ID_0 | (HAL::Reg::PAN_ID_1 << 8);
	}

	void setRxFrameHandler(FrameHandler func) {
		frmHandler = func;
	}

	void rxOn() {
		HAL::setTrxState(TRXState::RX_AACK_ON);
	}

	void rxOff() {
		HAL::setTrxState(TRXState::TRX_OFF);
	}

	void IRQHandler()
	{
		uint8_t intp_src;
		TRXState state;

		intp_src = HAL::Reg::IRQ_STATUS;
		//XPCC_LOG_DEBUG .printf("Interrupt Status: %02x\n", intp_src);


		/*Handle the incoming interrupt. Prioritized.*/
		if ((intp_src & IRQStatus::IRQ_RX_START_MASK))
		{
			//intp_src &= ~IRQStatus::IRQ_RX_START_MASK;
		}
		if (intp_src & IRQStatus::IRQ_TRX_END_MASK)
		{
			state = HAL::getTrxState();

			if ((state == TRXState::RX_ON) || (state == TRXState::RX_AACK_ON) || (state == TRXState::BUSY_RX_AACK))
			{
				// get the ed measurement
				//pcb->ed = chb_reg_read(PHY_ED_LEVEL);
				// if the crc is not valid, then do not read the frame and set the rx flag
				if (HAL::Reg::RX_CRC_VALID)
				{
					rx_pending = HAL::frameLength();
					//XPCC_LOG_DEBUG << "RX" << xpcc::endl;
					if(frmHandler)
						frmHandler();

					// get the data
					//chb_frame_read();
					//pcb->rcvd_xfers++;
					//pcb->data_rcv = true;
				}
			}
			else
			{
				//pcb->tx_end = true;
				tx_busy = false;
			}
			//intp_src &= ~IRQStatus::IRQ_TRX_END_MASK;

			HAL::setTrxState(TRXState::RX_AACK_ON);
		}
		if (intp_src & IRQStatus::IRQ_TRX_UR_MASK)
		{
			//intp_src &= ~IRQStatus::IRQ_TRX_UR_MASK;
			//pcb->underrun++;
		}
		if (intp_src & IRQStatus::IRQ_PLL_UNLOCK_MASK)
		{
			//intp_src &= ~IRQStatus::IRQ_PLL_UNLOCK_MASK;
		}
		if (intp_src & IRQStatus::IRQ_PLL_LOCK_MASK)
		{
			//intp_src &= ~IRQStatus::IRQ_PLL_LOCK_MASK;
		}
		if (intp_src & IRQStatus::IRQ_BAT_LOW_MASK)
		{
			//intp_src &= ~IRQStatus::IRQ_BAT_LOW_MASK;
			//pcb->battlow++;
		}
		else
		{
		}
	}

	RadioStatus getTxStatus();

	RadioStatus checkTxStatus(bool block = false) {
		xpcc::Timeout<> t(200);

		if(block) {
			while(tx_busy) {
				if(t.isExpired()) {
					return RadioStatus::TIMED_OUT;
				}
			}
			return getTxStatus();
		} else {
			if(tx_busy) {
				return RadioStatus::TRX_BUSY;
			} else {
				return getTxStatus();
			}
		}
	}

	TRXState getTRXState() {
		return HAL::getTrxState();
	}

	Stats* getStats() {
		return &stats;
	}

	//transmit a frame that is already uploaded to the frame buffer
	RadioStatus sendFrame(bool blocking = false) {
		TRXState state = HAL::getTrxState();

		if ((state == TRXState::BUSY_TX) || (state == TRXState::BUSY_TX_ARET))
		{
			return RadioStatus::TRX_BUSY;
		}

		if(state == TRXState::TX_ARET_ON ||
				HAL::setTrxState(TRXState::TX_ARET_ON) == RadioStatus::SUCCESS) {

			HAL::Reg::TRX_CMD = TRXCmd::CMD_TX_START;
			stats.tx_frames++;
			tx_busy = true;

			if(blocking)
				return checkTxStatus(true);
			else
				return RadioStatus::SUCCESS;

		} else {
			return RadioStatus::TIMED_OUT;
		}

	}

	//upload a frame and transfer it, if blocking is true block until frame
	//is transferred and return status
	RadioStatus sendFrame(const Frame &frame, bool blocking = false) {

		RadioStatus status;
		//initiate frame transmission
		status = sendFrame(false);

		if(status == RadioStatus::SUCCESS) {
			//write frame data while radio transceiver sends the sync data etc.

			HAL::frameWrite(frame.data, frame.data_len);
			stats.tx_bytes += frame.data_len;

			//uint8_t buf[60];
			//HAL::sramRead(0, 60, buf);
			//XPCC_LOG_DEBUG .printf("dump buffer -00000\n");
			//XPCC_LOG_DEBUG .dump_buffer(buf, frame.data_len);
			//XPCC_LOG_DEBUG .printf("status %d", (int)getTRXState());

			if(blocking)
				return checkTxStatus(true);
			else
				return RadioStatus::SUCCESS;
		}
		return status;
	}

	uint8_t getFrameLength() {
		return rx_pending;
	}

	bool readFrame(Frame& frame) {
		if(rx_pending) {
			uint8_t len = rx_pending;

			stats.rx_frames++;
			stats.rx_bytes += len;

			if(frame.buffer_size < len) {
				return false;
			}
			if(HAL::frameRead(frame) == len) {
				rx_pending = 0;
				return true;
			}
		}
		return false;
	}


protected:
	static Hal<Spi, rst, cs, slp_tr> hal;

private:

	FrameHandler frmHandler;

	volatile bool tx_busy;
	volatile uint8_t rx_pending;

	Stats stats;


};

template<typename Spi, typename rst, typename cs, typename slp_tr>
void Driver<Spi, rst, cs, slp_tr>::init() {
	frmHandler = 0;

	XPCC_LOG_DEBUG << "Initializing AT86RF230 Driver\n";

	hal.init();
	hal.reset();

	delay_us(500);

	uint8_t partnum = (int)HAL::Reg::PART_NUM;

	XPCC_LOG_DEBUG << "PART_NUM: " << partnum << xpcc::endl;
	XPCC_LOG_DEBUG << "VERSION_NUM: " << ((int)HAL::Reg::VERSION_NUM) << xpcc::endl;
	XPCC_LOG_DEBUG << "MAN_ID_0: " << ((int)HAL::Reg::MAN_ID_0) << xpcc::endl;

	if(partnum != 2) {
		XPCC_LOG_DEBUG << "AT86RF230 Initialization Failed" << xpcc::endl;
	}

	HAL::setTrxState(TRXState::TRX_OFF);

	HAL::Reg::TX_AUTO_CRC_ON = 1;

	HAL::Reg::CSMA_SEED_0 = xpcc::Random::random();
	HAL::Reg::CSMA_SEED_1 = xpcc::Random::random();

	HAL::Reg::IRQ_MASK = IRQStatus::IRQ_TRX_END_MASK;

}


template<typename Spi, typename rst, typename cs, typename slp_tr>
RadioStatus rf230::Driver<Spi, rst, cs, slp_tr>::setChannel(
	uint8_t channel)	{
    /*Do function parameter and state check.*/
    if ((channel < RF230_MIN_CHANNEL) ||
        (channel > RF230_MAX_CHANNEL)){
        return RadioStatus::INVALID_ARGUMENT;
    }

    if (HAL::is_sleeping()){
        return RadioStatus::WRONG_STATE;
    }

    if (getChannel() == channel){
        return RadioStatus::SUCCESS;
    }

    /*Set new operating channel.*/
    HAL::Reg::CHANNEL = channel;

    /* Read current state and wait for the PLL_LOCK interrupt if the */
    /* radio transceiver is in either RX_ON or PLL_ON. */
    TRXState trx_state = HAL::get_state();

    if ((trx_state == TRXState::RX_ON) ||
        (trx_state == TRXState::PLL_ON)){
        xpcc::delay_us(Timings::TIME_PLL_LOCK);
    }

    RadioStatus channel_set_status = RadioStatus::TIMED_OUT;

    /* Check that the channel was set properly. */
    if (getChannel() == channel){
        channel_set_status = RadioStatus::SUCCESS;
    }

    return channel_set_status;
}

template<typename Spi, typename rst, typename cs, typename slp_tr>
inline uint8_t Driver<Spi, rst, cs, slp_tr>::getChannel() {

	return HAL::Reg::CHANNEL;

}

template<typename Spi, typename rst, typename cs, typename slp_tr>
inline uint16_t Driver<Spi, rst, cs, slp_tr>::getShortAddress() {

	uint16_t short_address = ((uint16_t) (HAL::Reg::SHORT_ADDR_1 << 8)) | HAL::Reg::SHORT_ADDR_0;
	return short_address;

}

template<typename Spi, typename rst, typename cs, typename slp_tr>
inline void Driver<Spi, rst, cs, slp_tr>::setShortAddress(
		uint16_t new_short_address) {

	HAL::Reg::SHORT_ADDR_0 = new_short_address & 0xFF;
	HAL::Reg::SHORT_ADDR_1 = new_short_address >> 8;

}

template<typename Spi, typename rst, typename cs, typename slp_tr>
inline RadioStatus Driver<Spi, rst, cs, slp_tr>::getTxStatus() {
	TracStatus status = HAL::getTracStatus();
	if(status == TracStatus::NO_ACK) {
		return RadioStatus::NO_ACK;
	}
	if (status == TracStatus::CHANNEL_ACCESS_FAILURE) {
		return RadioStatus::CHANNEL_ACCESS_FAILURE;
	}
	if (status == TracStatus::INVALID) {
		return RadioStatus::INVALID_ARGUMENT;
	}
	if (status == TracStatus::SUCCESS ||
			status == TracStatus::SUCCESS_DATA_PENDING ||
			status == TracStatus::SUCCESS_WAIT_FOR_ACK) {
        return RadioStatus::SUCCESS;
    }
	return RadioStatus::INVALID_ARGUMENT;
}

}
}
