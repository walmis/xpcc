#pragma once

namespace xpcc {
namespace lpc17 {

#include <xpcc/architecture.hpp>

#define DAC_VALUE(n) 		((uint32_t)((n&0x3FF)<<6))
/** If this bit = 0: The settling time of the DAC is 1 microsecond max,
 * and the maximum current is 700 microAmpere
 * If this bit = 1: The settling time of the DAC is 2.5 microsecond
 * and the maximum current is 350 microAmpere */
#define DAC_BIAS_EN			((uint32_t)(1<<16))
/** Value to reload interrupt DMA counter */
#define DAC_CCNT_VALUE(n)  ((uint32_t)(n&0xffff))

/** DCAR double buffering */
#define DAC_DBLBUF_ENA		((uint32_t)(1<<1))
/** DCAR Time out count enable */
#define DAC_CNT_ENA			((uint32_t)(1<<2))
/** DCAR DMA access */
#define DAC_DMA_ENA			((uint32_t)(1<<3))
/** DCAR DACCTRL mask bit */
#define DAC_DACCTRL_MASK	((uint32_t)(0x0F))

enum
{
	DAC_MAX_CURRENT_700uA = 0, 	/*!< The settling time of the DAC is 1 us max,
								and the maximum	current is 700 uA */
	DAC_MAX_CURRENT_350uA		/*!< The settling time of the DAC is 2.5 us
								and the maximum current is 350 uA */
};

class DAC {
public:
	static void init() {
		CLKPwr::setClkDiv(CLKPwr::ClkType::DAC, CLKPwr::DIV_1);
		Pinsel::setFunc(0, 26, 2);

		setBias(0);
	}

	static void configDAConverterControl (bool double_buffer,
			bool timeout_counter,
			bool dma_enable) {

		LPC_DAC->DACCTRL &= ~DAC_DACCTRL_MASK;
		if (double_buffer)
			LPC_DAC->DACCTRL	|= DAC_DBLBUF_ENA;
		if (timeout_counter)
			LPC_DAC->DACCTRL	|= DAC_CNT_ENA;
		if (dma_enable)
			LPC_DAC->DACCTRL	|= DAC_DMA_ENA;
	}

	static void setValue (uint32_t dac_value)
	{
		uint32_t tmp;

		tmp = LPC_DAC->DACR & DAC_BIAS_EN;
		tmp |= DAC_VALUE(dac_value);
		// Update value
		LPC_DAC->DACR = tmp;
	}


	static void setBias (bool bias)	{

		LPC_DAC->DACR &=~DAC_BIAS_EN;
		if (bias  == DAC_MAX_CURRENT_350uA)
		{
			LPC_DAC->DACR |= DAC_BIAS_EN;
		}
	}

	static void setDMATimeOut(uint32_t time_out)
	{

		LPC_DAC->DACCNTVAL = DAC_CCNT_VALUE(time_out);
	}

};


}
}
