// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
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
#ifndef XPCC_LOG_STYLE_WRAPPER__HPP
#define XPCC_LOG_STYLE_WRAPPER__HPP

#include <xpcc/io/iodevice.hpp>
#include "style.hpp"
#include <stdint.h>

namespace xpcc
{
	namespace log
	{
		/**
		 * \brief 	Wrapper to use the Style as a IODevice in the Logger
		 *
		 * \ingroup logger
		 * \author	Martin Rosekeit <martin.rosekeit@rwth-aachen.de>
		 */
		template < typename STYLE >
		class StyleWrapper : public IODevice
		{
		public :
			StyleWrapper( STYLE inStyle );

			virtual
			~StyleWrapper();

			virtual size_t
			write(char c);

			virtual size_t
			write(const char* str);

			virtual void
			flush();

			virtual int16_t
			read();

		private :
			StyleWrapper( const StyleWrapper& );

			StyleWrapper&
			operator=( const StyleWrapper& );

			STYLE style;
		};
	}
}

// -----------------------------------------------------------------------------

template < typename STYLE >
xpcc::log::StyleWrapper<STYLE>::StyleWrapper( STYLE inStyle ) :
	style( inStyle )
{
}

// -----------------------------------------------------------------------------

template < typename STYLE >
xpcc::log::StyleWrapper<STYLE>::~StyleWrapper()
{
}

// -----------------------------------------------------------------------------

template < typename STYLE >
size_t
xpcc::log::StyleWrapper<STYLE>::write( char c )
{
	this->style.write( c );
	return 1;
}

// -----------------------------------------------------------------------------

template < typename STYLE >
size_t
xpcc::log::StyleWrapper<STYLE>::write( const char* s )
{
	this->style.write( s );
}

// -----------------------------------------------------------------------------

template < typename STYLE >
void
xpcc::log::StyleWrapper<STYLE>::flush()
{
	this->style.flush( );
}

// -----------------------------------------------------------------------------

template < typename STYLE >
int16_t
xpcc::log::StyleWrapper<STYLE>::read()
{
	return -1;
}


#endif // XPCC_LOG_STYLE_WRAPPER__HPP
