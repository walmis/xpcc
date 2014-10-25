// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2011, Roboterclub Aachen e.V.
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

#ifndef XPCC__QUATERNION_HPP
#	error	"Don't include this file directly, use 'quaternion.hpp' instead!"
#endif

#include "quaternion.hpp"
#include <type_traits>

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>::Quaternion()
:
	w(),
	x(),
	y(),
	z()
{
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>::Quaternion(T inW, T inX, T inY, T inZ)
:
	w(inW),
	x(inX),
	y(inY),
	z(inZ)
{
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>::Quaternion(const Vector<T, 3> &axis, T angle)
:
	w(),
	x(),
	y(),
	z()
{
	float sinAngleOver2 = std::sin(angle / 2);
	
	w = std::cos(angle / 2);
	x = reinterpret_cast<const T*>(&axis)[0]*sinAngleOver2;
	y = reinterpret_cast<const T*>(&axis)[1]*sinAngleOver2;
	z = reinterpret_cast<const T*>(&axis)[2]*sinAngleOver2;
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>::Quaternion(const Vector<T, 3> &u, const Vector<T, 3> &v)
:
	w(),
	x(),
	y(),
	z()
{

	Vector<T, 3> w = u.cross(v);

	this->w = 1.f + dot(u, v);
	this->x = w.x;
	this->y = w.y;
	this->z = w.z;

	normalize();
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>::Quaternion(const Matrix<T, 4, 1> &rhs)
:
	w(reinterpret_cast<const T*>(&rhs)[0]),
	x(reinterpret_cast<const T*>(&rhs)[1]),
	y(reinterpret_cast<const T*>(&rhs)[2]),
	z(reinterpret_cast<const T*>(&rhs)[3])
{
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>::Quaternion(const Quaternion<T> &rhs)
:
	w(rhs.w),
	x(rhs.x),
	y(rhs.y),
	z(rhs.z)
{
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>& xpcc::Quaternion<T>::operator = (const Matrix<T, 4, 1> &rhs)
{
	w = reinterpret_cast<const T*>(&rhs)[0];
	x = reinterpret_cast<const T*>(&rhs)[1];
	y = reinterpret_cast<const T*>(&rhs)[2];
	z = reinterpret_cast<const T*>(&rhs)[3];
	return *this;
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>& xpcc::Quaternion<T>::operator = (const Quaternion<T> &rhs)
{
	w = rhs.w;
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	
	return *this;
}

// ----------------------------------------------------------------------------
template<class T>
bool
xpcc::Quaternion<T>::operator == (const Quaternion<T> &rhs) const
{
	return (rhs.w == w) && (rhs.x == x) && (rhs.y == y) && (rhs.z == z);
}

// ----------------------------------------------------------------------------
template<class T>
bool
xpcc::Quaternion<T>::operator != (const Quaternion<T> &rhs) const
{
	return (rhs.w != w) || (rhs.x != x) || (rhs.y != y) || (rhs.z != z);
}

// ----------------------------------------------------------------------------
template<class T>
bool
xpcc::Quaternion<T>::operator < (const Quaternion<T> &rhs) const
{
	return (w < rhs.w) || ((w == rhs.w) && ((x < rhs.x) || ((x == rhs.x) && ((y < rhs.y) || ((y == rhs.y) && (z < rhs.z))))));
}

// ----------------------------------------------------------------------------
template<class T>
bool
xpcc::Quaternion<T>::operator <= (const Quaternion<T> &rhs) const
{
	return (w < rhs.w) || ((w == rhs.w) && ((x < rhs.x) || ((x == rhs.x) && ((y < rhs.y) || ((y == rhs.y) && (z <= rhs.z))))));
}

// ----------------------------------------------------------------------------
template<class T>
bool
xpcc::Quaternion<T>::operator > (const Quaternion<T> &rhs) const
{
	return (w > rhs.w) || ((w == rhs.w) && ((x > rhs.x) || ((x == rhs.x) && ((y > rhs.y) || ((y == rhs.y) && (z > rhs.z))))));
}

// ----------------------------------------------------------------------------
template<class T>
bool
xpcc::Quaternion<T>::operator >= (const Quaternion<T> &rhs) const
{
	return (w > rhs.w) || ((w == rhs.w) && ((x > rhs.x) || ((x == rhs.x) && ((y > rhs.y) || ((y == rhs.y) && (z >= rhs.z))))));
}

// ----------------------------------------------------------------------------
template<class T>
const T&
xpcc::Quaternion<T>::operator [] (int index) const
{
	return reinterpret_cast<const T*>(this)[index];
}

// ----------------------------------------------------------------------------
template<class T>
T*
xpcc::Quaternion<T>::ptr()
{
	return reinterpret_cast<T*>(this);
}

// ----------------------------------------------------------------------------
template<class T>
const T*
xpcc::Quaternion<T>::ptr() const
{
	return reinterpret_cast<const T*>(this);
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>
xpcc::Quaternion<T>::operator + (const Quaternion<T> &rhs) const
{
	return Quaternion(w+rhs.w, x+rhs.x, y+rhs.y, z+rhs.z);
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>
xpcc::Quaternion<T>::operator - (const Quaternion<T> &rhs) const
{
	return Quaternion(w-rhs.w, x-rhs.x, y-rhs.y, z-rhs.z);
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>
xpcc::Quaternion<T>::operator * (const Quaternion<T> &rhs) const
{
	return Quaternion(
		w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z,
		w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y,
		w*rhs.y + y*rhs.w + z*rhs.x - x*rhs.z,
		w*rhs.z + z*rhs.w + x*rhs.y - y*rhs.x
	);
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>
xpcc::Quaternion<T>::operator * (const T &rhs) const
{
	return Quaternion(w*rhs, x*rhs, y*rhs, z*rhs);
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>
xpcc::Quaternion<T>::operator / (const T &rhs) const
{
	return Quaternion(w/rhs, x/rhs, y/rhs, z/rhs);
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>&
xpcc::Quaternion<T>::operator += (const Quaternion<T> &rhs)
{
	w+= rhs.w; x += rhs.x; y += rhs.y; z += rhs.z;
	return *this;
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>&
xpcc::Quaternion<T>::operator -= (const Quaternion<T> &rhs)
{
	w -= rhs.w; x -= rhs.x; y -= rhs.y; z -= rhs.z;
	return *this;
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>&
xpcc::Quaternion<T>::operator *= (const T &rhs)
{
	*this = *this * rhs;

	return *this;
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>&
xpcc::Quaternion<T>::operator /= (const T &rhs)
{
	w /= rhs; x /= rhs; y /= rhs; z /= rhs;
	return *this;
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>
xpcc::Quaternion<T>::operator - ()
{
	return Quaternion(-w, -x, -y, -z);
}

// ----------------------------------------------------------------------------
template<class T>
T
xpcc::Quaternion<T>::getLength() const
{
	return std::sqrt(getLengthSquared());
}

// ----------------------------------------------------------------------------
template<class T>
T
xpcc::Quaternion<T>::getLengthSquared() const
{
	return w*w + x*x + y*y + z*z;
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>&
xpcc::Quaternion<T>::scale(T newLength)
{
	T s;
	if(std::is_same<T, float>::value) {
		s = newLength * math::fastInvSqrt(getLengthSquared());
	} else {
		s = newLength / getLength();
	}
	w *= s;
	x *= s;
	y *= s;
	z *= s;
	
	return *this;
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>&
xpcc::Quaternion<T>::normalize()
{
	T s;
	if(std::is_same<T, float>::value) {
		s = math::fastInvSqrt(getLengthSquared());
	} else {
		s = static_cast<T>(1.0) / getLength();
	}
	w *= s;
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>&
xpcc::Quaternion<T>::conjugate()
{
	x = -x;
	y = -y;
	z = -z;
	
	return *this;
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>
xpcc::Quaternion<T>::scaled(T newLength) const
{
	T s = newLength / getLength();
	return Quaternion(w*s, x*s, y*s, z*s);
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>
xpcc::Quaternion<T>::normalized() const
{
	return scaled(1.0f);
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>
xpcc::Quaternion<T>::conjugated() const
{
	return Quaternion(w, -x, -y, -z);
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>
xpcc::Quaternion<T>::unitInverse()	//	assumes we have a unit quaternion
{
	return conjugate();
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>
xpcc::Quaternion<T>::inverse()
{
	return conjugate() / getLengthSquared();
}

template<class T>
xpcc::Vector<T, 3> xpcc::Quaternion<T>::toEuler() {
	xpcc::Vector<T, 3> angles;
	float sqw = w*w;
	float sqx = x*x;
	float sqy = y*y;
	float sqz = z*z;
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = x*y + z*w;
	if (test > 0.499*unit) { // singularity at north pole
		angles.z = 2 * std::atan2(x,w);
		angles.x = M_PI/2;
		angles.y = 0;
		return angles;
	}
	if (test < -0.499*unit) { // singularity at south pole
		angles.z = -2 * std::atan2(x,w);
		angles.x = -M_PI/2;
		angles.y = 0;
		return angles;
	}
	angles.z = std::atan2(2*y*w-2*x*z , sqx - sqy - sqz + sqw);
	angles.x = std::asin(2*test/unit);
	angles.y = std::atan2(2*x*w-2*y*z , -sqx + sqy - sqz + sqw);
	return angles;
}

// ----------------------------------------------------------------------------
template<class T>
void
xpcc::Quaternion<T>::to4x4Matrix(Matrix<T, 4, 4> *outMatrix)
{
	// avoid depending on Matrix.h
	T* m = reinterpret_cast<T*>(outMatrix);
	
	float xx = x*x;	float xy = x*y;
	float xz = x*z;	float xw = x*w;

	float yy = y*y;	float yz = y*z;
	float yw = y*w;

	float zz = z*z;	float zw = z*w;

	m[0*4+0] = 1-2*(yy+zz); m[1*4+0] =   2*(xy-zw); m[2*4+0] =   2*(xz+yw); m[3*4+0] = 0;
	m[0*4+1] =   2*(xy+zw); m[1*4+1] = 1-2*(xx+zz); m[2*4+1] =   2*(yz-xw); m[3*4+1] = 0;
	m[0*4+2] =   2*(xz-yw); m[1*4+2] =   2*(yz+xw); m[2*4+2] = 1-2*(xx+yy); m[3*4+2] = 0;
	m[0*4+3] =           0; m[1*4+3] =           0; m[2*4+3] =           0; m[3*4+3] = 1;
}

// ----------------------------------------------------------------------------
template<class T>
void
xpcc::Quaternion<T>::to3x3Matrix(Matrix<T, 3, 3> *outMatrix)
{
	// avoid depending on Matrix.h
	T* m = reinterpret_cast<T*>(outMatrix);

	float xx = x*x;	float xy = x*y;
	float xz = x*z;	float xw = x*w;

	float yy = y*y;	float yz = y*z;
	float yw = y*w;

	float zz = z*z;	float zw = z*w;

	m[0*3+0] = 1-2*(yy+zz); m[1*3+0] =   2*(xy-zw); m[2*3+0] =   2*(xz+yw);
	m[0*3+1] =   2*(xy+zw); m[1*3+1] = 1-2*(xx+zz); m[2*3+1] =   2*(yz-xw);
	m[0*3+2] =   2*(xz-yw); m[1*3+2] =   2*(yz+xw); m[2*3+2] = 1-2*(xx+yy);
}
template<typename T>
xpcc::Quaternion<T>::Quaternion(T yaw, T pitch, T roll) {

    float c1 = std::cos(pitch/2);
    float s1 = std::sin(pitch/2);
    float c2 = std::cos(roll/2);
    float s2 = std::sin(roll/2);
    float c3 = std::cos(yaw/2);
    float s3 = std::sin(yaw/2);

    float c2c3 = c2*c3;
    float s1s2 = s1*s2;

    w =c1*c2c3 + s1s2*s3;
    x =s1*c2c3 - c1*s2*s3;
    y =c1*s2*c3 + s1*c2*s3;
    z =c1*c2*s3 - s1s2*c3;
}

// ----------------------------------------------------------------------------
template<class T>
xpcc::Quaternion<T>
xpcc::operator * (const T &lhs, const xpcc::Quaternion<T> &rhs)
{
	return rhs * lhs;
}
