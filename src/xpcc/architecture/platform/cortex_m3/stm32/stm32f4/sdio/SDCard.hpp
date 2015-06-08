/*
 * sdcard.hpp
 *
 *  Created on: Apr 24, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_SDIO_SDCARD_HPP_
#define SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_SDIO_SDCARD_HPP_

#include <xpcc/debug.hpp>
#include <xpcc/processing/semaphore.hpp>
#include <xpcc/processing/timeout.hpp>

namespace xpcc {

enum SDError {
};

#define SDCARD_FAIL 0
#define SDCARD_V1   1
#define SDCARD_V2   2
#define SDCARD_V2HC 3

#define R1_IDLE_STATE           (1 << 0)
#define R1_ERASE_RESET          (1 << 1)
#define R1_ILLEGAL_COMMAND      (1 << 2)
#define R1_COM_CRC_ERROR        (1 << 3)
#define R1_ERASE_SEQUENCE_ERROR (1 << 4)
#define R1_ADDRESS_ERROR        (1 << 5)
#define R1_PARAMETER_ERROR      (1 << 6)

#define SD_COMMAND_TIMEOUT 50

#define SD_CMD_GO_IDLE_STATE                       ((uint8_t)0)
#define SD_CMD_SEND_OP_COND                        ((uint8_t)1)
#define SD_CMD_ALL_SEND_CID                        ((uint8_t)2)
#define SD_CMD_SET_REL_ADDR                        ((uint8_t)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                             ((uint8_t)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((uint8_t)5)
#define SD_CMD_HS_SWITCH                           ((uint8_t)6)
#define SD_CMD_SEL_DESEL_CARD                      ((uint8_t)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((uint8_t)8)
#define SD_CMD_SEND_CSD                            ((uint8_t)9)
#define SD_CMD_SEND_CID                            ((uint8_t)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((uint8_t)11) /*!< SD Card doesn't support it */
#define SD_CMD_STOP_TRANSMISSION                   ((uint8_t)12)
#define SD_CMD_SEND_STATUS                         ((uint8_t)13)
#define SD_CMD_HS_BUSTEST_READ                     ((uint8_t)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((uint8_t)15)
#define SD_CMD_SET_BLOCKLEN                        ((uint8_t)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((uint8_t)17)
#define SD_CMD_READ_MULT_BLOCK                     ((uint8_t)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((uint8_t)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((uint8_t)20) /*!< SD Card doesn't support it */
#define SD_CMD_SET_BLOCK_COUNT                     ((uint8_t)23) /*!< SD Card doesn't support it */
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((uint8_t)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((uint8_t)25)
#define SD_CMD_PROG_CID                            ((uint8_t)26) /*!< reserved for manufacturers */
#define SD_CMD_PROG_CSD                            ((uint8_t)27)
#define SD_CMD_SET_WRITE_PROT                      ((uint8_t)28)
#define SD_CMD_CLR_WRITE_PROT                      ((uint8_t)29)
#define SD_CMD_SEND_WRITE_PROT                     ((uint8_t)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((uint8_t)32) /*!< To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                    ((uint8_t)33) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                     ((uint8_t)35) /*!< To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define SD_CMD_ERASE_GRP_END                       ((uint8_t)36) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define SD_CMD_ERASE                               ((uint8_t)38)
#define SD_CMD_FAST_IO                             ((uint8_t)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((uint8_t)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((uint8_t)42)
#define SD_CMD_APP_CMD                             ((uint8_t)55)
#define SD_CMD_GEN_CMD                             ((uint8_t)56)
#define SD_CMD_NO_CMD                              ((uint8_t)64)

/**
  * @brief Following commands are SD Card Specific commands.
  *        SDIO_APP_CMD should be sent before sending these commands.
  */
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((uint8_t)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((uint8_t)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((uint8_t)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((uint8_t)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                      ((uint8_t)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                    ((uint8_t)53) /*!< For SD I/O Card only */

/**
  * @brief Following commands are SD Card Specific security commands.
  *        SDIO_APP_CMD should be sent before sending these commands.
  */
#define SD_CMD_SD_APP_GET_MKB                      ((uint8_t)43) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_MID                      ((uint8_t)44) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RN1                  ((uint8_t)45) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RN2                  ((uint8_t)46) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RES2                 ((uint8_t)47) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RES1                 ((uint8_t)48) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint8_t)18) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint8_t)25) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_ERASE                 ((uint8_t)38) /*!< For SD Card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((uint8_t)49) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((uint8_t)48) /*!< For SD Card only */

#define SDIO_SEND_IF_COND               ((uint32_t)0x00000008)

enum SDCardState
{
  SD_CARD_READY                  = ((uint32_t)0x00000001),
  SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
  SD_CARD_STANDBY                = ((uint32_t)0x00000003),
  SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
  SD_CARD_SENDING                = ((uint32_t)0x00000005),
  SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
  SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
  SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
  SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
};

uint8_t const DATA_START_BLOCK = 0XFE;
/** stop token for write multiple blocks*/
uint8_t const STOP_TRAN_TOKEN = 0XFD;

/** init timeout ms */
uint16_t const SD_INIT_TIMEOUT = 2000;
/** erase timeout ms */
uint16_t const SD_ERASE_TIMEOUT = 10000;
/** read timeout ms */
uint16_t const SD_READ_TIMEOUT = 300;
/** write time out ms */
uint16_t const SD_WRITE_TIMEOUT = 600;
/** start data token for write multiple blocks*/
uint8_t const WRITE_MULTIPLE_TOKEN = 0XFC;
/** mask for data response tokens after a write block operation */
uint8_t const DATA_RES_MASK = 0X1F;
/** write data accepted token */
uint8_t const DATA_RES_ACCEPTED = 0X05;

class SDCard {
public:
	enum class ResponseType {
			None, Short, Long, R3Resp
	};

	virtual bool init() = 0;

	virtual bool isInitialized() = 0;

	//------------------------------------------------------------------------------
	/** Begin a read multiple blocks sequence.
	 *
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	virtual bool readStart(uint32_t blockNumber) = 0;

	//------------------------------------------------------------------------------
	/** End a read multiple blocks sequence.
	 *
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	virtual bool readStop()= 0;

	virtual bool readData(uint8_t* buffer, size_t length)= 0;

	virtual bool readSingleBlock(uint8_t* buffer, size_t block_number)= 0;

	//------------------------------------------------------------------------------
	/** Start a write multiple blocks sequence.
	 *
	 * \param[in] blockNumber Address of first block in sequence.
	 * \param[in] eraseCount The number of blocks to be pre-erased.
	 *
	 * \note This function is used with writeData() and writeStop()
	 * for optimized multiple block writes.
	 *
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	virtual bool writeStart(uint32_t blockNumber, uint32_t eraseCount)= 0;
	//------------------------------------------------------------------------------
	/** End a write multiple blocks sequence.
	 *
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	virtual bool writeStop()= 0;

	//------------------------------------------------------------------------------
	/** Write one data block in a multiple block write sequence
	 * \param[in] src Pointer to the location of the data to be written.
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	virtual bool writeData(const uint8_t* src)= 0;

	//------------------------------------------------------------------------------
	// send one block of data for write block or write multiple blocks
	virtual bool writeData(uint8_t token, const uint8_t* src)= 0;

	//write a single block to SD card
	virtual bool writeBlock(uint32_t blockNumber, const uint8_t* src)= 0;

	virtual bool cmd(uint32_t cmd, uint32_t arg, ResponseType resp = ResponseType::None,
			uint32_t timeout = SD_COMMAND_TIMEOUT)= 0;

//////////////////////////////

	uint8_t initializeCard() {
		uint8_t s;
		for(int i = 0; i < 8; i++) {
			s = cmd(0, 0);
			XPCC_LOG_DEBUG.printf("_cmd(0, 0) = %x\n", s);
			// send CMD0, should return with all zeros except IDLE STATE set (bit 0)
			if(s == R1_IDLE_STATE) {
				break;
			}
		}
		if (s != R1_IDLE_STATE) {
			XPCC_LOG_DEBUG.printf(
					"No disk, or could not put SD card in to SPI idle state\n");
			return SDCARD_FAIL;
		}

		// send CMD8 to determine whther it is ver 2.x
		int r = cmd(8, 0x1AA, ResponseType::Short);
		if (r == R1_IDLE_STATE) {
			return initialise_card_v2();
		} else if (r == (R1_IDLE_STATE | R1_ILLEGAL_COMMAND)) {
			return initialise_card_v1();
		} else {
			XPCC_LOG_DEBUG.printf(
					"Not in idle state after sending CMD8 (not an SD card?)\n");
			return SDCARD_FAIL;
		}
	}

	int initialise_card_v1() {
		Timeout<> t(500);
		while (!t.isExpired()) {
			cmd(55, 0);
			if (cmd(41, 0) == 0) {
				cdv = 512;
				XPCC_LOG_DEBUG.printf("Init: SDCARD_V1\n");
				return SDCARD_V1;
			}
		}
		XPCC_LOG_DEBUG.printf("Timeout waiting for v1.x card\n");
		return SDCARD_FAIL;
	}

	int initialise_card_v2() {
		xpcc::Timeout<> t(500);
		while (!t.isExpired()) {

			cmd(58, 0);
			cmd(55, 0);
			if (cmd(41, 0x40000000) == 0) {
				cmd(58, 0);
				XPCC_LOG_DEBUG.printf("Init: SDCARD_V2\n");
				cdv = 1;
				return SDCARD_V2;
			}
		}
		XPCC_LOG_DEBUG.printf("Timeout waiting for v2.x card\n");
		return SDCARD_FAIL;
	}

	uint32_t ext_bits(unsigned char* data, int msb,
			int lsb) {
		uint32_t bits = 0;
		uint32_t size = 1 + msb - lsb;
		for (uint32_t i = 0; i < size; i++) {
			uint32_t position = lsb + i;
			uint32_t byte = 15 - (position >> 3);
			uint32_t bit = position & 0x7;
			uint32_t value = (data[byte] >> bit) & 1;
			bits |= value << i;
		}
		return bits;
	}

	uint32_t _sd_sectors() {
		uint32_t c_size, c_size_mult, read_bl_len;
		uint32_t block_len, mult, blocknr;
		uint32_t hc_c_size;
		uint64_t blocks, capacity;
		// CMD9, Response R2 (R1 byte + 16-byte block read)
		if (cmd(9, 0) != 0) {
			XPCC_LOG_DEBUG.printf("Didn't get a response from the disk\n");
			return 0;
		}
		uint8_t csd[16];
		if (!readData(csd, 16)) {
			XPCC_LOG_DEBUG.printf("Couldn't read csd response from disk\n");
			return 0;
		}
		// csd_structure : csd[127:126]
		// c_size        : csd[73:62]
		// c_size_mult   : csd[49:47]
		// read_bl_len   : csd[83:80] - the *maximum* read block length
		int csd_structure = ext_bits(csd, 127, 126);
		switch (csd_structure) {
		case 0:
			cdv = 512;
			c_size = ext_bits(csd, 73, 62);
			c_size_mult = ext_bits(csd, 49, 47);
			read_bl_len = ext_bits(csd, 83, 80);

			block_len = 1 << read_bl_len;
			mult = 1 << (c_size_mult + 2);
			blocknr = (c_size + 1) * mult;
			capacity = blocknr * block_len;
			blocks = capacity / 512;
			XPCC_LOG_DEBUG.printf(
					"SDCard\nc_size: %d \ncapacity: %lld \nsectors: %lld\n", c_size,
					capacity, blocks);
			break;

		case 1:
			cdv = 1;
			hc_c_size = ext_bits(csd, 63, 48);
			blocks = ((hc_c_size + 1) * 1024) /*- 1*/;
			capacity = (blocks * 512) / 1000000;
			XPCC_LOG_DEBUG.printf(
					"SDHC Card \nhc_c_size: %d\ncapacity: %lldMiB \nsectors: %lld\n",
					hc_c_size, capacity, blocks);
			break;

		default:
			XPCC_LOG_DEBUG.printf("CSD struct unsupported\n");
			return 0;
		};
		return blocks;
	}

	SDError errno();

	Semaphore* semaphore() {
		return &_semaphore;
	}

protected:
	bool initialized;

private:

	Semaphore _semaphore;
	uint32_t cdv;
	uint32_t _sectors;
};

}


#endif /* SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_SDIO_SDCARD_HPP_ */
