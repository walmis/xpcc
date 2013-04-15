/*
 * iap.hpp
 *
 *  Created on: Apr 15, 2013
 *      Author: walmis
 */

#ifndef IAP_HPP_
#define IAP_HPP_


namespace xpcc {
namespace lpc11 {

#define IAP_ADDRESS 0x1FFF1FF1
class IAP {
public:
	static bool call(uint32_t param_tab[5], uint32_t result_tab[5]) {
		void (*iap)(uint32_t[], uint32_t[]);
		iap = (void (*)(uint32_t[], uint32_t[]))IAP_ADDRESS;

		iap(param_tab, result_tab);
		if(result_tab[0] == 0)
			return true;

		return false;

	}

};
}
}

#endif /* IAP_HPP_ */
