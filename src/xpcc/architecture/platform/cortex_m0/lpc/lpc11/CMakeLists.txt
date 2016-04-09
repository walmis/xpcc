add_sources(
	./spi/spi_1.cpp
	./spi/spi_0.cpp
	./Timer16B0.cpp
	./i2c_master.cpp
	./Timer32B1.cpp
	./Timer32B0.cpp
	./Timer16B1.cpp
	./gpio_interrupt.cpp

	../i2c_master.cpp
	../delay.cpp
	../uart/uart_base.cpp
	../uart/uart_1.cpp
	../systick_timer.cpp
	../core.cpp

)

if (${CHIBI_RTOS}) 


endif()

