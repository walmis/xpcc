/*
 * iocon.hpp
 *
 *  Created on: Apr 3, 2013
 *      Author: walmis
 */

#ifndef IOCON_HPP_
#define IOCON_HPP_


#if defined(__ARM_LPC11UXX__)
typedef struct {						/*!< LPC11AXX/LPC11UXX/LPC11EXX IOCON Structure */
	__IO uint32_t  PIO0[24];
	__IO uint32_t  PIO1[32];
} LPC_IOCON_T;

enum CHIP_IOCON_PIN_LOC_T {
};

#define SWITCH_PINS_PORT0(pin)  (pin == 0 || pin == 10 || pin == 11 || \
							pin == 12 || pin == 13 || pin == 14 || pin == 15)

#define SWITCH_PINS_PORT1(pin)  0

#else
/**
 * @brief LPC11XX I/O Configuration register offset
 */

const uint16_t iocon_offsets0[] = { 0x00C, 0x010, 0x01C, 0x02C, 0x030, 0x034,
		0x04C, 0x050, 0x060, 0x064, 0x068, 0x074, };

const uint16_t iocon_offsets1[] = { 0x078, 0x07C, 0x080, 0x090, 0x094, 0x0A0,
		0x0A4, 0x0A8, 0x014, 0x038, 0x06C, 0x098, };

const uint16_t iocon_offsets2[] = { 0x008, 0x028, 0x05C, 0x08C, 0x040, 0x044,
		0x000, 0x020, 0x024, 0x054, 0x058, 0x070, };

const uint16_t iocon_offsets3[] = { 0x084, 0x088, 0x09C, 0x0AC, 0x03C, 0x048, };

/**
 * @brief LPC11XX Pin location select
 */
typedef enum CHIP_IOCON_PIN_LOC {
	IOCON_SCKLOC_PIO0_10        = ((0xB0 << 2)),		/*!< Selects SCK0 function in pin location PIO0_10 */
	IOCON_SCKLOC_PIO2_11        = ((0xB0 << 2) | 1),	/*!< Selects SCK0 function in pin location PIO2_11 */
	IOCON_SCKLOC_PIO0_6         = ((0xB0 << 2) | 2),	/*!< Selects SCK0 function in pin location PIO0_6 */
	IOCON_DSRLOC_PIO2_1         = ((0xB4 << 2)),		/*!< Selects DSR function in pin location PIO2_1 */
	IOCON_DSRLOC_PIO3_1         = ((0xB4 << 2) | 1),	/*!< Selects DSR function in pin location PIO3_1 */
	IOCON_DCDLOC_PIO2_2         = ((0xB8 << 2)),		/*!< Selects DCD function in pin location PIO2_2 */
	IOCON_DCDLOC_PIO3_2         = ((0xB8 << 2) | 1),	/*!< Selects DCD function in pin location PIO3_2 */
	IOCON_RILOC_PIO2_3          = ((0xBC << 2)),		/*!< Selects RI function in pin location PIO2_3 */
	IOCON_RILOC_PIO3_3          = ((0xBC << 2) | 1),	/*!< Selects Ri function in pin location PIO3_3 */
} CHIP_IOCON_PIN_LOC_T;

typedef struct {						/*!< LPC11XX/LPC11XXLV/LPC11UXX IOCON Structure */
	__IO uint32_t  REG[48];
} LPC_IOCON_T;
#endif

enum PinMode {
	/** Disable pull-down and pull-up resistor at resistor at pad */
	Floating = (0x0 << 3),
	/** Enable pull-down resistor at pad */
	PullDown = (0x1 << 3),
	/** Enable pull-up resistor at pad */
	PullUp = (0x2 << 3),
	/** Enable pull-down and pull-up resistor at resistor at pad= (repeater mode), */
	MD_PDN = (0x3 << 3),
	/** Enable hysteresis */
	Hysteresis = (0x1 << 5),
	/** Invert enable */
	MD_INV = (0x1 << 6),
	/** Select analog mode */
	Analog = (0x0 << 7),
	/** Select digitial mode */
	Digital = (0x1 << 7),
	/** Disable 10nS input glitch filter */
	MD_DISFIL = (0x0 << 8),
	/** Enable 10nS input glitch filter */
	GlitchFilter = (0x1 << 8),
	/** I2C standard mode/fast-mode */
	MD_SFI2C = (0x0 << 8),
	/** I2C standard I/O functionality */
	StdI2c = (0x1 << 8),
	/** I2C Fast-mode Plus */
	FastI2c = (0x2 << 8),
	/** Open drain mode bit */
	OpenDrain = (0x1 << 10)
};

ENUM_CLASS_FLAG(PinMode);

#define FUNC0 0x0
#define FUNC1 0x1
#define FUNC2 0x2
#define FUNC3 0x3
#define FUNC4 0x4
#define FUNC5 0x5
#define FUNC6 0x6
#define FUNC7 0x7

namespace xpcc {
namespace lpc11 {


class IOCon {
public:

#if defined(__ARM_LPC11UXX__)
	static inline void
	setPinFunc(uint8_t port, uint8_t pin, uint8_t func)
	{
		uint32_t tmp;
		if (port == 0) {
			tmp = ((LPC_IOCON_T*)LPC_IOCON)->PIO0[pin];
			tmp &= ~0b111;
			tmp |= func;
			((LPC_IOCON_T*)LPC_IOCON)->PIO0[pin] = tmp;
		}
		else {
			tmp = ((LPC_IOCON_T*)LPC_IOCON)->PIO1[pin];
			tmp &= ~0b111;
			tmp |= func;
			((LPC_IOCON_T*)LPC_IOCON)->PIO1[pin] = tmp;
		}
	}
	static inline void
	setGpioMode(uint8_t port, uint8_t pin)
	{
		uint32_t tmp;
		if (port == 0) {
			tmp = ((LPC_IOCON_T*)LPC_IOCON)->PIO0[pin];
			tmp &= ~0b111;
			if(SWITCH_PINS_PORT0(pin)) {
				tmp |= 1;
			}
			((LPC_IOCON_T*)LPC_IOCON)->PIO0[pin] = tmp;
		}
		else {
			tmp = ((LPC_IOCON_T*)LPC_IOCON)->PIO1[pin];
			tmp &= ~0b111;
			if(SWITCH_PINS_PORT1(pin)) {
				tmp |= 1;
			}
			((LPC_IOCON_T*)LPC_IOCON)->PIO1[pin] = tmp;
		}
	}
	static inline void
	setPinMode(uint8_t port, uint8_t pin, PinMode mode)
	{
		uint32_t tmp;
		if (port == 0) {
			tmp = ((LPC_IOCON_T*)LPC_IOCON)->PIO0[pin];
			tmp &= 0b111;
			tmp |= (uint8_t)mode;
			((LPC_IOCON_T*)LPC_IOCON)->PIO0[pin] = tmp;
		}
		else {
			tmp = ((LPC_IOCON_T*)LPC_IOCON)->PIO1[pin];
			tmp &= 0b111;
			tmp |= (uint8_t)mode;
			((LPC_IOCON_T*)LPC_IOCON)->PIO1[pin] = tmp;
		}
	}
	static inline void
	setPinLoc(CHIP_IOCON_PIN_LOC_T sel)	{
	}
#else
	static void
	ALWAYS_INLINE
	setPinFunc(uint8_t port, uint8_t pin, uint8_t func)
	{
		uint32_t tmp;

		int offset=0;
		if(port == 0) {
			offset = iocon_offsets0[pin];
		} else if(port == 1) {
			offset = iocon_offsets1[pin];
		} else if(port == 2) {
			offset = iocon_offsets2[pin];
		} else if(port == 3) {
			offset = iocon_offsets3[pin];
		}

		tmp = *((uint32_t *) (((uint32_t) LPC_IOCON) + offset));
		tmp &= ~0b111;
		tmp |= func;
		*((uint32_t *) (((uint32_t) LPC_IOCON) + offset)) = tmp;
	}

	static void
	ALWAYS_INLINE
	setPinMode(uint8_t port, uint8_t pin, PinMode mode)
	{
		uint32_t tmp;

		int offset=0;
		if(port == 0) {
			offset = iocon_offsets0[pin];
		} else if(port == 1) {
			offset = iocon_offsets1[pin];
		} else if(port == 2) {
			offset = iocon_offsets2[pin];
		} else if(port == 3) {
			offset = iocon_offsets3[pin];
		}

		tmp = *((uint32_t *) (((uint32_t) LPC_IOCON) + offset));
		tmp &= 0b0111000111;
		tmp |= (uint8_t)mode;
		*((uint32_t *) (((uint32_t) LPC_IOCON) + offset)) = tmp;
	}
	static void
	ALWAYS_INLINE
	setPinLoc(CHIP_IOCON_PIN_LOC_T sel)	{
		*((uint32_t *) (((uint32_t) LPC_IOCON) + (sel >> 2))) = sel & 0x03;
	}
#endif



};



}
}


#endif /* IOCON_HPP_ */
