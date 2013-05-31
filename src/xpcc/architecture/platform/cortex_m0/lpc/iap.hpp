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

typedef enum
{
	PREPARE_SECTOR_FOR_WRITE	=50,
	COPY_RAM_TO_FLASH			=51,
	ERASE_SECTOR				=52,
	BLANK_CHECK_SECTOR			=53,
	READ_PART_ID				=54,
	READ_BOOT_VER				=55,
	COMPARE						=56,
	REINVOKE_ISP				=57
}IAP_Command_Code;

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

	static void writeFlash(unsigned flash_address, unsigned * flash_data_buf,
			unsigned count, uint32_t clk = SystemCoreClock / 1000) {
		__disable_irq();
		uint32_t param_table[5];
		uint32_t result_table[5];

		param_table[0] = COPY_RAM_TO_FLASH;
		param_table[1] = flash_address;
		param_table[2] = (unsigned) flash_data_buf;
		param_table[3] = count;
		param_table[4] = clk;
		call(param_table, result_table);
		__enable_irq();
	}

	static void eraseSector(unsigned start_sector, unsigned end_sector,
			uint32_t clk = SystemCoreClock / 1000) {

		uint32_t param_table[5];
		uint32_t result_table[5];
		param_table[0] = ERASE_SECTOR;
		param_table[1] = start_sector;
		param_table[2] = end_sector;
		param_table[3] = clk;
		call(param_table, result_table);
	}

	static void prepareSector(unsigned start_sector, unsigned end_sector,
			uint32_t clk = SystemCoreClock / 1000) {

		uint32_t param_table[5];
		uint32_t result_table[5];
		param_table[0] = PREPARE_SECTOR_FOR_WRITE;
		param_table[1] = start_sector;
		param_table[2] = end_sector;
		param_table[3] = clk;
		call(param_table, result_table);
	}


};
}
}

#endif /* IAP_HPP_ */
