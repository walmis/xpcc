add_sources(
	./spi/spi_1.cpp
	./spi/spi_0.cpp
	./eeprom.cpp
	./usb/USBHAL_LPC11U.cpp
	./gpio_interrupt.cpp
	./romdivide.S


	../i2c_master.cpp
	../delay.cpp
	../uart/uart_base.cpp
	../uart/uart_1.cpp
	../systick_timer.cpp
	../core.cpp
	
	./startup/LPC11Uxx_handlers.c
	./startup/LPC1xxx_startup.c

)

set(XPCC_PLATFORM_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/CMakePlatform.inc" CACHE INTERNAL "")    

if (${CHIBI_RTOS}) 


endif()

