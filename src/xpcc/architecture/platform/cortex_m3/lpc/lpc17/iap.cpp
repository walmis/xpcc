/*
 * iap.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: walmis
 */

#include "iap.hpp"

namespace xpcc {
namespace lpc17 {

#define SECTOR_0_START      0x00000000
#define SECTOR_1_START      0x00001000
#define SECTOR_2_START      0x00002000
#define SECTOR_3_START      0x00003000
#define SECTOR_4_START      0x00004000
#define SECTOR_5_START      0x00005000
#define SECTOR_6_START      0x00006000
#define SECTOR_7_START      0x00007000
#define SECTOR_8_START      0x00008000
#define SECTOR_9_START      0x00009000
#define SECTOR_10_START     0x0000A000
#define SECTOR_11_START     0x0000B000
#define SECTOR_12_START     0x0000C000
#define SECTOR_13_START     0x0000D000
#define SECTOR_14_START     0x0000E000
#define SECTOR_15_START     0x0000F000
#define SECTOR_16_START     0x00010000
#define SECTOR_17_START     0x00018000
#define SECTOR_18_START     0x00020000
#define SECTOR_19_START     0x00028000
#define SECTOR_20_START     0x00030000
#define SECTOR_21_START     0x00038000
#define SECTOR_22_START     0x00040000
#define SECTOR_23_START     0x00048000
#define SECTOR_24_START     0x00050000
#define SECTOR_25_START     0x00058000
#define SECTOR_26_START     0x00060000
#define SECTOR_27_START     0x00068000
#define SECTOR_28_START     0x00070000
#define SECTOR_29_START     0x00078000

/* Define end address of each Flash sector */
#define SECTOR_0_END        0x00000FFF
#define SECTOR_1_END        0x00001FFF
#define SECTOR_2_END        0x00002FFF
#define SECTOR_3_END        0x00003FFF
#define SECTOR_4_END        0x00004FFF
#define SECTOR_5_END        0x00005FFF
#define SECTOR_6_END        0x00006FFF
#define SECTOR_7_END        0x00007FFF
#define SECTOR_8_END        0x00008FFF
#define SECTOR_9_END        0x00009FFF
#define SECTOR_10_END       0x0000AFFF
#define SECTOR_11_END       0x0000BFFF
#define SECTOR_12_END       0x0000CFFF
#define SECTOR_13_END       0x0000DFFF
#define SECTOR_14_END       0x0000EFFF
#define SECTOR_15_END       0x0000FFFF
#define SECTOR_16_END       0x00017FFF
#define SECTOR_17_END       0x0001FFFF
#define SECTOR_18_END       0x00027FFF
#define SECTOR_19_END       0x0002FFFF
#define SECTOR_20_END       0x00037FFF
#define SECTOR_21_END       0x0003FFFF
#define SECTOR_22_END       0x00047FFF
#define SECTOR_23_END       0x0004FFFF
#define SECTOR_24_END       0x00057FFF
#define SECTOR_25_END       0x0005FFFF
#define SECTOR_26_END       0x00067FFF
#define SECTOR_27_END       0x0006FFFF
#define SECTOR_28_END       0x00077FFF
#define SECTOR_29_END       0x0007FFFF

#define USER_FLASH_START (sector_start_map[USER_START_SECTOR])
#define USER_FLASH_END	 (sector_end_map[MAX_USER_SECTOR])
#define USER_FLASH_SIZE  ((USER_FLASH_END - USER_FLASH_START) + 1)
#define MAX_FLASH_SECTOR 30

#define USER_START_SECTOR 0
#define MAX_USER_SECTOR MAX_FLASH_SECTOR

const unsigned sector_start_map[MAX_FLASH_SECTOR]= { SECTOR_0_START,
		SECTOR_1_START, SECTOR_2_START, SECTOR_3_START, SECTOR_4_START,
		SECTOR_5_START, SECTOR_6_START, SECTOR_7_START, SECTOR_8_START,
		SECTOR_9_START, SECTOR_10_START, SECTOR_11_START, SECTOR_12_START,
		SECTOR_13_START, SECTOR_14_START, SECTOR_15_START, SECTOR_16_START,
		SECTOR_17_START, SECTOR_18_START, SECTOR_19_START, SECTOR_20_START,
		SECTOR_21_START, SECTOR_22_START, SECTOR_23_START, SECTOR_24_START,
		SECTOR_25_START, SECTOR_26_START, SECTOR_27_START, SECTOR_28_START,
		SECTOR_29_START };

const unsigned sector_end_map[MAX_FLASH_SECTOR] = { SECTOR_0_END, SECTOR_1_END,
		SECTOR_2_END, SECTOR_3_END, SECTOR_4_END, SECTOR_5_END, SECTOR_6_END,
		SECTOR_7_END, SECTOR_8_END, SECTOR_9_END, SECTOR_10_END, SECTOR_11_END,
		SECTOR_12_END, SECTOR_13_END, SECTOR_14_END, SECTOR_15_END,
		SECTOR_16_END, SECTOR_17_END, SECTOR_18_END, SECTOR_19_END,
		SECTOR_20_END, SECTOR_21_END, SECTOR_22_END, SECTOR_23_END,
		SECTOR_24_END, SECTOR_25_END, SECTOR_26_END, SECTOR_27_END,
		SECTOR_28_END, SECTOR_29_END };

//unsigned IAP::writeFlash(unsigned * dst, char* src,
//		unsigned no_of_bytes) {
//	unsigned i;
//	if (flash_address == 0) {
//		/* Store flash start address */
//		flash_address = (unsigned *) (dst);
//	}
//	for (i = 0; i < no_of_bytes; i++) {
//		flash_buf[(byte_ctr + i)] = *(src + i);
//	}
//	byte_ctr = byte_ctr + no_of_bytes;
//	if (byte_ctr == sizeof(flash_buf)) {
//		/* We have accumulated enough bytes to trigger a flash write */
//		findErasePrepareSector((unsigned) flash_address);
//
//		if (result_table[0] != CMD_SUCCESS) {
//			return CMD_FAILURE;
//		}
//		writeData((unsigned) flash_address,
//				(unsigned *) flash_buf, sizeof(flash_buf));
//
//		if (result_table[0] != CMD_SUCCESS) {
//			return CMD_FAILURE;
//		}
//		/* Reset byte counter and flash address */
//		byte_ctr = 0;
//		flash_address = 0;
//	}
//	return (CMD_SUCCESS);
//}

void IAP::findErasePrepareSector(unsigned flash_address) {
	unsigned i;
	__disable_irq();
	for (i = USER_START_SECTOR; i <= MAX_USER_SECTOR; i++) {
		if (flash_address < sector_end_map[i]) {
			if (flash_address == sector_start_map[i]) {
				prepareSector(i, i);
				eraseSector(i, i);
			}
			prepareSector(i, i);
			break;
		}
	}
	__enable_irq();
}

void IAP::writeData(unsigned flash_address,
		unsigned * flash_data_buf, unsigned count) {
	__disable_irq();
	param_table[0] = COPY_RAM_TO_FLASH;
	param_table[1] = flash_address;
	param_table[2] = (unsigned) (flash_data_buf);
	param_table[3] = count;
	param_table[4] = SystemCoreClock / 1000;
	iapEntry(param_table, result_table);
	__enable_irq();
}

void IAP::eraseSector(unsigned start_sector, unsigned end_sector) {
	param_table[0] = ERASE_SECTOR;
	param_table[1] = start_sector;
	param_table[2] = end_sector;
	param_table[3] = SystemCoreClock / 1000;
	iapEntry(param_table, result_table);
}

inline void IAP::prepareSector(unsigned start_sector, unsigned end_sector) {
	param_table[0] = PREPARE_SECTOR_FOR_WRITE;
	param_table[1] = start_sector;
	param_table[2] = end_sector;
	param_table[3] = SystemCoreClock / 1000;
	iapEntry(param_table, result_table);
}

inline void IAP::iapEntry(unsigned param_tab[], unsigned result_tab[]) {
	void (*iap)(unsigned[], unsigned[]);
	iap = (void (*)(unsigned[], unsigned[])) IAP_ADDRESS;
	iap(param_tab, result_tab);
}

unsigned IAP::readPartId() {
	param_table[0] = READ_PART_ID;
	param_table[1] = 0;
	param_table[2] = 0;
	param_table[3] = 0;

	iapEntry(param_table, result_table);
	return result_table[1];
}

}
}


