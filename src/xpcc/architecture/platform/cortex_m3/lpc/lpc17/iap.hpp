/*
 * iap.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: walmis
 */

#ifndef IAP_CPP_
#define IAP_CPP_

#include "../lpc17xx.hpp"

namespace xpcc {
namespace lpc17 {

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

enum {
	CMD_SUCCESS = 0,
	CMD_FAILURE = 1,
};

#define IAP_ADDRESS 0x1FFF1FF1

class IAP {

public:
	IAP() {
		flash_address = 0;
		byte_ctr = 0;
	}

	//unsigned writeFlash(unsigned * dst, char* src, unsigned no_of_bytes);

	void findErasePrepareSector(unsigned flash_address);

	void writeData(unsigned flash_address,
			unsigned * flash_data_buf, unsigned count);

	void eraseSector(unsigned start_sector, unsigned end_sector);

	void prepareSector(unsigned start_sector, unsigned end_sector);

	void iapEntry(unsigned param_tab[], unsigned result_tab[]);

	unsigned readPartId();

private:

	unsigned * flash_address;
	unsigned byte_ctr;

	unsigned param_table[5];
	unsigned result_table[5];

};


}
}



#endif /* IAP_CPP_ */
