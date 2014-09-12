//!
//! @file 		FixedPoint-Fp32f.hpp
//! @author 	Geoffrey Hunter <gbmhunter@gmail.com> (www.cladlab.com), Markus Trenkwalder
//! @edited 	n/a
//! @date 		2012/10/23
//! @brief 		Fast 32-bit fixed point library.
//! @details
//!				See README.rst in root dir for more info.


/* Original copyright notice
Copyright (c) 2007, Markus Trenkwalder

All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

* Neither the name of the library's copyright owner nor the names of its 
  contributors may be used to endorse or promote products derived from this 
  software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//===============================================================================================//
//====================================== HEADER GUARD ===========================================//
//===============================================================================================//

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

#ifndef FP32F_H
#define FP32F_H

#include <xpcc/io.hpp>
#include <stdint.h>

namespace xpcc
{

	// The template argument q in all of the following functions refers to the 
	// fixed point precision (e.g. q = 8 gives 24.8 fixed point functions).

	//! @brief		Perform a fixed point multiplication without a 64-bit intermediate result.
	//!	@note 		This is fast but beware of intermediatry overflow!
	template <uint8_t q> 
	constexpr inline int32_t FixMulF(int32_t a, int32_t b)
	{
		return (a * b) >> q;
	}

	//! @brief		Perform a fixed point multiplication using a 64-bit intermediate result to
	//! 			prevent intermediatry overflow problems.
	//! @note 		Slower than Fp32f::FixMulF()
	template <uint8_t q>
	constexpr int32_t FixMul(int32_t a, int32_t b)
	{
		return (int32_t)(((int64_t)a * b) >> q);
	}

	// Fixed point division
	template <uint8_t q>
	int32_t fixdiv(const int32_t &a, int32_t b)
	{
		#if 0
			return (int32_t)((((int64_t)a) << q) / b);
		#else	
			// The following produces the same results as the above but gcc 4.0.3 
			// generates fewer instructions (at least on the ARM processor).
			union {
				int64_t a;
				struct {
					int32_t l;
					int32_t h;
				};
			} x;

			x.l = a << q;
			x.h = a >> (sizeof(int32_t) * 8 - q);
			return (int32_t)(x.a / b);
		#endif
	}

	namespace detail {
		inline uint32_t CountLeadingZeros(uint32_t x)
		{
			return __builtin_clz(x);
		}
	}

	// q is the precision of the input
	// output has 32-q bits of fraction
	template <uint8_t q>
	inline int32_t fixinv(int32_t a)
	{
		int32_t x;

		bool sign = false;

		if (a < 0) {
			sign = true;
			a = -a;
		}

		static const uint16_t rcp_tab[] = { 
			0x8000, 0x71c7, 0x6666, 0x5d17, 0x5555, 0x4ec4, 0x4924, 0x4444
		};
			
		int32_t exp = detail::CountLeadingZeros(a);
		x = ((int32_t)rcp_tab[(a>>(28-exp))&0x7]) << 2;
		exp -= 16;

		if (exp <= 0)
			x >>= -exp;
		else
			x <<= exp;

		// Two iterations of newton-raphson  x = x(2-ax)
		x = FixMul<(32-q)>(x,((2<<(32-q)) - FixMul<q>(a,x)));
		x = FixMul<(32-q)>(x,((2<<(32-q)) - FixMul<q>(a,x)));

		if (sign)
			return -x;
		else
			return x;
	}

	//! @brief		Converts from float to fixed-point.
	//! @details	Good for inputting values into fixed-point arithmetic.
	//! @warning	Slow!
	template <uint8_t q>
	constexpr int32_t FloatToFix32(float f)
	{
		return (int32_t)(f * (1 << q));
	}
	
	//! @brief		Converts from double to fixed-point.
	//! @details	Good for inputting values into fixed-point arithmetic.
	//! @warning	Slow!
	template <uint8_t q>
	constexpr int32_t DoubleToFix32(double f)
	{
		return (int32_t)(f * (double)(1 << q));
	}
	
	

	int32_t fixcos16(int32_t a);
	int32_t fixsin16(int32_t a);
	int32_t fixrsqrt16(int32_t a);
	int32_t fixsqrt16(int32_t a);

	//! The template argument p in all of the following functions refers to the 
	//! fixed point precision (e.g. q = 8 gives 24.8 fixed point functions).
	//! Contains mathematical operator overloading. Doesn't have modulus (%) overloading
	template <uint8_t q>
	class Fp32f {
		
		public:
		
		//! @brief		The fixed-point number is stored in this basic data type.
		int32_t rawVal;			
		
		constexpr Fp32f() : rawVal(0)
		{
		}
		
//		constexpr Fp32f(Fp32f const &i) : rawVal(i.rawVal) {
//		}

		template<uint8_t w>
		Fp32f(Fp32f<w> i) {
			*this = i;
		}

		constexpr Fp32f(int8_t i) : rawVal((int32_t)i << q)
		{
			
		}
		
		constexpr Fp32f(int16_t i) : rawVal((int32_t)i << q)
		{
			
		}
		
		constexpr Fp32f(int32_t i) : rawVal(i << q)
		{
		
		}
		constexpr Fp32f(int64_t i) : rawVal((int32_t)i)
		{

		}
		
		constexpr Fp32f(int i) : rawVal(i << q)
		{

		}

		constexpr Fp32f(float f) : rawVal(FloatToFix32<q>(f))
		{
		
		}
		
		constexpr Fp32f(double f) : rawVal(FloatToFix32<q>((float)f))
		{
		
		}
		
		template<uint8_t w>
		inline Fp32f& operator = (Fp32f<w> r) {
			if(w-q < 0) {
				rawVal = r.rawVal << (q-w);
			} else {
				rawVal = r.rawVal >> (w-q);
			}

			return *this;
		}

		// Compound Arithmetic Overloads
		
		Fp32f& operator += (const Fp32f r)
		{
			rawVal += r.rawVal;
			return *this;
		}
		
		Fp32f& operator -= (const Fp32f r)
		{
			rawVal -= r.rawVal;
			return *this;
		}
		
		//! @brief		Overlaod for '*=' operator.
		//! @details	Uses intermediatary casting to int64_t to prevent overflows.
		Fp32f& operator *= (Fp32f r)
		{
			rawVal = FixMul<q>(rawVal, r.rawVal);
			return *this;
		}
		
		//! @brief		Overlaod for '/=' operator.
		//! @details	Uses intermediatary casting to int64_t to prevent overflows.
		Fp32f& operator /= (Fp32f r)
		{
			rawVal = fixdiv<q>(rawVal, r.rawVal);
			return *this;
		}
		
		//! @brief		Overlaod for '%=' operator.
		Fp32f& operator %= (Fp32f r)
		{
			rawVal %= r.rawVal;
			return *this;
		}

		template <typename T>
		Fp32f& operator *= (T r)
		{
			*this *= Fp32f<q>(r);
			return *this;
		}
		
		template <typename T>
		Fp32f& operator /= (T r)
		{
			*this /= Fp32f(r);
			return *this;
		}
		
		// Simple Arithmetic Overloads
		
		//! @brief		Overload for '-itself' operator.
		Fp32f operator - () const
		{
			Fp32f x;
			x.rawVal = -rawVal;
			return x;
		}
		
		//! @brief		Overload for '+' operator.
		//! @details	Uses '+=' operator.
		Fp32f operator + (Fp32f r) const
		{
			Fp32f x = *this;
			x += r;
			return x;
		}
		
		//! @brief		Overload for '-' operator.
		//! @details	Uses '-=' operator.
		Fp32f operator - (Fp32f r) const
		{
			Fp32f x = *this;
			x -= r;
			return x;
		}
		
		//! @brief		Overload for '*' operator.
		//! @details	Uses '*=' operator.
		Fp32f operator * (Fp32f r) const
		{
			Fp32f x = *this;
			x *= r;
			return x;
		}
		
		//! @brief		Overload for '/' operator.
		//! @details	Uses '/=' operator.
		constexpr Fp32f operator / (Fp32f r) const
		{
			//Fp32f x = *this;
			//x /= r;
			return fixdiv<q>(rawVal, r.rawVal);
		}

		//! @brief		Overload for '%' operator.
		//! @details	Uses '%=' operator.
		Fp32f operator % (Fp32f r) const
		{
			Fp32f x = *this;
			x %= r;
			return x;
		}
		
		// Fp32f-Fp32f Binary Operator Overloads
		
		bool operator == (Fp32f r) const
		{
			return rawVal == r.rawVal;
		}
		
		bool operator != (const Fp32f &r)
		{
			return !(*this == r);
		}
		
		bool operator <  (const Fp32f &r)
		{
			return rawVal < r.rawVal;
		}
		
		bool operator >  (const Fp32f &r)
		{
			return rawVal > r.rawVal;
		}
		
		bool operator <= (Fp32f r) const
		{
			return rawVal <= r.rawVal;
		}
		
		bool operator >= (Fp32f r) const
		{
			return rawVal >= r.rawVal;
		}
		
		//! @defgroup Explicit "From Fp32f" Conversion Overloads (casts)
		//! @{
		
		//! @brief		Conversion operator from fixed-point to int16_t.
		//! @warning	Possible loss of accuracy from conversion from
		//!				int32_t to int16_t.
		operator int16_t() const
		{
			// Right-shift to get rid of all the decimal bits (truncate)
			return (int16_t)(rawVal >> q);
		}
		
		//! @brief		Conversion operator from fixed-point to int32_t.
		operator int32_t() const
		{
			// Right-shift to get rid of all the decimal bits (truncate)
			return (rawVal >> q);
		}
		
		//! @brief		Conversion operator from fixed-point to int64_t.
		operator int64_t() const
		{
			// Right-shift to get rid of all the decimal bits (truncate)
			return (int64_t)(rawVal >> q);
		}
		
		//! @brief		Conversion operator from fixed-point to float.
		operator float() const
		{ 
			return (float)rawVal / (float)(1 << q);
		}
		
		//! @brief		Conversion operator from fixed-point to double.
		//! @note		Similar to float conversion.
		operator double() const
		{ 
			return (double)rawVal / (double)(1 << q);
		}
		
		//! @}
		
		// Overloads Between Fp32f And int32_t
		Fp32f operator <<(int r) const
		{
			Fp32f x = *this;
			x.rawVal <<= r;
			return x;
		}
		
		Fp32f operator >>(int r) const
		{
			Fp32f x = *this;
			x.rawVal >>= r;
			return x;
		}

		template <typename T>
		Fp32f operator + (T r) const
		{
			Fp32f x = *this;
			x += static_cast<Fp32f>(r);
			return x;
		}
		
		template <typename T>
		Fp32f operator - (T r) const
		{
			Fp32f x = *this;
			x -= static_cast<Fp32f>(r);
			return x;
		}
		template <typename T>
		Fp32f operator * (T r) const
		{
			Fp32f x = *this;
			x *= static_cast<Fp32f>(r);
			return x;
		}

		template<typename T>
		Fp32f operator / (T r) const
		{
			Fp32f x = *this;
			x /= Fp32f(r);
			return x;
		}

		
		bool operator >  (int32_t r) const
		{
			return rawVal > (r << q);
		}
		
		bool operator >=  (int32_t r) const
		{
			return rawVal >= (r << q);
		}
		
		bool operator <  (int32_t r) const
		{
			return rawVal < (r << q);
		}
		
		bool operator <=  (int32_t r) const
		{
			return rawVal < (r << q);
		}
		
		bool operator ==  (int32_t r) const
		{
			return rawVal == (r << q);
		}
		
		bool operator !=  (int32_t r) const
		{
			return rawVal != (r << q);
		}
		
	};

	// Specializations for use with plain integers

	//! @note 		Assumes integer has the same precision as Fp32f

	template <uint8_t q, typename T>
	inline Fp32f<q> operator + (T a, Fp32f<q> b)
	{
		return b + Fp32f<q>(a);
	}

	//! @note 		Assumes integer has the same precision as Fp32f
	template <uint8_t q, typename T>
	inline Fp32f<q> operator - (T a, Fp32f<q> b)
	{
		return -b + Fp32f<q>(a);
	}


	template <uint8_t q, typename T>
	inline Fp32f<q> operator *(T a, Fp32f<q> b)
	{
		return Fp32f<q>(a) * b;
	}

	template <uint8_t q, typename T>
	inline Fp32f<q> operator / (T a, Fp32f<q> b)
	{ 
		Fp32f<q> r(a); 
		r /= b; 
		return r;
	}

	template <uint8_t q, typename T>
	inline Fp32f<q> operator % (T a, Fp32f<q> b)
	{
		Fp32f<q> r(a);
		r %= b;
		return r;
	}

	// math functions
	// no default implementation

	template <uint8_t q>
	inline Fp32f<q> sin(Fp32f<q> a) {
		return std::sin(a);
	}

	template <uint8_t q>
	inline Fp32f<q> cos(Fp32f<q> a) {
		return std::cos(a);
	}

	template <uint8_t q>
	inline Fp32f<q> sqrt(Fp32f<q> a) {
		return std::sqrt(a);
	}

	template <uint8_t q>
	inline Fp32f<q> atan(Fp32f<q> a) {
		return std::sqrt(a);
	}
	template <uint8_t q>
	inline Fp32f<q> atan2(Fp32f<q> a, Fp32f<q> b) {
		return std::atan2(a, b);
	}

	template <uint8_t q>
	inline Fp32f<q> rsqrt(Fp32f<q> a);

	template <uint8_t q>
	inline Fp32f<q> inv(Fp32f<q> a);

	template <uint8_t q>
	inline Fp32f<q> abs(Fp32f<q> a)
	{ 
		Fp32f<q> r; 
		r.rawVal = a.rawVal > 0 ? a.rawVal : -a.rawVal; 
		return r; 
	}

	// Specializations for 16.16 format

	template <>
	inline Fp32f<16> sin(Fp32f<16> a)
	{
		Fp32f<16> r;
		r.rawVal = fixsin16(a.rawVal);
		return r;
	}

	template <>
	inline Fp32f<16> cos(Fp32f<16> a)
	{
		Fp32f<16> r;
		r.rawVal = fixcos16(a.rawVal);
		return r;
	}


	template <>
	inline Fp32f<16> sqrt(Fp32f<16> a)
	{
		Fp32f<16> r;
		r.rawVal = fixsqrt16(a.rawVal);
		return r;
	}

	template <>
	inline Fp32f<16> rsqrt(Fp32f<16> a)
	{
		Fp32f<16> r;
		r.rawVal = fixrsqrt16(a.rawVal);
		return r;
	}

	template <>
	inline Fp32f<16> inv(Fp32f<16> a)
	{
		Fp32f<16> r;
		r.rawVal = fixinv<16>(a.rawVal);
		return r;
	}

	// The multiply accumulate case can be optimized.
	template <uint8_t q>
	inline Fp32f<q> multiply_accumulate(
		int32_t count, 
		const Fp32f<q> *a,
		const Fp32f<q> *b)
	{
		long long result = 0;
		for (int32_t i = 0; i < count; ++i)
			result += static_cast<long long>(a[i].rawVal) * b[i].rawVal;
		Fp32f<q> r;
		r.rawVal = static_cast<int32_t>(result >> q);
		return r;
	}
	
	// Make the fixed-point struct  ostream outputtable
	template <uint8_t q>
	xpcc::IOStream& operator<< (xpcc::IOStream &stream, Fp32f<q> fixedPoint)
	{
		int32_t i = fixedPoint.rawVal;

	    if(i < 0) {
	    	i = -i;
	    	stream << '-';
	    }

	    int32_t integer = i >> q;
	    i &= (1<<q)-1;

	    stream << integer << '.';

	    int32_t quot = (((int64_t)i*100000) >> q);

	    for(int x = 1; x < 100000; x*=10) {
	    	if(quot < x) {
	    		stream << '0';
	    	}
	    }
	    if(quot) {
	    	stream << quot;
	    }

		return stream;
	}
	typedef Fp32f<4> Q4;
	typedef Fp32f<5> Q5;
	typedef Fp32f<6> Q6;
	typedef Fp32f<7> Q7;
	typedef Fp32f<8> Q8;
	typedef Fp32f<9> Q9;
	typedef Fp32f<10> Q10;
	typedef Fp32f<11> Q11;
	typedef Fp32f<12> Q12;
	typedef Fp32f<13> Q13;
	typedef Fp32f<14> Q14;
	typedef Fp32f<15> Q15;
	typedef Fp32f<16> Q16;
	typedef Fp32f<17> Q17;
	typedef Fp32f<18> Q18;
	typedef Fp32f<20> Q20;
	typedef Fp32f<21> Q21;
	typedef Fp32f<22> Q22;
	typedef Fp32f<23> Q23;
	typedef Fp32f<24> Q24;
	typedef Fp32f<25> Q25;
	typedef Fp32f<26> Q26;
	typedef Fp32f<27> Q27;
	typedef Fp32f<28> Q28;
	typedef Fp32f<29> Q29;
	typedef Fp32f<30> Q30;
	typedef Fp32f<31> Q31;
}

namespace std {

template <uint8_t q>
inline xpcc::Fp32f<q> sin(xpcc::Fp32f<q> a) {
	return xpcc::Fp32f<q>(std::sin((float)a));
}

template <uint8_t q>
inline xpcc::Fp32f<q> cos(xpcc::Fp32f<q> a) {
	return xpcc::Fp32f<q>(std::cos((float)a));
}

template <uint8_t q>
inline xpcc::Fp32f<q> atan(xpcc::Fp32f<q> a) {
	return xpcc::Fp32f<q>(std::atan((float)a));
}

template <uint8_t q>
inline xpcc::Fp32f<q> atan2(xpcc::Fp32f<q> a, xpcc::Fp32f<q> b) {
	return xpcc::Fp32f<q>(std::atan2((float)a, (float)b));
}

template<uint8_t q>
inline xpcc::Fp32f<q> sqrt(xpcc::Fp32f<q> s) {
	xpcc::Fp32f<q> x, px;
	x = s >> 1;

	int i = 0;
	do {
		px = x;
		x = (s >> 1) / x + (x >> 1);
		if (i > 10)
			return (int32_t)0;
	} while ((px.rawVal - x.rawVal) != 0);

	return x;
}




}


#endif // #ifndef FP32F_H

// EOF
