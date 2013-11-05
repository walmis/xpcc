
#include <xpcc/architecture/platform.hpp>
#include "../stm32f4_discovery.hpp"


MAIN_FUNCTION
{
	typedef SystemClock<Pll<ExternalClock<MHz8>, MHz168, MHz48> > systemClock;
	systemClock::enable();

	LedOrange::setOutput(xpcc::Gpio::HIGH);
	LedGreen::setOutput(xpcc::Gpio::LOW);
	LedRed::setOutput(xpcc::Gpio::HIGH);
	LedBlue::setOutput(xpcc::Gpio::HIGH);

	// Enable Uart SPI 2
	GpioOutputA2::connect(UartSpiSimpleMaster2::Mosi);
	GpioInputA3::connect(UartSpiSimpleMaster2::Miso);
	GpioOutputA4::connect(UartSpiSimpleMaster2::Sck);
	UartSpiSimpleMaster2::initialize<systemClock, 10500000/2>();


	while (1)
	{
		UartSpiSimpleMaster2::writeBlocking(0xF0);
	}

	return 0;
}
