
#include <xpcc/architecture.hpp>
#include <xpcc/driver/temperature/ds18b20.hpp>

#include <xpcc/io/iostream.hpp>

using namespace xpcc::atmega;

typedef GpioC2 OneWirePin
xpcc::SoftwareOneWireMaster<OneWirePin> ow;

Uart0 uart(9600);

int
main()
{
	// Enable interrupts, this is needed for every buffered UART
	sei();

	// Create a IOStream for complex formatting tasks
	xpcc::IODeviceWrapper<Uart0> device(uart);
	xpcc::IOStream output(device);

	output << "Welcome" << xpcc::endl;
	xpcc::delay_ms(100);

	ow.initialize();

	if (!ow.touchReset()) {
		output << "No devices found!" << xpcc::endl;
		xpcc::delay_ms(100);
		while (1) {
			// wait forever
		}
	}

	// search for connected DS18B20 devices
	ow.resetSearch(0x28);

	uint8_t rom[8];
	while (ow.searchNext(rom)) {
		output << "found: " << xpcc::hex;
		for (uint8_t i = 0; i < 8; ++i) {
			output << rom[i];
		}
		output << xpcc::ascii << xpcc::endl;
		xpcc::delay_ms(100);
	}
	output << "finished!" << xpcc::endl;

	// read the temperature from a connected DS18B20
	xpcc::Ds18b20< xpcc::SoftwareOneWireMaster<OneWirePin> > ds18b20(rom);

	ds18b20.startConversion();

	while (1)
	{
		if (ds18b20.isConversionDone())
		{
			int16_t temperature = ds18b20.readTemperature();

			output << "Temperature: " << temperature << xpcc::endl;
			xpcc::delay_ms(100);

			ds18b20.startConversion();
		}
	}
}

