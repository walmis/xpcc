/*
 * iir.hpp
 *
 *  Created on: Apr 6, 2014
 *      Author: walmis
 */

#ifndef IIR_HPP_
#define IIR_HPP_

namespace xpcc {
namespace filter {

class FastIIR {

        uint32_t sum;
        uint8_t strength;
public:
        FastIIR(uint8_t strength, uint32_t initial = 0) {
                sum = initial << strength;
                this->strength = strength;
        }

        void setStrength(uint8_t strength) {
        	this->strength = strength;
        }

        uint32_t append(uint32_t sample) {
                sum = sum - (sum >> strength) + sample;
                return sum >> strength;
        }

        uint32_t getValue() {
                return sum >> strength;
        }
};

template <typename T>
class LPF {
		bool first;
		uint16_t strength;
        T sum;
public:
        LPF(uint16_t strength) : strength(strength) {
			sum = 0;
			first = true;
        }

        void setValue(T value) {
        	sum = value * strength;
        }

        inline T append(T& sample) {
        	if(!first) {
        		sum = sample * strength;
        		first = true;
        	}
            sum = sum - (sum / strength) + sample;
            return sum / strength;
        }

        inline T getValue() {
                return sum / strength;
        }
        inline T operator << (T v) {
        	return append(v);
        }
};

}
}

#endif /* IIR_HPP_ */
