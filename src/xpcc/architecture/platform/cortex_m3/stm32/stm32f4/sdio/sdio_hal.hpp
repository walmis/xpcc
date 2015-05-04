/*
 * sdio.hpp
 *
 *  Created on: Apr 20, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_SDIO_SDIO_HPP_
#define SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_SDIO_SDIO_HPP_

#include "../../../stm32.hpp"

namespace xpcc {
namespace stm32 {

/* ---------------------- SDIO registers bit mask ------------------------ */
/* --- CLKCR Register ---*/
/* CLKCR register clear mask */
#define CLKCR_CLEAR_MASK         ((uint32_t)0xFFFF8100)

/* --- PWRCTRL Register ---*/
/* SDIO PWRCTRL Mask */
#define PWR_PWRCTRL_MASK         ((uint32_t)0xFFFFFFFC)

/* --- DCTRL Register ---*/
/* SDIO DCTRL Clear Mask */
#define DCTRL_CLEAR_MASK         ((uint32_t)0xFFFFFF08)

/* --- CMD Register ---*/
/* CMD Register clear mask */
#define CMD_CLEAR_MASK           ((uint32_t)0xFFFFF800)

/* SDIO RESP Registers Address */
#define SDIO_RESP_ADDR           ((uint32_t)(SDIO_BASE + 0x14))

/* ------------ SDIO registers bit address in the alias region ----------- */
#define SDIO_OFFSET                (SDIO_BASE - PERIPH_BASE)

/* --- CLKCR Register ---*/
/* Alias word address of CLKEN bit */
#define CLKCR_OFFSET              (SDIO_OFFSET + 0x04)
#define CLKEN_BitNumber           0x08
#define CLKCR_CLKEN_BB            (PERIPH_BB_BASE + (CLKCR_OFFSET * 32) + (CLKEN_BitNumber * 4))

/* --- CMD Register ---*/
/* Alias word address of SDIOSUSPEND bit */
#define CMD_OFFSET                (SDIO_OFFSET + 0x0C)
#define SDIOSUSPEND_BitNumber     0x0B
#define CMD_SDIOSUSPEND_BB        (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (SDIOSUSPEND_BitNumber * 4))

/* Alias word address of ENCMDCOMPL bit */
#define ENCMDCOMPL_BitNumber      0x0C
#define CMD_ENCMDCOMPL_BB         (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (ENCMDCOMPL_BitNumber * 4))

/* Alias word address of NIEN bit */
#define NIEN_BitNumber            0x0D
#define CMD_NIEN_BB               (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (NIEN_BitNumber * 4))

/* Alias word address of ATACMD bit */
#define ATACMD_BitNumber          0x0E
#define CMD_ATACMD_BB             (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (ATACMD_BitNumber * 4))

/* --- DCTRL Register ---*/
/* Alias word address of DMAEN bit */
#define DCTRL_OFFSET              (SDIO_OFFSET + 0x2C)
#define DMAEN_BitNumber           0x03
#define DCTRL_DMAEN_BB            (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (DMAEN_BitNumber * 4))

/* Alias word address of RWSTART bit */
#define RWSTART_BitNumber         0x08
#define DCTRL_RWSTART_BB          (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (RWSTART_BitNumber * 4))

/* Alias word address of RWSTOP bit */
#define RWSTOP_BitNumber          0x09
#define DCTRL_RWSTOP_BB           (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (RWSTOP_BitNumber * 4))

/* Alias word address of RWMOD bit */
#define RWMOD_BitNumber           0x0A
#define DCTRL_RWMOD_BB            (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (RWMOD_BitNumber * 4))

/* Alias word address of SDIOEN bit */
#define SDIOEN_BitNumber          0x0B
#define DCTRL_SDIOEN_BB           (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (SDIOEN_BitNumber * 4))

class SDIO_HAL {
public:

	enum class ClockEdge {
		Rising = ((uint32_t) 0x00000000), Falling = ((uint32_t) 0x00002000)
	};

	enum class ClockFlags {
		None = 0x00, Bypass = 0x400, PowerSave = 0x200
	};

	enum class BusWidth {
		_1b = 0x00, _4b = 0x800, _8b = 0x1000
	};

	enum class HardwareFlowControl {
		Disable = 0x00, Enable = 0x4000
	};

	enum class PowerState {
		Off = 0x00, On = 0x03
	};

	/** @defgroup SDIO_Interrupt_sources
	 * @{
	 */

	enum Interrupt {
		CCRCFAIL = ((uint32_t) 0x00000001),
		DCRCFAIL = ((uint32_t) 0x00000002),
		CTIMEOUT = ((uint32_t) 0x00000004),
		DTIMEOUT = ((uint32_t) 0x00000008),
		TXUNDERR = ((uint32_t) 0x00000010),
		RXOVERR = ((uint32_t) 0x00000020),
		CMDREND = ((uint32_t) 0x00000040),
		CMDSENT = ((uint32_t) 0x00000080),
		DATAEND = ((uint32_t) 0x00000100),
		STBITERR = ((uint32_t) 0x00000200),
		DBCKEND = ((uint32_t) 0x00000400),
		CMDACT = ((uint32_t) 0x00000800),
		TXACT = ((uint32_t) 0x00001000),
		RXACT = ((uint32_t) 0x00002000),
		TXFIFOHE = ((uint32_t) 0x00004000),
		RXFIFOHF = ((uint32_t) 0x00008000),
		TXFIFOF = ((uint32_t) 0x00010000),
		RXFIFOF = ((uint32_t) 0x00020000),
		TXFIFOE = ((uint32_t) 0x00040000),
		RXFIFOE = ((uint32_t) 0x00080000),
		TXDAVL = ((uint32_t) 0x00100000),
		RXDAVL = ((uint32_t) 0x00200000),
		SDIOIT = ((uint32_t) 0x00400000),
		CEATAEND = ((uint32_t) 0x00800000)
	};
#define IS_SDIO_IT(IT) ((((IT) & (uint32_t)0xFF000000) == 0x00) && ((IT) != (uint32_t)0x00))
	/**
	 * @}
	 */

	/** @defgroup SDIO_Command_Index
	 * @{
	 */

#define IS_SDIO_CMD_INDEX(INDEX)            ((INDEX) < 0x40)

	enum class ResponseType {
		None = 0, Short = 0x40, Long = 0xC0
	};

	/** @defgroup SDIO_Wait_Interrupt_State
	 * @{
	 */

	enum class SDIOWait {
		NoWait = 0,
		WaitIT = 0x100,
		WaitPend = 0x200
	};

	enum class CPSM {
		Disable = 0x00, Enable = 0x400
	};

	enum class SDIOResp {
		RESP1 = ((uint32_t) 0x00000000),
		RESP2 = ((uint32_t) 0x00000004),
		RESP3 = ((uint32_t) 0x00000008),
		RESP4 = ((uint32_t) 0x0000000C)
	};

	enum class DataBlockSize {
		_1b = ((uint32_t) 0x00000000),
		_2b = ((uint32_t) 0x00000010),
		_4b = ((uint32_t) 0x00000020),
		_8b = ((uint32_t) 0x00000030),
		_16b = ((uint32_t) 0x00000040),
		_32b = ((uint32_t) 0x00000050),
		_64b = ((uint32_t) 0x00000060),
		_128b = ((uint32_t) 0x00000070),
		_256b = ((uint32_t) 0x00000080),
		_512b = ((uint32_t) 0x00000090),
		_1024b = ((uint32_t) 0x000000A0),
		_2048b = ((uint32_t) 0x000000B0),
		_4096b = ((uint32_t) 0x000000C0),
		_8192b = ((uint32_t) 0x000000D0),
		_16384b = ((uint32_t) 0x000000E0)
	};

	enum class TransferDir {
		ToCard = 0x00, ToSDIO = 0x02
	};

	enum class TransferMode {
		Block = 0x0, Stream = 0x04
	};

	enum class DPSM {
		Disable = 0, Enable = 1
	};

	enum ReadWaitMode {
		CLK = 0x0, DATA2 = 0x01
	};

	static inline void init() {
		RCC->APB2ENR |= RCC_APB2ENR_SDIOEN;

		uint32_t tmpreg;
		/* Get the SDIO CLKCR value */
		tmpreg = SDIO->CLKCR;

		/* Clear CLKDIV, PWRSAV, BYPASS, WIDBUS, NEGEDGE, HWFC_EN bits */
		tmpreg &= CLKCR_CLEAR_MASK;

		/* Set CLKDIV bits according to SDIO_ClockDiv value */
		/* Set PWRSAV bit according to SDIO_ClockPowerSave value */
		/* Set BYPASS bit according to SDIO_ClockBypass value */
		/* Set WIDBUS bits according to SDIO_BusWide value */
		/* Set NEGEDGE bits according to SDIO_ClockEdge value */
		/* Set HWFC_EN bits according to SDIO_HardwareFlowControl value */
		tmpreg |= (uint32_t) ClockFlags::None | (uint32_t) BusWidth::_1b
				| (uint32_t) ClockEdge::Rising
				| (uint32_t) HardwareFlowControl::Disable;

		/* Write to SDIO CLKCR */
		SDIO->CLKCR = tmpreg;
	}

	static inline void setClockFlags(ClockFlags flags) {
		SDIO->CLKCR &= ~((1 << 9) | (1 << 10));
		SDIO->CLKCR |= (uint32_t) flags;
	}

	static inline void setClockEdge(ClockEdge edge) {
		SDIO->CLKCR &= ~((1 << 13));
		SDIO->CLKCR |= (uint32_t) edge;
	}

	static inline void setClockDiv(uint8_t div) {
		SDIO->CLKCR &= ~0xFF;
		SDIO->CLKCR |= (uint32_t) div;
	}

	static inline void setBusWidth(BusWidth bw) {
		SDIO->CLKCR &= ~((1 << 11) | (1 << 12));
		SDIO->CLKCR |= (uint32_t) bw;
	}

	static inline void setHardwareFlowControl(HardwareFlowControl fc) {
		SDIO->CLKCR &= ~((1 << 14));
		SDIO->CLKCR |= (uint32_t) fc;
	}

	static inline void setClockState(bool state) {
		*(__IO uint32_t *) CLKCR_CLKEN_BB = (uint32_t) state;
	}

	static inline void setPowerState(PowerState state = PowerState::On) {
		SDIO->POWER = (uint32_t) state;
	}

	static inline PowerState getPowerState() {
		return (PowerState) SDIO->POWER;
	}

	static inline void sendCommand(uint32_t argument, uint32_t cmdindex,
			ResponseType rtype, SDIOWait wait = SDIOWait::NoWait) {
		uint32_t tmpreg;
		/*---------------------------- SDIO ARG Configuration ------------------------*/
		/* Set the SDIO Argument value */
		SDIO->ARG = argument;

		/*---------------------------- SDIO CMD Configuration ------------------------*/
		/* Get the SDIO CMD value */
		tmpreg = SDIO->CMD;
		/* Clear CMDINDEX, WAITRESP, WAITINT, WAITPEND, CPSMEN bits */
		tmpreg &= CMD_CLEAR_MASK;
		/* Set CMDINDEX bits according to SDIO_CmdIndex value */
		/* Set WAITRESP bits according to SDIO_Response value */
		/* Set WAITINT and WAITPEND bits according to SDIO_Wait value */
		/* Set CPSMEN bits according to SDIO_CPSM value */
		tmpreg |= (uint32_t) cmdindex | (uint32_t) rtype | (uint32_t) wait
				| 0x01;

		/* Write to SDIO CMD */
		SDIO->CMD = tmpreg;
	}

	static inline uint8_t getCommandResponse() {
		return (uint8_t) (SDIO->RESPCMD);
	}

	static inline uint32_t getResponse(SDIOResp resp) {
		__IO uint32_t tmp = 0;

		tmp = SDIO_RESP_ADDR + (uint32_t) resp;

		return (*(__IO uint32_t *) tmp);
	}

	static inline void startDataTransaction(TransferDir dir, uint32_t length,
			TransferMode mode = TransferMode::Block, DataBlockSize bs =
					DataBlockSize::_512b, uint32_t timeout = 0xFFFF) {

		uint32_t tmpreg;
		/* Set the SDIO Data TimeOut value */
		SDIO->DTIMER = timeout;

		/*---------------------------- SDIO DLEN Configuration -----------------------*/
		/* Set the SDIO DataLength value */
		SDIO->DLEN = length;

		/*---------------------------- SDIO DCTRL Configuration ----------------------*/
		/* Get the SDIO DCTRL value */
		tmpreg = SDIO->DCTRL;
		/* Clear DEN, DTMODE, DTDIR and DBCKSIZE bits */
		tmpreg &= DCTRL_CLEAR_MASK;
		/* Set DEN bit according to SDIO_DPSM value */
		/* Set DTMODE bit according to SDIO_TransferMode value */
		/* Set DTDIR bit according to SDIO_TransferDir value */
		/* Set DBCKSIZE bits according to SDIO_DataBlockSize value */
		tmpreg |= (uint32_t) bs | (uint32_t) dir | (uint32_t) mode | 0x01;

		/* Write to SDIO DCTRL */
		SDIO->DCTRL = tmpreg;
	}

	static inline uint32_t getDataCounter(void) {
		return SDIO->DCOUNT;
	}

	static inline uint32_t readData(void) {
		return SDIO->FIFO;
	}

	static inline void writeData(uint32_t Data) {
		SDIO->FIFO = Data;
	}

	static inline uint32_t getFIFOCount(void) {
		return SDIO->FIFOCNT;
	}

	/**
	 * @brief  Starts the SD I/O Read Wait operation.
	 * @param  NewState: new state of the Start SDIO Read Wait operation.
	 *         This parameter can be: ENABLE or DISABLE.
	 * @retval None
	 */
	static inline void startSDIOReadWait(bool NewState) {
		*(__IO uint32_t *) DCTRL_RWSTART_BB = (uint32_t) NewState;
	}

	/**
	 * @brief  Stops the SD I/O Read Wait operation.
	 * @param  NewState: new state of the Stop SDIO Read Wait operation.
	 *         This parameter can be: ENABLE or DISABLE.
	 * @retval None
	 */
	static inline void stopSDIOReadWait(bool NewState) {
		*(__IO uint32_t *) DCTRL_RWSTOP_BB = (uint32_t) NewState;
	}

	/**
	 * @brief  Sets one of the two options of inserting read wait interval.
	 * @param  SDIO_ReadWaitMode: SD I/O Read Wait operation mode.
	 *          This parameter can be:
	 *            @arg SDIO_ReadWaitMode_CLK: Read Wait control by stopping SDIOCLK
	 *            @arg SDIO_ReadWaitMode_DATA2: Read Wait control using SDIO_DATA2
	 * @retval None
	 */
	static inline void setSDIOReadWaitMode(ReadWaitMode SDIO_ReadWaitMode) {
		*(__IO uint32_t *) DCTRL_RWMOD_BB = (uint32_t) SDIO_ReadWaitMode;
	}

	/**
	 * @brief  Enables or disables the SD I/O Mode Operation.
	 * @param  NewState: new state of SDIO specific operation.
	 *         This parameter can be: ENABLE or DISABLE.
	 * @retval None
	 */
	static inline void setSDIOOperation(bool NewState) {
		*(__IO uint32_t *) DCTRL_SDIOEN_BB = (uint32_t) NewState;
	}

	/**
	 * @brief  Enables or disables the SD I/O Mode suspend command sending.
	 * @param  NewState: new state of the SD I/O Mode suspend command.
	 *         This parameter can be: ENABLE or DISABLE.
	 * @retval None
	 */
	static inline void sendSDIOSuspendCmd(bool NewState) {
		*(__IO uint32_t *) CMD_SDIOSUSPEND_BB = (uint32_t) NewState;
	}

	/**
	 * @brief  Enables or disables the SDIO DMA request.
	 * @param  NewState: new state of the selected SDIO DMA request.
	 *          This parameter can be: ENABLE or DISABLE.
	 * @retval None
	 */
	static inline void DMACmd(bool enable) {
		*(__IO uint32_t *) DCTRL_DMAEN_BB = (uint32_t) enable;
	}

	static inline void interruptConfig(Interrupt it, bool NewState) {
		if (NewState) {
			/* Enable the SDIO interrupts */
			SDIO->MASK |= (uint32_t) it;
		} else {
			/* Disable the SDIO interrupts */
			SDIO->MASK &= ~(uint32_t) it;
		}
	}

	static inline bool getInterruptStatus(Interrupt it) {
		if ((SDIO->STA & (uint32_t) it)) {
			return true;
		} else {
			return false;
		}
	}

	static inline void clearInterrupt(Interrupt it) {
		SDIO->ICR = (uint32_t) it;
	}

};

}
}

#endif /* SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_SDIO_SDIO_HPP_ */
