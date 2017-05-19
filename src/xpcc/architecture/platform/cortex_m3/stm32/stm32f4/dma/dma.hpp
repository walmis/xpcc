/*
 * channel.hpp
 *
 *  Created on: Apr 21, 2015
 *      Author: walmis
 */

#pragma once

#include <xpcc/architecture/utils.hpp>
#include "../../device.hpp"
#include <xpcc/processing/function.hpp>
#include <xpcc/debug.hpp>
namespace xpcc {
namespace stm32 {
namespace dma {

enum class Stream {
	DMA1_0 = static_cast<uint32_t>( DMA1_Stream0_BASE),
	DMA1_1 = static_cast<uint32_t>( DMA1_Stream1_BASE),
	DMA1_2 = static_cast<uint32_t>( DMA1_Stream2_BASE),
	DMA1_3 = static_cast<uint32_t>( DMA1_Stream3_BASE),
	DMA1_4 = static_cast<uint32_t>( DMA1_Stream4_BASE),
	DMA1_5 = static_cast<uint32_t>( DMA1_Stream5_BASE),
	DMA1_6 = static_cast<uint32_t>( DMA1_Stream6_BASE),
	DMA1_7 = static_cast<uint32_t>( DMA1_Stream7_BASE),
	DMA2_0 = static_cast<uint32_t>( DMA2_Stream0_BASE),
	DMA2_1 = static_cast<uint32_t>( DMA2_Stream1_BASE),
	DMA2_2 = static_cast<uint32_t>( DMA2_Stream2_BASE),
	DMA2_3 = static_cast<uint32_t>( DMA2_Stream3_BASE),
	DMA2_4 = static_cast<uint32_t>( DMA2_Stream4_BASE),
	DMA2_5 = static_cast<uint32_t>( DMA2_Stream5_BASE),
	DMA2_6 = static_cast<uint32_t>( DMA2_Stream6_BASE),
	DMA2_7 = static_cast<uint32_t>( DMA2_Stream7_BASE),
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
	TransferComplete                        = ((uint32_t)0x00000020),
	HalfTransfer                        = ((uint32_t)0x000000010),
	TransferError                        = ((uint32_t)0x00000008),
	DirectModeError                       = ((uint32_t)0x00000004),
	FifoError                        = ((uint32_t)0x0000001),
	All = TransferComplete | HalfTransfer | TransferError | DirectModeError | FifoError
};


enum class Interrupt {
	TransferComplete                        = ((uint32_t)0x00000010),
	HalfTransfer                        = ((uint32_t)0x00000008),
	TransferError                        = ((uint32_t)0x00000004),
	DirectModeError                       = ((uint32_t)0x00000002),
	FifoError                        = ((uint32_t)0x00000080)
};

ENUM_CLASS_FLAG(Interrupt);
ENUM_CLASS_FLAG(IntFlag);

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

		CR_flags = 0;
		FCR_flags = 0x21; //reset value

		/* Initialize the DMA_PeripheralBaseAddr member */
		_PeripheralBaseAddr = 0;

		/* Initialize the DMA_Memory0BaseAddr member */
		_Memory0BaseAddr = 0;

		/* Initialize the DMA_BufferSize member */
		_BufferSize = 0;

	}

	ALWAYS_INLINE
	Config* channel(Channel ch) {
		CR_flags &= ((uint32_t) ~(DMA_SxCR_CHSEL));
		CR_flags |= (uint32_t)ch;
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
		CR_flags &= ((uint32_t) ~(DMA_SxCR_DIR));
		CR_flags |= (uint32_t)dir;
		return this;
	}
	ALWAYS_INLINE
	Config* bufferSize(uint16_t size) {
		_BufferSize = size;
		return this;
	}
	ALWAYS_INLINE
	Config* memoryDataSize(MemoryDataSize size) {
		CR_flags &= ((uint32_t) ~(DMA_SxCR_MSIZE));
		CR_flags |= (uint32_t)size;
		return this;
	}
	ALWAYS_INLINE
	Config* peripheralDataSize(PeripheralDataSize size) {
		CR_flags &= ((uint32_t) ~(DMA_SxCR_PSIZE));
		CR_flags |= (uint32_t)size;
		return this;
	}
	ALWAYS_INLINE
	Config* peripheralInc(PeripheralInc mode) {
		CR_flags &= ((uint32_t) ~(DMA_SxCR_PINC));
		CR_flags |= (uint32_t)mode;
		return this;
	}
	ALWAYS_INLINE
	Config* flowControl(FlowControl mode) {
		/* Check the needed flow controller  */
		if (mode != FlowControl::Memory) {
			/* Configure DMA_SxCR_PFCTRL bit with the input parameter */
			CR_flags |= (uint32_t) DMA_SxCR_PFCTRL;
		} else {
			/* Clear the PFCTRL bit: Memory is the flow controller */
			CR_flags &= ~(uint32_t) DMA_SxCR_PFCTRL;
		}
		return this;
	}

	ALWAYS_INLINE
	Config* memoryInc(MemoryInc mode) {
		CR_flags &= ((uint32_t) ~(DMA_SxCR_MINC));
		CR_flags |= (uint32_t)mode;
		return this;
	}
	ALWAYS_INLINE
	Config* mode(Mode mode) {
		CR_flags &= ((uint32_t) ~(DMA_SxCR_CIRC));
		CR_flags |= (uint32_t)mode;
		return this;
	}
	ALWAYS_INLINE
	Config* priority(Prioriy prio) {
		CR_flags &= ((uint32_t) ~(DMA_SxCR_PL));
		CR_flags |= (uint32_t)prio;
		return this;
	}
	ALWAYS_INLINE
	Config* fifoMode(FIFOMode mode) {
		FCR_flags &= (uint32_t) ~(DMA_SxFCR_DMDIS);
		FCR_flags |= (uint32_t)mode;
		return this;
	}
	ALWAYS_INLINE
	Config* fifoThreshold(FIFOThreshold thresh) {
		FCR_flags &= (uint32_t) ~(DMA_SxFCR_FTH);
		FCR_flags |= (uint32_t)thresh;
		return this;
	}
	ALWAYS_INLINE
	Config* memoryBurst(MemoryBurst burst) {
		CR_flags &= ((uint32_t) ~(DMA_SxCR_MBURST));
		CR_flags |= (uint32_t)burst;
		return this;
	}
	ALWAYS_INLINE
	Config* peripheralBurst(PeripheralBurst burst) {
		CR_flags &= ((uint32_t) ~(DMA_SxCR_PBURST));
		CR_flags |= (uint32_t)burst;
		return this;
	}

	friend DMAStream;
	///-----------------------///
private:

	uint32_t FCR_flags;
	uint32_t CR_flags;

	uint32_t _PeripheralBaseAddr; /*!< Specifies the peripheral base address for DMAy Streamx. */

	uint32_t _Memory0BaseAddr; /*!< Specifies the memory 0 base address for DMAy Streamx.
	 This memory is the default memory used when double buffer mode is
	 not enabled. */

	uint32_t _BufferSize; /*!< Specifies the buffer size, in data unit, of the specified Stream.
	 The data unit is equal to the configuration set in DMA_PeripheralDataSize
	 or DMA_MemoryDataSize members depending in the transfer direction. */

};

class DMAStream {

public:
	DMAStream(Stream stream) {
		DMAy_Streamx = (DMA_Stream_TypeDef*) stream;

		stream_index = getStreamIndex((uint32_t)DMAy_Streamx);

		if(stream_index == 255) return;

		//make sure dma is turned on
		if(stream_index < 8) {
			RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
		} else {
			RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
		}

		if(streams[stream_index] == 0) {
			streams[stream_index] = this;
		} else {
			return;
		}

		NVIC_EnableIRQ((IRQn_Type)getIRQn(stream_index));
	}

	~DMAStream() {
		if(stream_index == 255) return;

		if(streams[stream_index] == this) {
			streams[stream_index] = 0;
		}

		NVIC_DisableIRQ((IRQn_Type)getIRQn(stream_index));
	}

	void init(Config &cfg) {
		uint32_t tmpreg = 0;
		/*------------------------- DMAy Streamx CR Configuration ------------------*/
		/* Get the DMAy_Streamx CR value */

		DMAy_Streamx->CR &= TRANSFER_IT_ENABLE_MASK;
		while (DMAy_Streamx->CR & DMA_SxCR_EN);

		clearInterruptFlags(IntFlag::DirectModeError | IntFlag::FifoError |
				IntFlag::HalfTransfer | IntFlag::TransferComplete | IntFlag::TransferError);

		tmpreg = 0;

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

		/* Write to DMAy Streamx CR register */
		DMAy_Streamx->CR |= cfg.CR_flags;

		/*------------------------- DMAy Streamx FCR Configuration -----------------*/
		/* Get the DMAy_Streamx FCR value */
		tmpreg = DMAy_Streamx->FCR;

		/* Clear DMDIS and FTH bits */
		tmpreg &= (uint32_t) ~(DMA_SxFCR_DMDIS | DMA_SxFCR_FTH);

		/* Configure DMAy Streamx FIFO:
		 Set DMDIS bits according to DMA_FIFOMode value
		 Set FTH bits according to DMA_FIFOThreshold value */
		tmpreg |= cfg.FCR_flags;

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

	inline void enable(bool enable = true) {
		if (enable) {
			/* Enable the selected DMAy Streamx by setting EN bit */
			DMAy_Streamx->CR |= (uint32_t) DMA_SxCR_EN;
		} else {
			/* Disable the selected DMAy Streamx by clearing EN bit */
			DMAy_Streamx->CR &= ~(uint32_t) DMA_SxCR_EN;
		}
	}

	inline void disable() {
		clearInterruptFlags(IntFlag::DirectModeError | IntFlag::FifoError |
				IntFlag::HalfTransfer | IntFlag::TransferComplete | IntFlag::TransferError);

		enable(0);
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

	inline void setChannel(Channel ch) {
		DMAy_Streamx->CR &= ((uint32_t) ~(DMA_SxCR_CHSEL));
		DMAy_Streamx->CR |= (uint32_t)ch;
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

	ALWAYS_INLINE
	void setXferDirection(XferDir dir) {
		DMAy_Streamx->CR &= ((uint32_t) ~(DMA_SxCR_DIR));
		DMAy_Streamx->CR |= (uint32_t)dir;
	}

	ALWAYS_INLINE
	void setMemoryInc(MemoryInc mode) {
		if(mode != MemoryInc::Enable) {
			DMAy_Streamx->CR &= ((uint32_t) ~(DMA_SxCR_MINC));
		} else {
			DMAy_Streamx->CR |= (uint32_t)mode;
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
		return getInterruptStatus(IntFlag::TransferComplete);
	}

	inline bool isError() {
		return getInterruptStatus(IntFlag::TransferError|
				IntFlag::DirectModeError|IntFlag::FifoError);
	}

	const uint8_t flag_bitpos[8] = {0, 6, 16, 22};


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

	uint32_t getInterruptFlags() {
		DMA_TypeDef* DMAy;

		/* Determine the DMA to which belongs the stream */
		if (DMAy_Streamx < DMA2_Stream0) {
			/* DMAy_Streamx belongs to DMA1 */
			DMAy = DMA1;
		} else {
			/* DMAy_Streamx belongs to DMA2 */
			DMAy = DMA2;
		}

		uint8_t idx = stream_index & 7;

		volatile uint32_t* ISR = &DMAy->LISR;
		if(idx > 3) {
			ISR = &DMAy->HISR;
		}

		return (*ISR >> flag_bitpos[idx & 3]) & 0x3F;
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
	bool getInterruptStatus(IntFlag DMA_IT) {
		DMA_TypeDef* DMAy;

		/* Determine the DMA to which belongs the stream */
		if (DMAy_Streamx < DMA2_Stream0) {
			/* DMAy_Streamx belongs to DMA1 */
			DMAy = DMA1;
		} else {
			/* DMAy_Streamx belongs to DMA2 */
			DMAy = DMA2;
		}

		uint8_t idx = stream_index & 7;

		volatile uint32_t* volatile ISR = &DMAy->LISR;
		if(idx > 3) {
			ISR = &DMAy->HISR;
		}

		return (*ISR >> flag_bitpos[idx & 3]) & (uint32_t)DMA_IT;
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
	void clearInterruptFlags(IntFlag DMA_IT) {
		DMA_TypeDef* DMAy;

		/* Determine the DMA to which belongs the stream */
		if (DMAy_Streamx < DMA2_Stream0) {
			/* DMAy_Streamx belongs to DMA1 */
			DMAy = DMA1;
		} else {
			/* DMAy_Streamx belongs to DMA2 */
			DMAy = DMA2;
		}

		uint8_t idx = stream_index & 7;

		volatile uint32_t* volatile IFCR = &DMAy->LIFCR;
		if(idx > 3) {
			IFCR = &DMAy->HIFCR;
		}

		*IFCR = (uint64_t)DMA_IT << flag_bitpos[idx & 3];
	}

	void handleIRQ() {
		if(callback) {
			callback();
		}
		if(getInterruptStatus(IntFlag::TransferComplete)) {
			clearInterruptFlags(IntFlag::TransferComplete);
		} else
		if(getInterruptStatus(IntFlag::TransferError)) {
			clearInterruptFlags(IntFlag::TransferError);
		}
	}

	bool attachCallback(xpcc::function<void()> fn) {
		callback = fn;

		interruptConfig(Interrupt::TransferComplete, ENABLE);
		interruptConfig(Interrupt::TransferError, ENABLE);

		return true;
	}

	static DMAStream* streams[];
	xpcc::function<void()> callback;

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
	uint8_t getIRQn(uint8_t idx) {
		switch(idx) {
		case 0:
			return DMA1_Stream0_IRQn;
		case 1:
			return DMA1_Stream1_IRQn;
		case 2:
			return DMA1_Stream2_IRQn;
		case 3:
			return DMA1_Stream3_IRQn;
		case 4:
			return DMA1_Stream4_IRQn;
		case 5:
			return DMA1_Stream5_IRQn;
		case 6:
			return DMA1_Stream6_IRQn;
		case 7:
			return DMA1_Stream7_IRQn;
		case 8:
			return DMA2_Stream0_IRQn;
		case 9:
			return DMA2_Stream1_IRQn;
		case 10:
			return DMA2_Stream2_IRQn;
		case 11:
			return DMA2_Stream3_IRQn;
		case 12:
			return DMA2_Stream4_IRQn;
		case 13:
			return DMA2_Stream5_IRQn;
		case 14:
			return DMA2_Stream6_IRQn;
		case 15:
			return DMA2_Stream7_IRQn;
		default:
			return 255;
		}
	}

	uint8_t stream_index;
	DMA_Stream_TypeDef* DMAy_Streamx;
};

}
}
}

