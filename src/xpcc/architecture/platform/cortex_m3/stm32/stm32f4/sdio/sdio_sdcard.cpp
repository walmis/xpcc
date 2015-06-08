/*
 * sdio_sdcard.cpp
 *
 *  Created on: Apr 24, 2015
 *      Author: walmis
 */

#include "sdio_sdcard.hpp"

using namespace xpcc;
using namespace stm32;

#define SDIO_STATIC_FLAGS              (SDIO_HAL::Interrupt) ((uint32_t)0x000005FF)
#define SD_OCR_ERRORBITS                ((uint32_t)0xFDFFE008)
#define DATATIMEOUT (0xFFFFFF)

static SDIO_SDCard* inst;

SDIO_SDCard::SDIO_SDCard() : dma_stm(dma::Stream::DMA2_3),
	initialized(false), _sectors(0), RCA(0) {
	inst = this;
}

extern "C"
void SDIO_IRQHandler() {
	//IRQWrapper w;
	CH_IRQ_PROLOGUE();
	inst->handleIRQ();
	CH_IRQ_EPILOGUE();
}

static uint32_t ext_bits(unsigned char* data, int msb,
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

bool xpcc::stm32::SDIO_SDCard::init() {

	//prepare dma channel config
	dma_cfg.channel(dma::Channel::Channel_4)
			->mode(dma::Mode::Normal)
			->periphBaseAddress((uint32_t)&SDIO->FIFO)
			->peripheralBurst(dma::PeripheralBurst::INC4)
			->memoryBurst(dma::MemoryBurst::INC4)
			->memoryDataSize(dma::MemoryDataSize::Word)
			->peripheralDataSize(dma::PeripheralDataSize::Word)
			->memoryInc(dma::MemoryInc::Enable)
			->flowControl(dma::FlowControl::Peripheral)
			->fifoMode(dma::FIFOMode::Enable)
			->fifoThreshold(dma::FIFOThreshold::Full)
			->priority(dma::Prioriy::VeryHigh);

	dma_stm.attachCallback([this]() { evt.signal(); });

	SDIO_HAL::init();
	NVIC_EnableIRQ(SDIO_IRQn);

	SDIO_HAL::setBusWidth(SDIO_HAL::BusWidth::_1b);
	SDIO_HAL::setClockDiv(192);

	SDIO_HAL::setPowerState(SDIO_HAL::PowerState::On);
	SDIO_HAL::setClockState(true);

	cmd(0, 0, ResponseType::None);

	if(!cmd(SDIO_SEND_IF_COND, 0x1AA, ResponseType::Short)) {
		//sdcard v1
		init_v1_card();
	} else {
		if(init_v2_card()) {
			initialized = true;
			return true;
		}
	}


}

bool xpcc::stm32::SDIO_SDCard::isInitialized() {
	return initialized;
}

bool xpcc::stm32::SDIO_SDCard::readStart(uint32_t blockNumber) {
	//XPCC_LOG_DEBUG .printf("r+ %d\n", blockNumber);
	rd_block = blockNumber;
	return true;

}

bool xpcc::stm32::SDIO_SDCard::readStop() {
	//XPCC_LOG_DEBUG .printf("r- %d\n", rd_block);
	return true;
}

bool xpcc::stm32::SDIO_SDCard::readData(uint8_t* buffer, size_t length) {
	//XPCC_LOG_DEBUG .printf("rd %d+%d\n", rd_block, length/512);
	if(length <= 512) {
		return readSingleBlock(buffer, rd_block++);
	} else {
		bool res = readMultipleBlocks(buffer, rd_block, length/512);
		rd_block += length/512;
		return res;
	}
}

bool xpcc::stm32::SDIO_SDCard::readMultipleBlocks(uint8_t* buffer,
		size_t block_number, uint32_t numBlocks) {

	SDIO_HAL::resetDataPath();

	//initialize dma controller
	if(!dmaInit(buffer, false)) {
		return false;
	}

	//tell SDIO to begin block transfer
	if(!startBlockTransfer(buffer, false, numBlocks*512)) {
		return false;
	}

	//send read cmd to initiate transfer
	if(!cmd(SD_CMD_READ_MULT_BLOCK, block_number*cdv)) {
		return false;
	}

	//GpioProfiler<PB15> p;

	//send read cmd to initiate transfer
	bool res = waitTransfer();

	cmd(SD_CMD_STOP_TRANSMISSION, 0);

	return res;
}

bool xpcc::stm32::SDIO_SDCard::readSingleBlock(uint8_t* buffer,
		size_t block_number) {

	SDIO_HAL::resetDataPath();

	//initialize dma controller
	if(!dmaInit(buffer, false)) {
		return false;
	}

	//tell SDIO to begin block transfer
	if(!startBlockTransfer(buffer, false)) {
		return false;
	}

	//send read cmd to initiate transfer
	if(!cmd(SD_CMD_READ_SINGLE_BLOCK, block_number*cdv)) {
		return false;
	}

	//GpioProfiler<PB15> p;

	//send read cmd to initiate transfer
	return waitTransfer();

}

bool xpcc::stm32::SDIO_SDCard::writeStart(uint32_t blockNumber,
		uint32_t eraseCount) {
	//XPCC_LOG_DEBUG .printf("w+ %d", blockNumber);
	SDIO_HAL::resetDataPath();

	if (eraseCount > 1) {
		cmd(SD_CMD_APP_CMD, RCA<<16);

		if (!cmd(23, eraseCount) != 0) {
			XPCC_LOG_DEBUG.printf("SD_CARD_ERROR_ACMD23\n");
			return false;
		}
	}

	if(!cmd(SD_CMD_WRITE_MULT_BLOCK, blockNumber*cdv)) {
		return false;
	}

	//stop clock. After writeStart data blocks can be written at irregular intervals.
	//We enable the clock when there is something to write
	SDIO_HAL::setClockState(false);
	//SDIO->ICR=(uint32_t) 0xA003FF;

	return true;
}

bool xpcc::stm32::SDIO_SDCard::writeStop() {
	//XPCC_LOG_DEBUG .printf("w-");

	SDIO_HAL::setClockState(true);
	if(!cmd(SD_CMD_STOP_TRANSMISSION, 0)) {
		return false;
	}

	//GpioProfiler<PB15> p;
	uint8_t status;
	while(1) {
		if(!getCardStatus(status)) break;

		if((SDCardState)status == SDCardState::SD_CARD_PROGRAMMING) {
			sleep(1);
		} else {
			break;
		}
	}

	//XPCC_LOG_DEBUG .printf("status 0x%x\n", status);

	return true;
}

bool xpcc::stm32::SDIO_SDCard::writeData(const uint8_t* src) {
	//XPCC_LOG_DEBUG .printf("wb\n");
	SDIO_HAL::resetDataPath();

	if(!dmaInit((uint8_t*)src, true)) {
		return false;
	}
	//startBlockTransfer will enable the clock for us
	if(!startBlockTransfer((uint8_t*)src, true)) {
		return false;
	}

	if(!waitTransfer()) {
		SDIO_HAL::setClockState(true);
		return false;
	}

	//data is sent, stop clock and wait for more data
	SDIO_HAL::setClockState(false);

	return true;
}

bool xpcc::stm32::SDIO_SDCard::writeData(uint8_t token, const uint8_t* src) {
	//unused
	return false;
}

bool xpcc::stm32::SDIO_SDCard::writeBlock(uint32_t blockNumber,
		const uint8_t* src) {

	SDIO_HAL::resetDataPath();
//	SDIO_HAL::clearInterrupt(SDIO_STATIC_FLAGS);
	//SDIO->ICR=(uint32_t) 0xA003FF;

	if(!dmaInit((uint8_t*)src, true)) {
		return false;
	}

	if(!cmd(SD_CMD_WRITE_SINGLE_BLOCK, blockNumber*cdv)) {
		return false;
	}

	if(!startBlockTransfer((uint8_t*)src, true)) {
		return false;
	}

	return waitTransfer();

}

bool xpcc::stm32::SDIO_SDCard::cmd(uint32_t cmd, uint32_t arg,
		ResponseType resp, uint32_t timeout) {

	SDIO_HAL::ResponseType type;
	switch(resp) {
	case ResponseType::None:
		type = SDIO_HAL::ResponseType::None;
		break;
	case ResponseType::Short:
		type = SDIO_HAL::ResponseType::Short;
		break;
	case ResponseType::Long:
		type = SDIO_HAL::ResponseType::Long;
		break;
	case ResponseType::R3Resp:
		type = SDIO_HAL::ResponseType::Short;
		break;
	}

	evt.reset();

	//SDIO_HAL::clearInterrupt(SDIO_STATIC_FLAGS);
	SDIO->ICR=(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT | SDIO_STA_CMDREND | SDIO_STA_CMDSENT);

	SDIO_HAL::interruptConfig(SDIO_HAL::Interrupt::CMDSENT|
							  SDIO_HAL::Interrupt::CCRCFAIL|
							  SDIO_HAL::Interrupt::CTIMEOUT|
							  SDIO_HAL::Interrupt::CMDREND, true);

	SDIO_HAL::sendCommand(arg, cmd, type);

	if(!evt.wait(20)) {
		return false;
	}

	//while(!t.isExpired()) {
	uint32_t status = SDIO_HAL::getInterruptFlags();

	if(resp == ResponseType::None) {
		if(status & SDIO_HAL::Interrupt::CMDSENT) {
			XPCC_LOG_DEBUG .printf("cmd %d sent\n", cmd);

			//SDIO_HAL::clearInterrupt(SDIO_STATIC_FLAGS);
			return 1;
		}
	} else {

		if(status & (SDIO_HAL::Interrupt::CMDREND|
				SDIO_HAL::Interrupt::CCRCFAIL|SDIO_HAL::Interrupt::CTIMEOUT))  {

			if ((status & SDIO_HAL::Interrupt::CMDREND)
					|| ((status & SDIO_HAL::Interrupt::CCRCFAIL)
							&& resp == ResponseType::R3Resp)) {
//
				//delay_ms(5);
//					XPCC_LOG_DEBUG.printf("cmd%d STA:%x resp received cmd:%d r:%x\n",
//							cmd, SDIO->STA, SDIO_HAL::getCommandResponse(),
//							SDIO_HAL::getResponse(SDIO_HAL::SDIOResp::RESP1));

				//SDIO_HAL::clearInterrupt(SDIO_STATIC_FLAGS);
				return 1;
			}

			XPCC_LOG_DEBUG .printf("cmd %d err st %x\n", cmd, SDIO->STA);
			return 0;

		}
	}
	//}

	XPCC_LOG_DEBUG .printf("cmd %d timeout\n", cmd);
	return 0;

}

bool xpcc::stm32::SDIO_SDCard::init_v1_card() {
	return false;
}

bool xpcc::stm32::SDIO_SDCard::init_v2_card() {
	cmd(55, 0, ResponseType::Short);
	cmd(41, (uint32_t) 0x80100000 | (uint32_t) 0x40000000, ResponseType::R3Resp);

	while (1) {
		////Send ACMD41
		//CMD55
		cmd(55, 0, ResponseType::Short);

		//ACMD41 (Response is R3 which does not contain any CRC)
		//Second argument in the argument indicates that host supports SDHC. We will check acmd41 response if the SD is SC or HC
		cmd(41, (uint32_t) 0x80100000 | (uint32_t) 0x40000000,
				ResponseType::R3Resp);

		uint32_t response = SDIO_HAL::getResponse(SDIO_HAL::SDIOResp::RESP1);
		//Check the ready status in the response (R3)
		if ((response >> 31) == 1) { //When card is busy this bit will be 0
			//Card is now initialized. Check to see if SD is SC or HC
			SDType = (response & 0x40000000) >> 30; //1=HC, 0=SC
			break;
		}
	}

    //Now we are in the Ready State. Ask for CID using CMD2
	//Response is R2. RESP1234 are filled with CID. I will ignore them
	cmd(2, 0, ResponseType::Long);

	//Now the card is in the identification mode. Request for the RCA number with cmd3
	cmd(3, 0, ResponseType::Short);
	//Read the RCA
	RCA = SDIO_HAL::getResponse(SDIO_HAL::SDIOResp::RESP1)>>16;

	cmd(SD_CMD_SEND_CSD, RCA<<16, ResponseType::Long);

	uint32_t csd[4];

	csd[0] = __builtin_bswap32(SDIO_HAL::getResponse(SDIO_HAL::SDIOResp::RESP1));
	csd[1] = __builtin_bswap32(SDIO_HAL::getResponse(SDIO_HAL::SDIOResp::RESP2));
	csd[2] = __builtin_bswap32(SDIO_HAL::getResponse(SDIO_HAL::SDIOResp::RESP3));
	csd[3] = __builtin_bswap32(SDIO_HAL::getResponse(SDIO_HAL::SDIOResp::RESP4));

	_sectors = _sd_sectors((uint8_t*)csd);

	//Put the Card in the tranfer mode using cmd7. (I will change the clock spped later together with bus width)
	//Bus width can only be changed in transfer mode
	cmd(7, (RCA << 16));

	//set block size
	cmd(SD_CMD_SET_BLOCKLEN, 512);

	//change bus width
	sd_wide_bus(true);
	SDIO_HAL::setClockDiv(1); // set clock rate 12mhz

	//card is ready
	return true;
}

void xpcc::stm32::SDIO_SDCard::sd_wide_bus(bool enable) {
	if(enable) {
		if(cmd(SD_CMD_APP_CMD, RCA<<16)) {
			/*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
			if(cmd(SD_CMD_APP_SD_SET_BUSWIDTH, 0x02)) {
				SDIO_HAL::setBusWidth(SDIO_HAL::BusWidth::_4b);
			}
		}
	}
}

bool xpcc::stm32::SDIO_SDCard::startBlockTransfer(uint8_t* block, bool write, uint32_t len) {
	xpcc::atomic::Lock l;

	if(write) {
		SDIO_HAL::setClockState(false);
	}

	evt.reset();

	//SDIO_HAL::clearInterrupt(SDIO_STATIC_FLAGS);
	SDIO->ICR=(SDIO_STA_DCRCFAIL | SDIO_STA_DTIMEOUT | SDIO_STA_TXUNDERR | SDIO_STA_RXOVERR | SDIO_STA_DATAEND | SDIO_STA_STBITERR | SDIO_STA_DBCKEND);

	SDIO_HAL::startDataTransaction(write ? SDIO_HAL::TransferDir::ToCard :
			SDIO_HAL::TransferDir::ToSDIO, len,
			SDIO_HAL::TransferMode::Block, SDIO_HAL::DataBlockSize::_512b);

	if(write) {
		//wait for fifo to fill up
		while(!SDIO_HAL::getInterruptStatus(SDIO_HAL::Interrupt::TXFIFOF));
		//resume clock
		SDIO_HAL::setClockState(true);
	}

	return true;

}

uint32_t xpcc::stm32::SDIO_SDCard::_sd_sectors(uint8_t* csd) {

		uint32_t c_size, c_size_mult, read_bl_len;
		uint32_t block_len, mult, blocknr;
		uint32_t hc_c_size;
		uint64_t blocks, capacity;

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

uint32_t xpcc::stm32::SDIO_SDCard::getSectorCount() {
	return _sectors;
}

bool xpcc::stm32::SDIO_SDCard::waitTransfer() {
	////Check if there is an ongoing transmission
	//Check if the DMA is disabled (SDIO disables the DMA after it is done with it)

//	//Wait till SDIO is not active
//	while (SDIO_HAL::getInterruptStatus(SDIO_HAL::Interrupt::RXACT|SDIO_HAL::Interrupt::TXACT)) {
//
//	}

	//enable interrupt
	//SDIO_HAL::interruptConfig(SDIO_HAL::Interrupt::DTIMEOUT|SDIO_HAL::Interrupt::DCRCFAIL|
	//		SDIO_HAL::Interrupt::DBCKEND|SDIO_HAL::Interrupt::STBITERR, true);

	//wait for interrupt
	{
		xpcc::atomic::Unlock u;
		if(!evt.wait(20)) {
			return false;
		}
	}


	if(dma_stm.isError()) {
		XPCC_LOG_DEBUG .printf("SDIO DMA Error 0x%x\n", dma_stm.getInterruptFlags());
		return false;
	}

	//GpioProfiler<PB15> p;
	while(!SDIO_HAL::getInterruptStatus(SDIO_HAL::Interrupt::DTIMEOUT|SDIO_HAL::Interrupt::DCRCFAIL|
			SDIO_HAL::Interrupt::DBCKEND|SDIO_HAL::Interrupt::STBITERR)) {

	}

    if (!SDIO_HAL::getInterruptStatus(SDIO_HAL::Interrupt::DBCKEND)) { //An Error has occured.
    	XPCC_LOG_DEBUG .printf("SDIO:Data Transmission Error 0x%x\n", SDIO_HAL::getInterruptFlags());
      	return false;
    }


    return true;
}

bool xpcc::stm32::SDIO_SDCard::dmaInit(uint8_t* block, bool write) {
	if(((uint32_t)block & 3) != 0) {
		XPCC_LOG_DEBUG << "Err SD Block is not 4 byte aligned\n";
		return false;
	}
	if((uint32_t)block & 0x10000000) {
		XPCC_LOG_DEBUG << "Err SD Block is in CCM memory\n";
		return false;
	}

	//if read, enable sdio dma before initializing dma channel
	if(!write)
		SDIO_HAL::DMACmd(ENABLE);

	dma_cfg.memory0BaseAddress((uint32_t)block);
	dma_cfg.xferDirection(write?dma::XferDir::MemoryToPeripheral : dma::XferDir::PeripheralToMemory);
	dma_cfg.bufferSize(0);
	dma_stm.init(dma_cfg);
	//Enable the DMA (When it is enabled, it starts to respond dma requests)
	dma_stm.enable();

	if(write)
		SDIO_HAL::DMACmd(ENABLE);

	return 1;
}

bool xpcc::stm32::SDIO_SDCard::getCardStatus(uint8_t& status) {
		// Send SEND_STATUS command
		if(!cmd(SD_CMD_SEND_STATUS,RCA << 16)) {
			return false;// CMD13
		}
		// Find out card status
		status = (SDIO_HAL::getResponse(SDIO_HAL::SDIOResp::RESP1) & 0x1e00) >> 9;
		// Check for errors
		return true;
}

void xpcc::stm32::SDIO_SDCard::handleIRQ() {
	//disable interrupts
	SDIO->MASK = 0;
	//signal threads
	evt.signal();
}


