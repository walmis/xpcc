/*
 * gpio.hpp
 *
 *  Created on: Apr 8, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_GPIO_HPP_
#define SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_GPIO_HPP_

#include <xpcc/architecture.hpp>

enum class AltFunction {
	AF_RTC_50Hz     = ((uint8_t)0x00),  /* RTC_50Hz Alternate Function mapping */
	AF_MCO          = ((uint8_t)0x00),  /* MCO (MCO1 and MCO2) Alternate Function mapping */
	AF_TAMPER       = ((uint8_t)0x00),  /* TAMPER (TAMPER_1 and TAMPER_2) Alternate Function mapping */
	AF_SWJ          = ((uint8_t)0x00),  /* SWJ (SWD and JTAG) Alternate Function mapping */
	AF_TRACE        = ((uint8_t)0x00),  /* TRACE Alternate Function mapping */

	/**
	  * @brief   AF 1 selection
	  */
	AF_TIM1         = ((uint8_t)0x01),  /* TIM1 Alternate Function mapping */
	AF_TIM2         = ((uint8_t)0x01),  /* TIM2 Alternate Function mapping */

	/**
	  * @brief   AF 2 selection
	  */
	AF_TIM3         = ((uint8_t)0x02),  /* TIM3 Alternate Function mapping */
	AF_TIM4         = ((uint8_t)0x02),  /* TIM4 Alternate Function mapping */
	AF_TIM5         = ((uint8_t)0x02),  /* TIM5 Alternate Function mapping */

	/**
	  * @brief   AF 3 selection
	  */
	AF_TIM8         = ((uint8_t)0x03),  /* TIM8 Alternate Function mapping */
	AF_TIM9         = ((uint8_t)0x03),  /* TIM9 Alternate Function mapping */
	AF_TIM10        = ((uint8_t)0x03),  /* TIM10 Alternate Function mapping */
	AF_TIM11        = ((uint8_t)0x03),  /* TIM11 Alternate Function mapping */

	/**
	  * @brief   AF 4 selection
	  */
	AF_I2C1         = ((uint8_t)0x04),  /* I2C1 Alternate Function mapping */
	AF_I2C2         = ((uint8_t)0x04),  /* I2C2 Alternate Function mapping */
	AF_I2C3         = ((uint8_t)0x04),  /* I2C3 Alternate Function mapping */

	/**
	  * @brief   AF 5 selection
	  */
	AF_SPI1         = ((uint8_t)0x05),  /* SPI1 Alternate Function mapping */
	AF_SPI2         = ((uint8_t)0x05),  /* SPI2/I2S2 Alternate Function mapping */

	/**
	  * @brief   AF 6 selection
	  */
	AF_SPI3         = ((uint8_t)0x06),  /* SPI3/I2S3 Alternate Function mapping */

	/**
	  * @brief   AF 7 selection
	  */
	AF_USART1       = ((uint8_t)0x07),  /* USART1 Alternate Function mapping */
	AF_USART2       = ((uint8_t)0x07),  /* USART2 Alternate Function mapping */
	AF_USART3       = ((uint8_t)0x07),  /* USART3 Alternate Function mapping */
	AF_I2S3ext      = ((uint8_t)0x07),  /* I2S3ext Alternate Function mapping */

	/**
	  * @brief   AF 8 selection
	  */
	AF_UART4        = ((uint8_t)0x08),  /* UART4 Alternate Function mapping */
	AF_UART5        = ((uint8_t)0x08),  /* UART5 Alternate Function mapping */
	AF_USART6       = ((uint8_t)0x08),  /* USART6 Alternate Function mapping */

	/**
	  * @brief   AF 9 selection
	  */
	AF_CAN1         = ((uint8_t)0x09),  /* CAN1 Alternate Function mapping */
	AF_CAN2         = ((uint8_t)0x09),  /* CAN2 Alternate Function mapping */
	AF_TIM12        = ((uint8_t)0x09),  /* TIM12 Alternate Function mapping */
	AF_TIM13        = ((uint8_t)0x09),  /* TIM13 Alternate Function mapping */
	AF_TIM14        = ((uint8_t)0x09),  /* TIM14 Alternate Function mapping */

	/**
	  * @brief   AF 10 selection
	  */
	AF_OTG_FS        = ((uint8_t)0xA),  /* OTG_FS Alternate Function mapping */
	AF_OTG_HS        = ((uint8_t)0xA),  /* OTG_HS Alternate Function mapping */

	/**
	  * @brief   AF 11 selection
	  */
	AF_ETH            = ((uint8_t)0x0B),  /* ETHERNET Alternate Function mapping */

	/**
	  * @brief   AF 12 selection
	  */
	AF_FSMC           = ((uint8_t)0xC),  /* FSMC Alternate Function mapping */
	AF_OTG_HS_FS      = ((uint8_t)0xC),  /* OTG HS configured in FS, Alternate Function mapping */
	AF_SDIO           = ((uint8_t)0xC),  /* SDIO Alternate Function mapping */

	/**
	  * @brief   AF 13 selection
	  */
	AF_DCMI         = ((uint8_t)0x0D),  /* DCMI Alternate Function mapping */

	/**
	  * @brief   AF 15 selection
	  */
	AF_EVENTOUT     = ((uint8_t)0x0F),  /* EVENTOUT Alternate Function mapping */
};

/**
  * @brief   AF 0 selection
  */
#define GPIO_AF_RTC_50Hz      ((uint8_t)0x00)  /* RTC_50Hz Alternate Function mapping */
#define GPIO_AF_MCO           ((uint8_t)0x00)  /* MCO (MCO1 and MCO2) Alternate Function mapping */
#define GPIO_AF_TAMPER        ((uint8_t)0x00)  /* TAMPER (TAMPER_1 and TAMPER_2) Alternate Function mapping */
#define GPIO_AF_SWJ           ((uint8_t)0x00)  /* SWJ (SWD and JTAG) Alternate Function mapping */
#define GPIO_AF_TRACE         ((uint8_t)0x00)  /* TRACE Alternate Function mapping */

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF_TIM1          ((uint8_t)0x01)  /* TIM1 Alternate Function mapping */
#define GPIO_AF_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping */

/**
  * @brief   AF 2 selection
  */
#define GPIO_AF_TIM3          ((uint8_t)0x02)  /* TIM3 Alternate Function mapping */
#define GPIO_AF_TIM4          ((uint8_t)0x02)  /* TIM4 Alternate Function mapping */
#define GPIO_AF_TIM5          ((uint8_t)0x02)  /* TIM5 Alternate Function mapping */

/**
  * @brief   AF 3 selection
  */
#define GPIO_AF_TIM8          ((uint8_t)0x03)  /* TIM8 Alternate Function mapping */
#define GPIO_AF_TIM9          ((uint8_t)0x03)  /* TIM9 Alternate Function mapping */
#define GPIO_AF_TIM10         ((uint8_t)0x03)  /* TIM10 Alternate Function mapping */
#define GPIO_AF_TIM11         ((uint8_t)0x03)  /* TIM11 Alternate Function mapping */

/**
  * @brief   AF 4 selection
  */
#define GPIO_AF_I2C1          ((uint8_t)0x04)  /* I2C1 Alternate Function mapping */
#define GPIO_AF_I2C2          ((uint8_t)0x04)  /* I2C2 Alternate Function mapping */
#define GPIO_AF_I2C3          ((uint8_t)0x04)  /* I2C3 Alternate Function mapping */

/**
  * @brief   AF 5 selection
  */
#define GPIO_AF_SPI1          ((uint8_t)0x05)  /* SPI1 Alternate Function mapping */
#define GPIO_AF_SPI2          ((uint8_t)0x05)  /* SPI2/I2S2 Alternate Function mapping */

/**
  * @brief   AF 6 selection
  */
#define GPIO_AF_SPI3          ((uint8_t)0x06)  /* SPI3/I2S3 Alternate Function mapping */

/**
  * @brief   AF 7 selection
  */
#define GPIO_AF_USART1        ((uint8_t)0x07)  /* USART1 Alternate Function mapping */
#define GPIO_AF_USART2        ((uint8_t)0x07)  /* USART2 Alternate Function mapping */
#define GPIO_AF_USART3        ((uint8_t)0x07)  /* USART3 Alternate Function mapping */
#define GPIO_AF_I2S3ext       ((uint8_t)0x07)  /* I2S3ext Alternate Function mapping */

/**
  * @brief   AF 8 selection
  */
#define GPIO_AF_UART4         ((uint8_t)0x08)  /* UART4 Alternate Function mapping */
#define GPIO_AF_UART5         ((uint8_t)0x08)  /* UART5 Alternate Function mapping */
#define GPIO_AF_USART6        ((uint8_t)0x08)  /* USART6 Alternate Function mapping */

/**
  * @brief   AF 9 selection
  */
#define GPIO_AF_CAN1          ((uint8_t)0x09)  /* CAN1 Alternate Function mapping */
#define GPIO_AF_CAN2          ((uint8_t)0x09)  /* CAN2 Alternate Function mapping */
#define GPIO_AF_TIM12         ((uint8_t)0x09)  /* TIM12 Alternate Function mapping */
#define GPIO_AF_TIM13         ((uint8_t)0x09)  /* TIM13 Alternate Function mapping */
#define GPIO_AF_TIM14         ((uint8_t)0x09)  /* TIM14 Alternate Function mapping */

/**
  * @brief   AF 10 selection
  */
#define GPIO_AF_OTG_FS         ((uint8_t)0xA)  /* OTG_FS Alternate Function mapping */
#define GPIO_AF_OTG_HS         ((uint8_t)0xA)  /* OTG_HS Alternate Function mapping */

/**
  * @brief   AF 11 selection
  */
#define GPIO_AF_ETH             ((uint8_t)0x0B)  /* ETHERNET Alternate Function mapping */

/**
  * @brief   AF 12 selection
  */
#define GPIO_AF_FSMC            ((uint8_t)0xC)  /* FSMC Alternate Function mapping */
#define GPIO_AF_OTG_HS_FS       ((uint8_t)0xC)  /* OTG HS configured in FS, Alternate Function mapping */
#define GPIO_AF_SDIO            ((uint8_t)0xC)  /* SDIO Alternate Function mapping */

/**
  * @brief   AF 13 selection
  */
#define GPIO_AF_DCMI          ((uint8_t)0x0D)  /* DCMI Alternate Function mapping */

/**
  * @brief   AF 15 selection
  */
#define GPIO_AF_EVENTOUT      ((uint8_t)0x0F)  /* EVENTOUT Alternate Function mapping */


enum Port {
	A = 0,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I
};

enum GPIOSpeed
{
    GPIO_Speed_2MHz   = 0x00, /*!< Low speed */
    GPIO_Speed_25MHz  = 0x01, /*!< Medium speed */
    GPIO_Speed_50MHz  = 0x02, /*!< Fast speed */
    GPIO_Speed_100MHz = 0x03  /*!< High speed on 30 pF (80 MHz Output max speed on 15 pF) */
};

enum GPIOOType
{
  GPIO_OType_PUSHPULL = 0x00,
  GPIO_OType_OPENDRAIN = 0x01
};

enum GPIOPuPd
{
  GPIO_PuPd_NOPULL = 0x00,
  GPIO_PuPd_UP     = 0x01,
  GPIO_PuPd_DOWN   = 0x02
};


static GPIO_TypeDef* const ports[] = {
		GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI
};

static inline GPIO_TypeDef* _port(uint8_t p) {
	return ports[p];
}


template <int Port, int Pin>
class GpioPin {
public:
	ALWAYS_INLINE static void
	setOutput(bool status) {
		set(status);
		setOutput(); }
	ALWAYS_INLINE static void
	setOutput() {
		setInput();
		_port(Port)->MODER |= (1<<(Pin*2));
	}
	ALWAYS_INLINE static void
	setInput() {
		_port(Port)->MODER &= ~(3<<(Pin*2));
	}

	ALWAYS_INLINE static void
	setAnalog() {
		setInput();
		_port(Port)->MODER |= (3<<(Pin*2));
	}

	ALWAYS_INLINE static void
	setSpeed(GPIOSpeed speed) {
		_port(Port)->OSPEEDR &= ~(0x03 << (Pin * 2));
		_port(Port)->OSPEEDR |= ((uint32_t)(speed) << (Pin * 2));
	}

	ALWAYS_INLINE static void
	setFunction(AltFunction af) {
		uint8_t altFunction = (uint8_t)af;
		setInput();

		altFunction &= 0xF;
		uint32_t temp = 0x00;
		uint32_t temp_2 = 0x00;
		temp = ((uint32_t)(altFunction) << ((uint32_t)((uint32_t)Pin & (uint32_t)0x07) * 4)) ;
		_port(Port)->AFR[Pin >> 0x03] &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)Pin & (uint32_t)0x07) * 4)) ;
		temp_2 = _port(Port)->AFR[Pin >> 0x03] | temp;
		_port(Port)->AFR[Pin >> 0x03] = temp_2;

		_port(Port)->MODER |= (2<<(Pin*2));
	}

	ALWAYS_INLINE static void
	setOutputType(GPIOOType type) {
		_port(Port)->OTYPER  &= ~(1 << ((uint16_t)Pin)) ;
		_port(Port)->OTYPER |= (uint16_t)(((uint16_t)type) << ((uint16_t)Pin));
	}

	ALWAYS_INLINE static void
	setPullMode(GPIOPuPd mode) {
		_port(Port)->PUPDR &= ~(2 << ((uint16_t)Pin * 2));
		_port(Port)->PUPDR |= (((uint32_t)mode) << (Pin * 2));
	}

	ALWAYS_INLINE static void
	set() {
		_port(Port)->BSRRL = 1<<Pin;
	}

	ALWAYS_INLINE static void
	reset() {
		_port(Port)->BSRRH = 1<<Pin;
	}

	ALWAYS_INLINE static void
	toggle() {
		_port(Port)->ODR ^= (1<<Pin);
	}

	ALWAYS_INLINE static void
	set(bool status) {
		if(status) {
			set();
		} else {
			reset();
		}
	}

	ALWAYS_INLINE static bool
	read() {
		return _port(Port)->ODR & (1<<Pin);
	}
};

typedef GpioPin<A,0> PA0;
typedef GpioPin<A,1> PA1;
typedef GpioPin<A,2> PA2;
typedef GpioPin<A,3> PA3;
typedef GpioPin<A,4> PA4;
typedef GpioPin<A,5> PA5;
typedef GpioPin<A,6> PA6;
typedef GpioPin<A,7> PA7;
typedef GpioPin<A,8> PA8;
typedef GpioPin<A,9> PA9;
typedef GpioPin<A,10> PA10;
typedef GpioPin<A,11> PA11;
typedef GpioPin<A,12> PA12;
typedef GpioPin<A,13> PA13;
typedef GpioPin<A,14> PA14;
typedef GpioPin<A,15> PA15;
typedef GpioPin<B,0> PB0;
typedef GpioPin<B,1> PB1;
typedef GpioPin<B,2> PB2;
typedef GpioPin<B,3> PB3;
typedef GpioPin<B,4> PB4;
typedef GpioPin<B,5> PB5;
typedef GpioPin<B,6> PB6;
typedef GpioPin<B,7> PB7;
typedef GpioPin<B,8> PB8;
typedef GpioPin<B,9> PB9;
typedef GpioPin<B,10> PB10;
typedef GpioPin<B,11> PB11;
typedef GpioPin<B,12> PB12;
typedef GpioPin<B,13> PB13;
typedef GpioPin<B,14> PB14;
typedef GpioPin<B,15> PB15;
typedef GpioPin<C,0> PC0;
typedef GpioPin<C,1> PC1;
typedef GpioPin<C,2> PC2;
typedef GpioPin<C,3> PC3;
typedef GpioPin<C,4> PC4;
typedef GpioPin<C,5> PC5;
typedef GpioPin<C,6> PC6;
typedef GpioPin<C,7> PC7;
typedef GpioPin<C,8> PC8;
typedef GpioPin<C,9> PC9;
typedef GpioPin<C,10> PC10;
typedef GpioPin<C,11> PC11;
typedef GpioPin<C,12> PC12;
typedef GpioPin<C,13> PC13;
typedef GpioPin<C,14> PC14;
typedef GpioPin<C,15> PC15;
typedef GpioPin<D,0> PD0;
typedef GpioPin<D,1> PD1;
typedef GpioPin<D,2> PD2;
typedef GpioPin<D,3> PD3;
typedef GpioPin<D,4> PD4;
typedef GpioPin<D,5> PD5;
typedef GpioPin<D,6> PD6;
typedef GpioPin<D,7> PD7;
typedef GpioPin<D,8> PD8;
typedef GpioPin<D,9> PD9;
typedef GpioPin<D,10> PD10;
typedef GpioPin<D,11> PD11;
typedef GpioPin<D,12> PD12;
typedef GpioPin<D,13> PD13;
typedef GpioPin<D,14> PD14;
typedef GpioPin<D,15> PD15;
typedef GpioPin<E,0> PE0;
typedef GpioPin<E,1> PE1;
typedef GpioPin<E,2> PE2;
typedef GpioPin<E,3> PE3;
typedef GpioPin<E,4> PE4;
typedef GpioPin<E,5> PE5;
typedef GpioPin<E,6> PE6;
typedef GpioPin<E,7> PE7;
typedef GpioPin<E,8> PE8;
typedef GpioPin<E,9> PE9;
typedef GpioPin<E,10> PE10;
typedef GpioPin<E,11> PE11;
typedef GpioPin<E,12> PE12;
typedef GpioPin<E,13> PE13;
typedef GpioPin<E,14> PE14;
typedef GpioPin<E,15> PE15;
typedef GpioPin<F,0> PF0;
typedef GpioPin<F,1> PF1;
typedef GpioPin<F,2> PF2;
typedef GpioPin<F,3> PF3;
typedef GpioPin<F,4> PF4;
typedef GpioPin<F,5> PF5;
typedef GpioPin<F,6> PF6;
typedef GpioPin<F,7> PF7;
typedef GpioPin<F,8> PF8;
typedef GpioPin<F,9> PF9;
typedef GpioPin<F,10> PF10;
typedef GpioPin<F,11> PF11;
typedef GpioPin<F,12> PF12;
typedef GpioPin<F,13> PF13;
typedef GpioPin<F,14> PF14;
typedef GpioPin<F,15> PF15;
typedef GpioPin<G,0> PG0;
typedef GpioPin<G,1> PG1;
typedef GpioPin<G,2> PG2;
typedef GpioPin<G,3> PG3;
typedef GpioPin<G,4> PG4;
typedef GpioPin<G,5> PG5;
typedef GpioPin<G,6> PG6;
typedef GpioPin<G,7> PG7;
typedef GpioPin<G,8> PG8;
typedef GpioPin<G,9> PG9;
typedef GpioPin<G,10> PG10;
typedef GpioPin<G,11> PG11;
typedef GpioPin<G,12> PG12;
typedef GpioPin<G,13> PG13;
typedef GpioPin<G,14> PG14;
typedef GpioPin<G,15> PG15;
typedef GpioPin<H,0> PH0;
typedef GpioPin<H,1> PH1;
typedef GpioPin<H,2> PH2;
typedef GpioPin<H,3> PH3;
typedef GpioPin<H,4> PH4;
typedef GpioPin<H,5> PH5;
typedef GpioPin<H,6> PH6;
typedef GpioPin<H,7> PH7;
typedef GpioPin<H,8> PH8;
typedef GpioPin<H,9> PH9;
typedef GpioPin<H,10> PH10;
typedef GpioPin<H,11> PH11;
typedef GpioPin<H,12> PH12;
typedef GpioPin<H,13> PH13;
typedef GpioPin<H,14> PH14;
typedef GpioPin<H,15> PH15;
typedef GpioPin<I,0> PI0;
typedef GpioPin<I,1> PI1;
typedef GpioPin<I,2> PI2;
typedef GpioPin<I,3> PI3;
typedef GpioPin<I,4> PI4;
typedef GpioPin<I,5> PI5;
typedef GpioPin<I,6> PI6;
typedef GpioPin<I,7> PI7;
typedef GpioPin<I,8> PI8;
typedef GpioPin<I,9> PI9;
typedef GpioPin<I,10> PI10;
typedef GpioPin<I,11> PI11;
typedef GpioPin<I,12> PI12;
typedef GpioPin<I,13> PI13;
typedef GpioPin<I,14> PI14;
typedef GpioPin<I,15> PI15;




#endif /* SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_GPIO_HPP_ */
