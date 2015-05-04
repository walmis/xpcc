/*
 * channel.hpp
 *
 *  Created on: Apr 21, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_DMA_DMA_CHANNEL_HPP_
#define SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_DMA_DMA_CHANNEL_HPP_

#include "../../../stm32.hpp"
#include <xpcc/processing/function.hpp>

namespace xpcc {
namespace stm32 {
namespace dma {

enum class Stream {
	DMA1_0 = (uint32_t) DMA1_Stream0,
	DMA1_1 = (uint32_t) DMA1_Stream1,
	DMA1_2 = (uint32_t) DMA1_Stream2,
	DMA1_3 = (uint32_t) DMA1_Stream3,
	DMA1_4 = (uint32_t) DMA1_Stream4,
	DMA1_5 = (uint32_t) DMA1_Stream5,
	DMA1_6 = (uint32_t) DMA1_Stream6,
	DMA1_7 = (uint32_t) DMA1_Stream7,
	DMA2_0 = (uint32_t) DMA2_Stream0,
	DMA2_1 = (uint32_t) DMA2_Stream1,
	DMA2_2 = (uint32_t) DMA2_Stream2,
	DMA2_3 = (uint32_t) DMA2_Stream3,
	DMA2_4 = (uint32_t) DMA2_Stream4,
	DMA2_5 = (uint32_t) DMA2_Stream5,
	DMA2_6 = (uint32_t) DMA2_Stream6,
	DMA2_7 = (uint32_t) DMA2_Stream7,
};

enum class Channel {
	Channel_0 = ((uint32_t) 0x00000000),
	Channel_1 = ((uint32_t) 0x02000000),
	Channel_2 = ((uint32_t) 0x04000000),
	Channel_3 = ((uint32_t) 0x06000000),
	Channel_4 = ((uint32_t) 0x08000000),
	Channel_5 = ((uint32_t) 0x0A000000),
	Channel_6 = ((uint32_t) 0x0C000000),
	Channel_7 = ((uint32_t) 0x0E000000)
};

enum class XferDir {
	PeripheralToMemory = ((uint32_t) 0x00000000),
	MemoryToPeripheral = ((uint32_t) 0x00000040),
	MemoryToMemory = ((uint32_t) 0x00000080)
};

enum class PeripheralInc {
	Disable = 0,
	Enable = ((uint32_t) 0x00000200)
};

enum class MemoryInc {
	Enable = ((uint32_t) 0x00000400),
	Disable = ((uint32_t) 0x00000000)
};

enum class PeripheralDataSize {
	Byte = ((uint32_t) 0x00000000),
	HalfWord = ((uint32_t) 0x00000800),
	Word = ((uint32_t) 0x00001000)
};

enum class MemoryDataSize {
	Byte = ((uint32_t) 0x00000000),
	HalfWord = ((uint32_t) 0x00002000),
	Word = ((uint32_t) 0x00004000)
};

enum class Mode {
	Normal = ((uint32_t) 0x00000000),
	Circular = ((uint32_t) 0x00000100)
};

enum class Prioriy {
	Low = ((uint32_t) 0x00000000),
	Medium = ((uint32_t) 0x00010000),
	High = ((uint32_t) 0x00020000),
	VeryHigh = ((uint32_t) 0x00030000)
};

enum class FIFOMode {
	Disable = ((uint32_t) 0x00000000),
	Enable = ((uint32_t) 0x00000004)
};

enum class FIFOThreshold {
	OneQuarterFull = ((uint32_t) 0x00000000),
	HalfFull = ((uint32_t) 0x00000001),
	ThreeQuartersFull = ((uint32_t) 0x00000002),
	Full = ((uint32_t) 0x00000003)
};

enum class MemoryBurst {
	Single = ((uint32_t) 0x00000000),
	INC4 = ((uint32_t) 0x00800000),
	INC8 = ((uint32_t) 0x01000000),
	INC16 = ((uint32_t) 0x01800000)
};

enum class PeripheralBurst {
	Single = ((uint32_t) 0x00000000),
	INC4 = ((uint32_t) 0x00200000),
	INC8 = ((uint32_t) 0x00400000),
	INC16 = ((uint32_t) 0x00600000)
};

enum class FIFOStatus {
	Less1QuarterFull  = ((uint32_t) 0x00000000 << 3),
	OneQuarterFull    = ((uint32_t) 0x00000001 << 3),
	HalfFull          = ((uint32_t) 0x00000002 << 3),
	ThreeQuartersFull = ((uint32_t) 0x00000003 << 3),
	Empty             = ((uint32_t) 0x00000004 << 3),
	Full              = ((uint32_t) 0x00000005 << 3)
};

enum class FlowControl {
	Memory     = ((uint32_t)0x00000000),
	Peripheral = ((uint32_t)0x00000020)
};

enum class Memory {
	Memory_0  = ((uint32_t)0x00000000),
	Memory_1  = ((uint32_t)0x00080000)
};

enum class PINCOS {
	Psize                 = ((uint32_t)0x00000000),
	WordAligned           = ((uint32_t)0x00008000)
};

enum class IntFlag {

	FEIF0  = ((uint32_t) 0x10800001),
	DMEIF0 = ((uint32_t) 0x10800004),
	TEIF0  = ((uint32_t) 0x10000008),
	HTIF0  = ((uint32_t) 0x10000010),
	TCIF0  = ((uint32_t) 0x10000020),
	FEIF1  = ((uint32_t) 0x10000040),
	DMEIF1 = ((uint32_t) 0x10000100),
	TEIF1  = ((uint32_t) 0x10000200),
	HTIF1  = ((uint32_t) 0x10000400),
	TCIF1  = ((uint32_t) 0x10000800),
	FEIF2  = ((uint32_t) 0x10010000),
	DMEIF2 = ((uint32_t) 0x10040000),
	TEIF2  = ((uint32_t) 0x10080000),
	HTIF2  = ((uint32_t) 0x10100000),
	TCIF2  = ((uint32_t) 0x10200000),
	FEIF3  = ((uint32_t) 0x10400000),
	DMEIF3 = ((uint32_t) 0x11000000),
	TEIF3  = ((uint32_t) 0x12000000),
	HTIF3  = ((uint32_t) 0x14000000),
	TCIF3  = ((uint32_t) 0x18000000),
	FEIF4  = ((uint32_t) 0x20000001),
	DMEIF4 = ((uint32_t) 0x20000004),
	TEIF4  = ((uint32_t) 0x20000008),
	HTIF4  = ((uint32_t) 0x20000010),
	TCIF4  = ((uint32_t) 0x20000020),
	FEIF5  = ((uint32_t) 0x20000040),
	DMEIF5 = ((uint32_t) 0x20000100),
	TEIF5  = ((uint32_t) 0x20000200),
	HTIF5  = ((uint32_t) 0x20000400),
	TCIF5  = ((uint32_t) 0x20000800),
	FEIF6  = ((uint32_t) 0x20010000),
	DMEIF6 = ((uint32_t) 0x20040000),
	TEIF6  = ((uint32_t) 0x20080000),
	HTIF6  = ((uint32_t) 0x20100000),
	TCIF6  = ((uint32_t) 0x20200000),
	FEIF7  = ((uint32_t) 0x20400000),
	DMEIF7 = ((uint32_t) 0x21000000),
	TEIF7  = ((uint32_t) 0x22000000),
	HTIF7  = ((uint32_t) 0x24000000),
	TCIF7  = ((uint32_t) 0x28000000)
};

enum class Interrupt {
	TransferComplete                        = ((uint32_t)0x00000010),
	HalfTransfer                        = ((uint32_t)0x00000008),
	TransferError                        = ((uint32_t)0x00000004),
	DirectModeError                       = ((uint32_t)0x00000002),
	FifoError                        = ((uint32_t)0x00000080)
};

#define TRANSFER_IT_ENABLE_MASK (uint32_t)(DMA_SxCR_TCIE | DMA_SxCR_HTIE | \
                                           DMA_SxCR_TEIE | DMA_SxCR_DMEIE)

#define HIGH_ISR_MASK           (uint32_t)0x20000000
#define TRANSFER_IT_MASK        (uint32_t)0x0F3C0F3C
#define HIGH_ISR_MASK           (uint32_t)0x20000000
#define RESERVED_MASK           (uint32_t)0x0F7D0F7D

class DMAStream;

class Config {
public:
	Config(Channel channel = Channel::Channel_0) {
		/*-------------- Reset DMA init structure parameters values ----------------*/
		/* Initialize the DMA_Channel member */
		_Channel = (uint32_t)channel;

		/* Initialize the DMA_PeripheralBaseAddr member */
		_PeripheralBaseAddr = 0;

		/* Initialize the DMA_Memory0BaseAddr member */
		_Memory0BaseAddr = 0;

		/* Initialize the DMA_DIR member */
		_DIR = (uint32_t)XferDir::PeripheralToMemory;

		/* Initialize the DMA_BufferSize member */
		_BufferSize = 0;

		/* Initialize the DMA_PeripheralInc member */
		_PeripheralInc = (uint32_t)PeripheralInc::Disable;

		/* Initialize the DMA_MemoryInc member */
		_MemoryInc = (uint32_t)MemoryInc::Disable;

		/* Initialize the DMA_PeripheralDataSize member */
		_PeripheralDataSize = (uint32_t)PeripheralDataSize::Byte;

		/* Initialize the DMA_MemoryDataSize member */
		_MemoryDataSize = (uint32_t)MemoryDataSize::Byte;

		/* Initialize the DMA_Mode member */
		_Mode = (uint32_t)Mode::Normal;

		/* Initialize the DMA_Priority member */
		_Priority = (uint32_t)Prioriy::Low;

		/* Initialize the DMA_FIFOMode member */
		_FIFOMode = (uint32_t)FIFOMode::Disable;

		/* Initialize the DMA_FIFOThreshold member */
		_FIFOThreshold = (uint32_t)FIFOThreshold::OneQuarterFull;

		/* Initialize the DMA_MemoryBurst member */
		_MemoryBurst = (uint32_t)MemoryBurst::Single;

		/* Initialize the DMA_PeripheralBurst member */
		_PeripheralBurst = (uint32_t)PeripheralBurst::Single;
	}

	ALWAYS_INLINE
	Config* channel(Channel ch) {
		_Channel = (uint32_t)ch;
		return this;
	}
	ALWAYS_INLINE
	Config* memory0BaseAddress(uint32_t address) {
		_Memory0BaseAddr = address;
		return this;
	}
	ALWAYS_INLINE
	Config* periphBaseAddress(uint32_t addr) {
		_PeripheralBaseAddr = addr;
		return this;
	}
	ALWAYS_INLINE
	Config* xferDirection(XferDir dir) {
		_DIR = (uint32_t)dir;
		return this;
	}
	ALWAYS_INLINE
	Config* bufferSize(uint16_t size) {
		_BufferSize = size;
		return this;
	}
	ALWAYS_INLINE
	Config* memoryDataSize(MemoryDataSize size) {
		_MemoryDataSize = (uint32_t)size;
		return this;
	}
	ALWAYS_INLINE
	Config* peripheralDataSize(PeripheralDataSize size) {
		_PeripheralDataSize = (uint32_t)size;
		return this;
	}
	ALWAYS_INLINE
	Config* peripheralInc(PeripheralInc mode) {
		_PeripheralInc = (uint32_t)mode;
		return this;
	}
	ALWAYS_INLINE
	Config* memoryInc(MemoryInc mode) {
		_MemoryInc = (uint32_t)mode;
		return this;
	}
	ALWAYS_INLINE
	Config* mode(Mode mode) {
		_Mode = (uint32_t)mode;
		return this;
	}
	ALWAYS_INLINE
	Config* priority(Prioriy prio) {
		_Priority = (uint32_t)prio;
		return this;
	}
	ALWAYS_INLINE
	Config* fifoMode(FIFOMode mode) {
		_FIFOMode = (uint32_t)mode;
		return this;
	}
	ALWAYS_INLINE
	Config* fifoThreshold(FIFOThreshold thresh) {
		_FIFOThreshold = (uint32_t)thresh;
		return this;
	}
	ALWAYS_INLINE
	Config* memoryBurst(MemoryBurst burst) {
		_MemoryBurst = (uint32_t)burst;
		return this;
	}
	ALWAYS_INLINE
	Config* peripheralBurst(PeripheralBurst burst) {
		_PeripheralBurst = (uint32_t)burst;
		return this;
	}

	friend DMAStream;
	///-----------------------///
private:

	uint32_t _Channel; /*!< Specifies the channel used for the specified stream.
	 This parameter can be a value of @ref channel */

	uint32_t _PeripheralBaseAddr; /*!< Specifies the peripheral base address for DMAy Streamx. */

	uint32_t _Memory0BaseAddr; /*!< Specifies the memory 0 base address for DMAy Streamx.
	 This memory is the default memory used when double buffer mode is
	 not enabled. */

	uint32_t _DIR; /*!< Specifies if the data will be transferred from memory to peripheral,
	 from memory to memory or from peripheral to memory.
	 This parameter can be a value of @ref DMA_data_transfer_direction */

	uint32_t _BufferSize; /*!< Specifies the buffer size, in data unit, of the specified Stream.
	 The data unit is equal to the configuration set in DMA_PeripheralDataSize
	 or DMA_MemoryDataSize members depending in the transfer direction. */

	uint32_t _PeripheralInc; /*!< Specifies whether the Peripheral address register should be incremented or not.
	 This parameter can be a value of @ref DMA_peripheral_incremented_mode */

	uint32_t _MemoryInc; /*!< Specifies whether the memory address register should be incremented or not.
	 This parameter can be a value of @ref DMA_memory_incremented_mode */

	uint32_t _PeripheralDataSize; /*!< Specifies the Peripheral data width.
	 This parameter can be a value of @ref DMA_peripheral_data_size */

	uint32_t _MemoryDataSize; /*!< Specifies the Memory data width.
	 This parameter can be a value of @ref DMA_memory_data_size */

	uint32_t _Mode; /*!< Specifies the operation mode of the DMAy Streamx.
	 This parameter can be a value of @ref DMA_circular_normal_mode
	 @note The circular buffer mode cannot be used if the memory-to-memory
	 data transfer is configured on the selected Stream */

	uint32_t _Priority; /*!< Specifies the software priority for the DMAy Streamx.
	 This parameter can be a value of @ref DMA_priority_level */

	uint32_t _FIFOMode; /*!< Specifies if the FIFO mode or Direct mode will be used for the specified Stream.
	 This parameter can be a value of @ref fifo_direct_mode
	 @note The Direct mode (FIFO mode disabled) cannot be used if the
	 memory-to-memory data transfer is configured on the selected Stream */

	uint32_t _FIFOThreshold; /*!< Specifies the FIFO threshold level.
	 This parameter can be a value of @ref DMA_fifo_threshold_level */

	uint32_t _MemoryBurst; /*!< Specifies the Burst transfer configuration for the memory transfers.
	 It specifies the amount of data to be transferred in a single non interruptable
	 transaction. This parameter can be a value of @ref DMA_memory_burst
	 @note The burst mode is possible only if the address Increment mode is enabled. */

	uint32_t _PeripheralBurst; /*!< Specifies the Burst transfer configuration for the peripheral transfers.
	 It specifies the amount of data to be transferred in a single non interruptable
	 transaction. This parameter can be a value of @ref DMA_peripheral_burst
	 @note The burst mode is possible only if the address Increment mode is enabled. */
};

class DMAStream {

public:
	DMAStream(Stream stream) {
		DMAy_Streamx = (DMA_Stream_TypeDef*) stream;

		uint8_t id = getStreamIndex((uint32_t)DMAy_Streamx);

		if(id == 255) return;

		if(streams[id] == 0) {
			streams[id] = this;
		} else {
			return;
		}

		NVIC_EnableIRQ((IRQn_Type)getIRQn());
	}

	~DMAStream() {
		uint8_t id = getStreamIndex((uint32_t)DMAy_Streamx);

		if(id == 255) return;

		if(streams[id] == this) {
			streams[id] = 0;
		}

		NVIC_DisableIRQ((IRQn_Type)getIRQn());
	}

	void init(Config &cfg) {
		uint32_t tmpreg = 0;
		/*------------------------- DMAy Streamx CR Configuration ------------------*/
		/* Get the DMAy_Streamx CR value */
		tmpreg = DMAy_Streamx->CR;

		/* Clear CHSEL, MBURST, PBURST, PL, MSIZE, PSIZE, MINC, PINC, CIRC and DIR bits */
		tmpreg &= ((uint32_t) ~(DMA_SxCR_CHSEL | DMA_SxCR_MBURST
				| DMA_SxCR_PBURST |
				DMA_SxCR_PL | DMA_SxCR_MSIZE | DMA_SxCR_PSIZE |
				DMA_SxCR_MINC | DMA_SxCR_PINC | DMA_SxCR_CIRC |
				DMA_SxCR_DIR));

		/* Configure DMAy Streamx: */
		/* Set CHSEL bits according to DMA_CHSEL value */
		/* Set DIR bits according to DMA_DIR value */
		/* Set PINC bit according to DMA_PeripheralInc value */
		/* Set MINC bit according to DMA_MemoryInc value */
		/* Set PSIZE bits according to DMA_PeripheralDataSize value */
		/* Set MSIZE bits according to DMA_MemoryDataSize value */
		/* Set CIRC bit according to DMA_Mode value */
		/* Set PL bits according to DMA_Priority value */
		/* Set MBURST bits according to DMA_MemoryBurst value */
		/* Set PBURST bits according to DMA_PeripheralBurst value */
		tmpreg |= cfg._Channel | cfg._DIR | cfg._PeripheralInc | cfg._MemoryInc
				| cfg._PeripheralDataSize | cfg._MemoryDataSize | cfg._Mode
				| cfg._Priority | cfg._MemoryBurst | cfg._PeripheralBurst;

		/* Write to DMAy Streamx CR register */
		DMAy_Streamx->CR = tmpreg;

		/*------------------------- DMAy Streamx FCR Configuration -----------------*/
		/* Get the DMAy_Streamx FCR value */
		tmpreg = DMAy_Streamx->FCR;

		/* Clear DMDIS and FTH bits */
		tmpreg &= (uint32_t) ~(DMA_SxFCR_DMDIS | DMA_SxFCR_FTH);

		/* Configure DMAy Streamx FIFO:
		 Set DMDIS bits according to DMA_FIFOMode value
		 Set FTH bits according to DMA_FIFOThreshold value */
		tmpreg |= cfg._FIFOMode | cfg._FIFOThreshold;

		/* Write to DMAy Streamx CR */
		DMAy_Streamx->FCR = tmpreg;

		/*------------------------- DMAy Streamx NDTR Configuration ----------------*/
		/* Write to DMAy Streamx NDTR register */
		DMAy_Streamx->NDTR = cfg._BufferSize;

		/*------------------------- DMAy Streamx PAR Configuration -----------------*/
		/* Write to DMAy Streamx PAR */
		DMAy_Streamx->PAR = cfg._PeripheralBaseAddr;

		/*------------------------- DMAy Streamx M0AR Configuration ----------------*/
		/* Write to DMAy Streamx M0AR */
		DMAy_Streamx->M0AR = cfg._Memory0BaseAddr;
	}

	void enable(bool enable = true) {
		if (enable) {
			/* Enable the selected DMAy Streamx by setting EN bit */
			DMAy_Streamx->CR |= (uint32_t) DMA_SxCR_EN;
		} else {
			/* Disable the selected DMAy Streamx by clearing EN bit */
			DMAy_Streamx->CR &= ~(uint32_t) DMA_SxCR_EN;
		}
	}

	void periphIncOffsetSizeConfig(PINCOS DMA_Pincos) {

		/* Check the needed Peripheral increment offset */
		if (DMA_Pincos != PINCOS::Psize) {
			/* Configure DMA_SxCR_PINCOS bit with the input parameter */
			DMAy_Streamx->CR |= (uint32_t) DMA_SxCR_PINCOS;
		} else {
			/* Clear the PINCOS bit: Peripheral address incremented according to PSIZE */
			DMAy_Streamx->CR &= ~(uint32_t) DMA_SxCR_PINCOS;
		}
	}

	/**
	 * @brief  Configures, when the DMAy Streamx is disabled, the flow controller for
	 *         the next transactions (Peripheral or Memory).
	 *
	 * @note   Before enabling this feature, check if the used peripheral supports
	 *         the Flow Controller mode or not.
	 *
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 * @param  DMA_FlowCtrl: specifies the DMA flow controller.
	 *          This parameter can be one of the following values:
	 *            @arg DMA_FlowCtrl_Memory: DMAy_Streamx transactions flow controller is
	 *                                      the DMA controller.
	 *            @arg DMA_FlowCtrl_Peripheral: DMAy_Streamx transactions flow controller
	 *                                          is the peripheral.
	 * @retval None
	 */
	inline void flowControllerConfig(FlowControl DMA_FlowCtrl) {

		/* Check the needed flow controller  */
		if (DMA_FlowCtrl != FlowControl::Memory) {
			/* Configure DMA_SxCR_PFCTRL bit with the input parameter */
			DMAy_Streamx->CR |= (uint32_t) DMA_SxCR_PFCTRL;
		} else {
			/* Clear the PFCTRL bit: Memory is the flow controller */
			DMAy_Streamx->CR &= ~(uint32_t) DMA_SxCR_PFCTRL;
		}
	}
	/**
	 * @}
	 */

	/** @defgroup DMA_Group2 Data Counter functions
	 *  @brief   Data Counter functions
	 *
	 @verbatim
	 ===============================================================================
	 Data Counter functions
	 ===============================================================================

	 This subsection provides function allowing to configure and read the buffer size
	 (number of data to be transferred).

	 The DMA data counter can be written only when the DMA Stream is disabled
	 (ie. after transfer complete event).

	 The following function can be used to write the Stream data counter value:
	 - void DMA_SetCurrDataCounter(uint16_t Counter);

	 @note It is advised to use this function rather than DMA_Init() in situations where
	 only the Data buffer needs to be reloaded.

	 @note If the Source and Destination Data Sizes are different, then the value written in
	 data counter, expressing the number of transfers, is relative to the number of
	 transfers from the Peripheral point of view.
	 ie. If Memory data size is Word, Peripheral data size is Half-Words, then the value
	 to be configured in the data counter is the number of Half-Words to be transferred
	 from/to the peripheral.

	 The DMA data counter can be read to indicate the number of remaining transfers for
	 the relative DMA Stream. This counter is decremented at the end of each data
	 transfer and when the transfer is complete:
	 - If Normal mode is selected: the counter is set to 0.
	 - If Circular mode is selected: the counter is reloaded with the initial value
	 (configured before enabling the DMA Stream)

	 The following function can be used to read the Stream data counter value:
	 - uint16_t DMA_GetCurrDataCounter();

	 @endverbatim
	 * @{
	 */

	/**
	 * @brief  Writes the number of data units to be transferred on the DMAy Streamx.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 * @param  Counter: Number of data units to be transferred (from 0 to 65535)
	 *          Number of data items depends only on the Peripheral data format.
	 *
	 * @note   If Peripheral data format is Bytes: number of data units is equal
	 *         to total number of bytes to be transferred.
	 *
	 * @note   If Peripheral data format is Half-Word: number of data units is
	 *         equal to total number of bytes to be transferred / 2.
	 *
	 * @note   If Peripheral data format is Word: number of data units is equal
	 *         to total  number of bytes to be transferred / 4.
	 *
	 * @note   In Memory-to-Memory transfer mode, the memory buffer pointed by
	 *         DMAy_SxPAR register is considered as Peripheral.
	 *
	 * @retval The number of remaining data units in the current DMAy Streamx transfer.
	 */
	inline void setCurrDataCounter(uint16_t Counter) {
		/* Write the number of data units to be transferred */
		DMAy_Streamx->NDTR = (uint16_t) Counter;
	}

	/**
	 * @brief  Returns the number of remaining data units in the current DMAy Streamx transfer.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 * @retval The number of remaining data units in the current DMAy Streamx transfer.
	 */
	inline uint16_t getCurrDataCounter() {

		/* Return the number of remaining data units for DMAy Streamx */
		return ((uint16_t) (DMAy_Streamx->NDTR));
	}
	/**
	 * @}
	 */

	/** @defgroup DMA_Group3 Double Buffer mode functions
	 *  @brief   Double Buffer mode functions
	 *
	 @verbatim
	 ===============================================================================
	 Double Buffer mode functions
	 ===============================================================================

	 This subsection provides function allowing to configure and control the double
	 buffer mode parameters.

	 The Double Buffer mode can be used only when Circular mode is enabled.
	 The Double Buffer mode cannot be used when transferring data from Memory to Memory.

	 The Double Buffer mode allows to set two different Memory addresses from/to which
	 the DMA controller will access alternatively (after completing transfer to/from target
	 memory 0, it will start transfer to/from target memory 1).
	 This allows to reduce software overhead for double buffering and reduce the CPU
	 access time.

	 Two functions must be called before calling the DMA_Init() function:
	 - void DMA_DoubleBufferModeConfig(uint32_t Memory1BaseAddr,
	 uint32_t DMA_CurrentMemory);
	 - void DMA_DoubleBufferModeCmd(FunctionalState NewState);

	 DMA_DoubleBufferModeConfig() is called to configure the Memory 1 base address and the first
	 Memory target from/to which the transfer will start after enabling the DMA Stream.
	 Then DMA_DoubleBufferModeCmd() must be called to enable the Double Buffer mode (or disable
	 it when it should not be used).


	 Two functions can be called dynamically when the transfer is ongoing (or when the DMA Stream is
	 stopped) to modify on of the target Memories addresses or to check wich Memory target is currently
	 used:
	 - void DMA_MemoryTargetConfig(uint32_t MemoryBaseAddr,
	 uint32_t DMA_MemoryTarget);
	 - uint32_t DMA_GetCurrentMemoryTarget();

	 DMA_MemoryTargetConfig() can be called to modify the base address of one of the two target Memories.
	 The Memory of which the base address will be modified must not be currently be used by the DMA Stream
	 (ie. if the DMA Stream is currently transferring from Memory 1 then you can only modify base address
	 of target Memory 0 and vice versa).
	 To check this condition, it is recommended to use the function DMA_GetCurrentMemoryTarget() which
	 returns the index of the Memory target currently in use by the DMA Stream.

	 @endverbatim
	 * @{
	 */

	/**
	 * @brief  Configures, when the DMAy Streamx is disabled, the double buffer mode
	 *         and the current memory target.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 * @param  Memory1BaseAddr: the base address of the second buffer (Memory 1)
	 * @param  DMA_CurrentMemory: specifies which memory will be first buffer for
	 *         the transactions when the Stream will be enabled.
	 *          This parameter can be one of the following values:
	 *            @arg DMA_Memory_0: Memory 0 is the current buffer.
	 *            @arg DMA_Memory_1: Memory 1 is the current buffer.
	 *
	 * @note   Memory0BaseAddr is set by the DMA structure configuration in DMA_Init().
	 *
	 * @retval None
	 */
	inline void doubleBufferModeConfig(uint32_t Memory1BaseAddr,
			Memory DMA_CurrentMemory) {

		if (DMA_CurrentMemory != Memory::Memory_0) {
			/* Set Memory 1 as current memory address */
			DMAy_Streamx->CR |= (uint32_t) (DMA_SxCR_CT);
		} else {
			/* Set Memory 0 as current memory address */
			DMAy_Streamx->CR &= ~(uint32_t) (DMA_SxCR_CT);
		}

		/* Write to DMAy Streamx M1AR */
		DMAy_Streamx->M1AR = Memory1BaseAddr;
	}

	/**
	 * @brief  Enables or disables the double buffer mode for the selected DMA stream.
	 * @note   This function can be called only when the DMA Stream is disabled.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 * @param  NewState: new state of the DMAy Streamx double buffer mode.
	 *          This parameter can be: ENABLE or DISABLE.
	 * @retval None
	 */
	inline void doubleBufferModeCmd(bool NewState) {
		/* Configure the Double Buffer mode */
		if (NewState) {
			/* Enable the Double buffer mode */
			DMAy_Streamx->CR |= (uint32_t) DMA_SxCR_DBM;
		} else {
			/* Disable the Double buffer mode */
			DMAy_Streamx->CR &= ~(uint32_t) DMA_SxCR_DBM;
		}
	}

	/**
	 * @brief  Configures the Memory address for the next buffer transfer in double
	 *         buffer mode (for dynamic use). This function can be called when the
	 *         DMA Stream is enabled and when the transfer is ongoing.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 * @param  MemoryBaseAddr: The base address of the target memory buffer
	 * @param  DMA_MemoryTarget: Next memory target to be used.
	 *         This parameter can be one of the following values:
	 *            @arg DMA_Memory_0: To use the memory address 0
	 *            @arg DMA_Memory_1: To use the memory address 1
	 *
	 * @note    It is not allowed to modify the Base Address of a target Memory when
	 *          this target is involved in the current transfer. ie. If the DMA Stream
	 *          is currently transferring to/from Memory 1, then it not possible to
	 *          modify Base address of Memory 1, but it is possible to modify Base
	 *          address of Memory 0.
	 *          To know which Memory is currently used, you can use the function
	 *          DMA_GetCurrentMemoryTarget().
	 *
	 * @retval None
	 */
	inline void memoryTargetConfig(uint32_t MemoryBaseAddr,
			Memory DMA_MemoryTarget) {

		/* Check the Memory target to be configured */
		if (DMA_MemoryTarget != Memory::Memory_0) {
			/* Write to DMAy Streamx M1AR */
			DMAy_Streamx->M1AR = MemoryBaseAddr;
		} else {
			/* Write to DMAy Streamx M0AR */
			DMAy_Streamx->M0AR = MemoryBaseAddr;
		}
	}

	/**
	 * @brief  Returns the current memory target used by double buffer transfer.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 * @retval The memory target number: 0 for Memory0 or 1 for Memory1.
	 */
	inline Memory getCurrentMemoryTarget() {
		/* Get the current memory target */
		if ((DMAy_Streamx->CR & DMA_SxCR_CT) != 0) {
			/* Current memory buffer used is Memory 1 */
			return Memory::Memory_1;
		} else {
			/* Current memory buffer used is Memory 0 */
			return Memory::Memory_0;
		}
	}
	/**
	 * @}
	 */

	/** @defgroup DMA_Group4 Interrupts and flags management functions
	 *  @brief   Interrupts and flags management functions
	 *
	 @verbatim
	 ===============================================================================
	 Interrupts and flags management functions
	 ===============================================================================

	 This subsection provides functions allowing to
	 - Check the DMA enable status
	 - Check the FIFO status
	 - Configure the DMA Interrupts sources and check or clear the flags or pending bits status.

	 1. DMA Enable status:
	 After configuring the DMA Stream (DMA_Init() function) and enabling the stream,
	 it is recommended to check (or wait until) the DMA Stream is effectively enabled.
	 A Stream may remain disabled if a configuration parameter is wrong.
	 After disabling a DMA Stream, it is also recommended to check (or wait until) the DMA
	 Stream is effectively disabled. If a Stream is disabled while a data transfer is ongoing,
	 the current data will be transferred and the Stream will be effectively disabled only after
	 this data transfer completion.
	 To monitor this state it is possible to use the following function:
	 - FunctionalState DMA_GetCmdStatus();

	 2. FIFO Status:
	 It is possible to monitor the FIFO status when a transfer is ongoing using the following
	 function:
	 - uint32_t DMA_GetFIFOStatus();

	 3. DMA Interrupts and Flags:
	 The user should identify which mode will be used in his application to manage the
	 DMA controller events: Polling mode or Interrupt mode.

	 Polling Mode
	 =============
	 Each DMA stream can be managed through 4 event Flags:
	 (x : DMA Stream number )
	 1. DMA_FLAG_FEIFx  : to indicate that a FIFO Mode Transfer Error event occurred.
	 2. DMA_FLAG_DMEIFx : to indicate that a Direct Mode Transfer Error event occurred.
	 3. DMA_FLAG_TEIFx  : to indicate that a Transfer Error event occurred.
	 4. DMA_FLAG_HTIFx  : to indicate that a Half-Transfer Complete event occurred.
	 5. DMA_FLAG_TCIFx  : to indicate that a Transfer Complete event occurred .

	 In this Mode it is advised to use the following functions:
	 - FlagStatus DMA_GetFlagStatus(uint32_t DMA_FLAG);
	 - void DMA_ClearFlag(uint32_t DMA_FLAG);

	 Interrupt Mode
	 ===============
	 Each DMA Stream can be managed through 4 Interrupts:

	 Interrupt Source
	 ----------------
	 1. DMA_IT_FEIFx  : specifies the interrupt source for the  FIFO Mode Transfer Error event.
	 2. DMA_IT_DMEIFx : specifies the interrupt source for the Direct Mode Transfer Error event.
	 3. DMA_IT_TEIFx  : specifies the interrupt source for the Transfer Error event.
	 4. DMA_IT_HTIFx  : specifies the interrupt source for the Half-Transfer Complete event.
	 5. DMA_IT_TCIFx  : specifies the interrupt source for the a Transfer Complete event.

	 In this Mode it is advised to use the following functions:
	 - void DMA_ITConfig(uint32_t DMA_IT, FunctionalState NewState);
	 - ITStatus DMA_GetITStatus(uint32_t DMA_IT);
	 - void DMA_ClearITPendingBit(uint32_t DMA_IT);

	 @endverbatim
	 * @{
	 */

	/**
	 * @brief  Returns the status of EN bit for the specified DMAy Streamx.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 *
	 * @note    After configuring the DMA Stream (DMA_Init() function) and enabling
	 *          the stream, it is recommended to check (or wait until) the DMA Stream
	 *          is effectively enabled. A Stream may remain disabled if a configuration
	 *          parameter is wrong.
	 *          After disabling a DMA Stream, it is also recommended to check (or wait
	 *          until) the DMA Stream is effectively disabled. If a Stream is disabled
	 *          while a data transfer is ongoing, the current data will be transferred
	 *          and the Stream will be effectively disabled only after the transfer
	 *          of this single data is finished.
	 *
	 * @retval Current state of the DMAy Streamx (ENABLE or DISABLE).
	 */
	inline bool getCmdStatus() {
		if ((DMAy_Streamx->CR & (uint32_t) DMA_SxCR_EN) != 0) {
			/* The selected DMAy Streamx EN bit is set (DMA is still transferring) */
			return 1;
		} else {
			/* The selected DMAy Streamx EN bit is cleared (DMA is disabled and
			 all transfers are complete) */
			return 0;
		}
	}

	/**
	 * @brief  Returns the current DMAy Streamx FIFO filled level.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *         to 7 to select the DMA Stream.
	 * @retval The FIFO filling state.
	 *           - DMA_FIFOStatus_Less1QuarterFull: when FIFO is less than 1 quarter-full
	 *                                               and not empty.
	 *           - DMA_FIFOStatus_1QuarterFull: if more than 1 quarter-full.
	 *           - DMA_FIFOStatus_HalfFull: if more than 1 half-full.
	 *           - DMA_FIFOStatus_3QuartersFull: if more than 3 quarters-full.
	 *           - DMA_FIFOStatus_Empty: when FIFO is empty
	 *           - DMA_FIFOStatus_Full: when FIFO is full
	 */
	inline FIFOStatus getFIFOStatus() {
		uint32_t tmpreg = 0;

		/* Get the FIFO level bits */
		tmpreg = (uint32_t) ((DMAy_Streamx->FCR & DMA_SxFCR_FS));

		return (FIFOStatus)tmpreg;
	}

	//convenience function to check if transfer is complete
	inline bool isComplete() {
		return getInterruptStatus(Interrupt::TransferComplete);
	}

	inline bool isError() {
		return getInterruptStatus(Interrupt::TransferError);
	}

	/**
	 * @brief  Checks whether the specified DMAy Streamx flag is set or not.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 * @param  DMA_FLAG: specifies the flag to check.
	 *          This parameter can be one of the following values:
	 *            @arg DMA_FLAG_TCIFx:  Streamx transfer complete flag
	 *            @arg DMA_FLAG_HTIFx:  Streamx half transfer complete flag
	 *            @arg DMA_FLAG_TEIFx:  Streamx transfer error flag
	 *            @arg DMA_FLAG_DMEIFx: Streamx direct mode error flag
	 *            @arg DMA_FLAG_FEIFx:  Streamx FIFO error flag
	 *         Where x can be 0 to 7 to select the DMA Stream.
	 * @retval The new state of DMA_FLAG (SET or RESET).
	 */
	inline FlagStatus getFlagStatus(IntFlag DMA_FLAG) {
		FlagStatus bitstatus = RESET;
		DMA_TypeDef* DMAy;
		uint32_t tmpreg = 0;

		/* Determine the DMA to which belongs the stream */
		if (DMAy_Streamx < DMA2_Stream0) {
			/* DMAy_Streamx belongs to DMA1 */
			DMAy = DMA1;
		} else {
			/* DMAy_Streamx belongs to DMA2 */
			DMAy = DMA2;
		}

		/* Check if the flag is in HISR or LISR */
		if (((uint32_t)DMA_FLAG & HIGH_ISR_MASK)) {
			/* Get DMAy HISR register value */
			tmpreg = DMAy->HISR;
		} else {
			/* Get DMAy LISR register value */
			tmpreg = DMAy->LISR;
		}

		/* Mask the reserved bits */
		tmpreg &= (uint32_t) RESERVED_MASK;

		/* Check the status of the specified DMA flag */
		if ((tmpreg & (uint32_t)DMA_FLAG)) {
			/* DMA_FLAG is set */
			bitstatus = SET;
		} else {
			/* DMA_FLAG is reset */
			bitstatus = RESET;
		}

		/* Return the DMA_FLAG status */
		return bitstatus;
	}

	/**
	 * @brief  Clears the DMAy Streamx's pending flags.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 * @param  DMA_FLAG: specifies the flag to clear.
	 *          This parameter can be any combination of the following values:
	 *            @arg DMA_FLAG_TCIFx:  Streamx transfer complete flag
	 *            @arg DMA_FLAG_HTIFx:  Streamx half transfer complete flag
	 *            @arg DMA_FLAG_TEIFx:  Streamx transfer error flag
	 *            @arg DMA_FLAG_DMEIFx: Streamx direct mode error flag
	 *            @arg DMA_FLAG_FEIFx:  Streamx FIFO error flag
	 *         Where x can be 0 to 7 to select the DMA Stream.
	 * @retval None
	 */
	inline void clearFlag(IntFlag DMA_FLAG) {
		DMA_TypeDef* DMAy;

		/* Determine the DMA to which belongs the stream */
		if (DMAy_Streamx < DMA2_Stream0) {
			/* DMAy_Streamx belongs to DMA1 */
			DMAy = DMA1;
		} else {
			/* DMAy_Streamx belongs to DMA2 */
			DMAy = DMA2;
		}

		/* Check if LIFCR or HIFCR register is targeted */
		if (((uint32_t)DMA_FLAG & HIGH_ISR_MASK) != (uint32_t) RESET) {
			/* Set DMAy HIFCR register clear flag bits */
			DMAy->HIFCR = (uint32_t) ((uint32_t)DMA_FLAG & RESERVED_MASK);
		} else {
			/* Set DMAy LIFCR register clear flag bits */
			DMAy->LIFCR = (uint32_t) ((uint32_t)DMA_FLAG & RESERVED_MASK);
		}
	}

	/**
	 * @brief  Enables or disables the specified DMAy Streamx interrupts.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 * @param DMA_IT: specifies the DMA interrupt sources to be enabled or disabled.
	 *          This parameter can be any combination of the following values:
	 *            @arg DMA_IT_TC:  Transfer complete interrupt mask
	 *            @arg DMA_IT_HT:  Half transfer complete interrupt mask
	 *            @arg DMA_IT_TE:  Transfer error interrupt mask
	 *            @arg DMA_IT_FE:  FIFO error interrupt mask
	 * @param  NewState: new state of the specified DMA interrupts.
	 *          This parameter can be: ENABLE or DISABLE.
	 * @retval None
	 */
	inline void interruptConfig(Interrupt DMA_IT, bool NewState) {

		/* Check if the DMA_IT parameter contains a FIFO interrupt */
		if (((uint32_t)DMA_IT & (uint32_t)Interrupt::FifoError) != 0) {
			if (NewState) {
				/* Enable the selected DMA FIFO interrupts */
				DMAy_Streamx->FCR |= (uint32_t) Interrupt::FifoError;
			} else {
				/* Disable the selected DMA FIFO interrupts */
				DMAy_Streamx->FCR &= ~(uint32_t) Interrupt::FifoError;
			}
		}

		/* Check if the DMA_IT parameter contains a Transfer interrupt */
		if (DMA_IT != Interrupt::FifoError) {
			if (NewState) {
				/* Enable the selected DMA transfer interrupts */
				DMAy_Streamx->CR |=
						(uint32_t) ((uint32_t)DMA_IT & TRANSFER_IT_ENABLE_MASK);
			} else {
				/* Disable the selected DMA transfer interrupts */
				DMAy_Streamx->CR &= ~(uint32_t) ((uint32_t)DMA_IT
						& TRANSFER_IT_ENABLE_MASK);
			}
		}

	}

	/**
	 * @brief  Checks whether the specified DMAy Streamx interrupt has occurred or not.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 * @param  DMA_IT: specifies the DMA interrupt source to check.
	 *          This parameter can be one of the following values:
	 *            @arg DMA_IT_TCIFx:  Streamx transfer complete interrupt
	 *            @arg DMA_IT_HTIFx:  Streamx half transfer complete interrupt
	 *            @arg DMA_IT_TEIFx:  Streamx transfer error interrupt
	 *            @arg DMA_IT_DMEIFx: Streamx direct mode error interrupt
	 *            @arg DMA_IT_FEIFx:  Streamx FIFO error interrupt
	 *         Where x can be 0 to 7 to select the DMA Stream.
	 * @retval The new state of DMA_IT (SET or RESET).
	 */
	bool getInterruptStatus(Interrupt DMA_IT) {
		bool bitstatus = RESET;
		DMA_TypeDef* DMAy;
		uint32_t tmpreg = 0, enablestatus = 0;

		/* Determine the DMA to which belongs the stream */
		if (DMAy_Streamx < DMA2_Stream0) {
			/* DMAy_Streamx belongs to DMA1 */
			DMAy = DMA1;
		} else {
			/* DMAy_Streamx belongs to DMA2 */
			DMAy = DMA2;
		}

		/* Check if the interrupt enable bit is in the CR or FCR register */
		if (((uint32_t)DMA_IT & TRANSFER_IT_MASK) != (uint32_t) RESET) {
			/* Get the interrupt enable position mask in CR register */
			tmpreg = (uint32_t) (((uint32_t)DMA_IT >> 11) & TRANSFER_IT_ENABLE_MASK);

			/* Check the enable bit in CR register */
			enablestatus = (uint32_t) (DMAy_Streamx->CR & tmpreg);
		} else {
			/* Check the enable bit in FCR register */
			enablestatus = (uint32_t) (DMAy_Streamx->FCR & (uint32_t)Interrupt::FifoError);
		}

		/* Check if the interrupt pending flag is in LISR or HISR */
		if (((uint32_t)DMA_IT & HIGH_ISR_MASK) != (uint32_t) RESET) {
			/* Get DMAy HISR register value */
			tmpreg = DMAy->HISR;
		} else {
			/* Get DMAy LISR register value */
			tmpreg = DMAy->LISR;
		}

		/* mask all reserved bits */
		tmpreg &= (uint32_t) RESERVED_MASK;

		/* Check the status of the specified DMA interrupt */
		if (((tmpreg & (uint32_t)DMA_IT) != (uint32_t) RESET)
				&& (enablestatus != (uint32_t) RESET)) {
			/* DMA_IT is set */
			bitstatus = SET;
		} else {
			/* DMA_IT is reset */
			bitstatus = RESET;
		}

		/* Return the DMA_IT status */
		return bitstatus;
	}

	/**
	 * @brief  Clears the DMAy Streamx's interrupt pending bits.
	 * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
	 *          to 7 to select the DMA Stream.
	 * @param  DMA_IT: specifies the DMA interrupt pending bit to clear.
	 *          This parameter can be any combination of the following values:
	 *            @arg DMA_IT_TCIFx:  Streamx transfer complete interrupt
	 *            @arg DMA_IT_HTIFx:  Streamx half transfer complete interrupt
	 *            @arg DMA_IT_TEIFx:  Streamx transfer error interrupt
	 *            @arg DMA_IT_DMEIFx: Streamx direct mode error interrupt
	 *            @arg DMA_IT_FEIFx:  Streamx FIFO error interrupt
	 *         Where x can be 0 to 7 to select the DMA Stream.
	 * @retval None
	 */
	void clearInterruptPendingBit(Interrupt DMA_IT) {
		DMA_TypeDef* DMAy;

		/* Determine the DMA to which belongs the stream */
		if (DMAy_Streamx < DMA2_Stream0) {
			/* DMAy_Streamx belongs to DMA1 */
			DMAy = DMA1;
		} else {
			/* DMAy_Streamx belongs to DMA2 */
			DMAy = DMA2;
		}

		/* Check if LIFCR or HIFCR register is targeted */
		if (((uint32_t)DMA_IT & HIGH_ISR_MASK) != (uint32_t) RESET) {
			/* Set DMAy HIFCR register clear interrupt bits */
			DMAy->HIFCR = (uint32_t) ((uint32_t)DMA_IT & RESERVED_MASK);
		} else {
			/* Set DMAy LIFCR register clear interrupt bits */
			DMAy->LIFCR = (uint32_t) ((uint32_t)DMA_IT & RESERVED_MASK);
		}
	}

	void handleIRQ() {
		if(callback) {
			callback(this);
		}
		if(getInterruptStatus(Interrupt::TransferComplete)) {
			clearInterruptPendingBit(Interrupt::TransferComplete);
		} else
		if(getInterruptStatus(Interrupt::TransferError)) {
			clearInterruptPendingBit(Interrupt::TransferError);
		}
	}

	bool attachCallback(xpcc::function<void(DMAStream*)> fn) {
		interruptConfig(Interrupt::TransferComplete, ENABLE);
		interruptConfig(Interrupt::TransferError, ENABLE);
		callback = fn;
		return true;
	}

	static DMAStream* streams[];
	xpcc::function<void(DMAStream*)> callback;

private:
	uint8_t getStreamIndex(uint32_t stream) {
		switch((Stream)stream) {
		case Stream::DMA1_0:
			return 0;
		case Stream::DMA1_1:
			return 1;
		case Stream::DMA1_2:
			return 2;
		case Stream::DMA1_3:
			return 3;
		case Stream::DMA1_4:
			return 4;
		case Stream::DMA1_5:
			return 5;
		case Stream::DMA1_6:
			return 6;
		case Stream::DMA1_7:
			return 7;
		case Stream::DMA2_0:
			return 8;
		case Stream::DMA2_1:
			return 9;
		case Stream::DMA2_2:
			return 10;
		case Stream::DMA2_3:
			return 11;
		case Stream::DMA2_4:
			return 12;
		case Stream::DMA2_5:
			return 13;
		case Stream::DMA2_6:
			return 14;
		case Stream::DMA2_7:
			return 15;
		default:
			return 255;
		}
	}

	/// get irqn of current stream
	uint8_t getIRQn() {
		uint32_t stream = reinterpret_cast<uint32_t>(DMAy_Streamx);
		switch((Stream)stream) {
		case Stream::DMA1_0:
			return DMA1_Stream0_IRQn;
		case Stream::DMA1_1:
			return DMA1_Stream1_IRQn;
		case Stream::DMA1_2:
			return DMA1_Stream2_IRQn;
		case Stream::DMA1_3:
			return DMA1_Stream3_IRQn;
		case Stream::DMA1_4:
			return DMA1_Stream4_IRQn;
		case Stream::DMA1_5:
			return DMA1_Stream5_IRQn;
		case Stream::DMA1_6:
			return DMA1_Stream6_IRQn;
		case Stream::DMA1_7:
			return DMA1_Stream7_IRQn;
		case Stream::DMA2_0:
			return DMA2_Stream0_IRQn;
		case Stream::DMA2_1:
			return DMA2_Stream1_IRQn;
		case Stream::DMA2_2:
			return DMA2_Stream2_IRQn;
		case Stream::DMA2_3:
			return DMA2_Stream3_IRQn;
		case Stream::DMA2_4:
			return DMA2_Stream4_IRQn;
		case Stream::DMA2_5:
			return DMA2_Stream5_IRQn;
		case Stream::DMA2_6:
			return DMA2_Stream6_IRQn;
		case Stream::DMA2_7:
			return DMA2_Stream7_IRQn;
		default:
			return 255;
		}
	}


	DMA_Stream_TypeDef* DMAy_Streamx;
};

}
}
}

#endif /* SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_DMA_DMA_CHANNEL_HPP_ */
