add_sources(
	./i2c/i2c0.cpp
	./i2c/i2c1.cpp
	./i2c/i2c2.cpp
	./iap.cpp
	./uart3.cpp
	./uart0.cpp
	./adc.cpp
	./uart2.cpp
	./usb/USBHAL_LPC17.cpp
	./systick_timer.cpp
	./core.cpp
	./dma/SETUP.cpp
	./dma/INIT.cpp
	./dma/DMA.cpp
	./uart1.cpp
	./gpio_interrupt.cpp
	../rand.cpp
)

if (${CHIBI_RTOS}) 


endif()

