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

#pragma once

#include "MacFrame.h"
#include "aes/AESCCM32.h"
#include "Frame.h"
#include "SecureFrame.h"

enum class RadioStatus
	: uint8_t {
		SUCCESS = 0x40, /**< The requested service was performed successfully. */
		UNSUPPORTED_DEVICE, /**< The connected device is not an Atmel AT86RF230. */
		INVALID_ARGUMENT, /**< One or more of the supplied function arguments are invalid. */
		TIMED_OUT, /**< The requested service timed out. */
		WRONG_STATE, /**< The end-user tried to do an invalid state transition. */
		BUSY_STATE, /**< The radio transceiver is busy receiving or transmitting. */
		STATE_TRANSITION_FAILED, /**< The requested state transition could not be completed. */
		CCA_IDLE, /**< Channel is clear, available to transmit a new frame. */
		CCA_BUSY, /**< Channel busy. */
		TRX_BUSY, /**< Transceiver is busy receiving or transmitting data. */
		BAT_LOW, /**< Measured battery voltage is lower than voltage threshold. */
		BAT_OK, /**< Measured battery voltage is above the voltage threshold. */
		CRC_FAILED, /**< The CRC failed for the actual frame. */
		CHANNEL_ACCESS_FAILURE, /**< The channel access failed during the auto mode. */
		NO_ACK /**< No acknowledge frame was received. */
};
