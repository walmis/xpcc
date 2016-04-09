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

)

if (${CHIBI_RTOS}) 


endif()

