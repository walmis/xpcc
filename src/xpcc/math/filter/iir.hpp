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

template <typename T, int strength>
class IIR {
		bool first;
        T sum;
public:
        IIR(T initial = 0) {
                sum = initial * strength;
                first = false;
        }

        inline T append(T sample) {
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
