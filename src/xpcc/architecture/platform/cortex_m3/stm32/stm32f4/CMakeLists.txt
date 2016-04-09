add_sources(
	../core.cpp
	../systick_timer.cpp
	
	./i2c/i2c3.cpp
	./i2c/i2c1.cpp
	./i2c/i2c2.cpp
	./adc/adc_interrupt.cpp
	./adc/adc.cpp
	./timer/general_purpose.cpp
	./sdio/sdio_sdcard.cpp
	./uart/usart6.cpp
	./uart/usart1.cpp
	./uart/usart2.cpp
	./uart/usart3.cpp
	./uart/uart4.cpp
	./uart/uart5.cpp
	./gpio/gpio_interrupt.cpp
	./usb/USBDevice-USBHAL_STM32F4.cpp
	./dma/dma.cpp
)

if (${CHIBI_RTOS}) 

add_sources(
	./chibi/crt0_v7m.S
	./chibi/chcoreasm_v7m.S
	./chibi/chcore.c
	./chibi/chcore_v7m.c
)

endif()

