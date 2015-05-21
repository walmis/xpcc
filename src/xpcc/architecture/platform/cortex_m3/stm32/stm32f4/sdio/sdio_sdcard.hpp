/*
 * sdio_sdcard.hpp
 *
 *  Created on: Apr 24, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_SDIO_SDIO_SDCARD_HPP_
#define SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_SDIO_SDIO_SDCARD_HPP_

#include "sdio_hal.hpp"
#include <xpcc/processing/semaphore.hpp>
#include <xpcc/driver/storage/sd/SDCardInterface.hpp>

namespace xpcc {
namespace stm32 {

class SDIO_SDCard : SDCardInterface  {
public:

	SDIO_SDCard() :	dma_stm(dma::Stream::DMA2_3),
		initialized(false), _sectors(0), RCA(0) {}

	enum class ResponseType {
			None, Short, Long, R3Resp
	};

	bool init();

	bool isInitialized();

	//------------------------------------------------------------------------------
	/** Begin a read multiple blocks sequence.
	 *
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	bool readStart(uint32_t blockNumber);

	//------------------------------------------------------------------------------
	/** End a read multiple blocks sequence.
	 *
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	bool readStop();

	bool readData(uint8_t* buffer, size_t length);

	bool readSingleBlock(uint8_t* buffer, size_t block_number);

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
	bool writeStart(uint32_t blockNumber, uint32_t eraseCount);
	//------------------------------------------------------------------------------
	/** End a write multiple blocks sequence.
	 *
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	bool writeStop();

	//------------------------------------------------------------------------------
	/** Write one data block in a multiple block write sequence
	 * \param[in] src Pointer to the location of the data to be written.
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	bool writeData(const uint8_t* src);

	//------------------------------------------------------------------------------
	// send one block of data for write block or write multiple blocks
	bool writeData(uint8_t token, const uint8_t* src);

	//write a single block to SD card
	bool writeBlock(uint32_t blockNumber, const uint8_t* src);

	uint32_t getSectorCount();

	xpcc::Semaphore* semaphore() {
		return &_semaphore;
	}

private:
	bool cmd(uint32_t cmd, uint32_t arg, ResponseType resp = ResponseType::Short,
			uint32_t timeout = SD_COMMAND_TIMEOUT);

	bool getCardStatus(uint8_t &status);

	bool init_v1_card();
	bool init_v2_card();

	void sd_wide_bus(bool enable);

	uint32_t _sd_sectors(uint8_t* csd);

	bool dmaInit(uint8_t* block, bool write);
	bool startBlockTransfer(uint8_t *block, bool write, uint32_t len = 512);
	bool waitTransfer();


	dma::DMAStream dma_stm;
	dma::Config dma_cfg;


	xpcc::Semaphore _semaphore;
	uint32_t cdv;
	uint32_t _sectors;
	uint32_t SDType;//1=HC, 0=SC

	uint32_t RCA;
	bool initialized;

	uint32_t rd_block;
};

}
}

#endif /* SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_SDIO_SDIO_SDCARD_HPP_ */
