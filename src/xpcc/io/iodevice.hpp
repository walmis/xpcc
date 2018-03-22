// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
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
// ----------------------------------------------------------------------------

#ifndef XPCC__IODEVICE_HPP
#define XPCC__IODEVICE_HPP
#include <stdint.h>
#include <stddef.h>

#define NULLCH (-1)

namespace xpcc
{
	/**
	 * \brief 	Abstract calls of io-devices
	 *
	 * \ingroup io
	 * \author	Martin Rosekeit <martin.rosekeit@rwth-aachen.de>
	 */
	class IODevice
	{
	public :
		IODevice() : _blocking(true) {}
		
		//write blocking

		///	Write a single character
		virtual size_t write(char c) = 0;
		/// Write a C-string
		virtual size_t write(const char* str);

		virtual size_t write(const uint8_t* buf, size_t len);
		/// Read a single character
		virtual int16_t read() = 0;

		virtual void flush() = 0;

		virtual int16_t rxAvailable() {
			return -1; //unknown
		}
		virtual int16_t txAvailable() {
			return -1;
		}

		inline bool isBlocking() { return _blocking; }
		inline void setBlocking(bool b) { _blocking = b; }
	protected:
		bool _blocking;
	private :

		IODevice(const IODevice&);
	};

	class NullIODevice : public IODevice {
		size_t write(char c) override {
			return 1;
		}
		void flush() override {}
		int16_t read() override {
			return -1;
		}

	};
}

#endif // XPCC__IODEVICE_HPP
