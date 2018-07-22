
add_sources(
	./container/io_buffer.cpp
	./container/smart_pointer.cpp
	./debug/error_report.cpp
	./utils/crc.cpp
	./utils/dummy.cpp
	./architecture/driver/clock_dummy.cpp
	./architecture/driver/accessor/flash.cpp
	./architecture/driver/clock.cpp
	./architecture/platform/common/syscalls.cpp
	./architecture/platform/common/cxxabi.cpp
	./io/iostream_printf.cpp
	./io/terminal.cpp
	./io/iodevice.cpp
	./io/iostream_float.cpp
	./io/iostream.cpp
	./io/buffered_device.cpp

	./driver/storage/fat.cpp

	./driver/time/time.cpp
	./math/geometry/vector2.cpp
	./math/geometry/angle.cpp
	./math/utils/bit_operation.cpp
	./math/utils/pc/operator.cpp
	
)

if(NOT ${CHIBI_RTOS})
add_sources(
	./processing/RTOSAbstraction/SimpleTasker/semaphore.cpp
)
endif()

if(${USB_SUPPORT})

add_sources(
	#TODO ./driver/connectivity/usb/USBDevice/USBAudio/USBAudio.cpp
	./driver/connectivity/usb/USBDevice/USBMSD/USBMSD.cpp
	./driver/connectivity/usb/USBDevice/USBMSD/USBMSDHandler.cpp
	./driver/connectivity/usb/USBDevice/USBSerial/USBSerialHandler.cpp
	./driver/connectivity/usb/USBDevice/USBSerial/USBCDC.cpp
	./driver/connectivity/usb/USBDevice/USBDevice/USBDevice.cpp
	./driver/connectivity/usb/USBDevice/Composite/USBCDCMSD.cpp
	./driver/connectivity/usb/USBDevice/Composite/USB2xCDCMSD.cpp
	#TODO ./driver/connectivity/usb/USBDevice/USBHID/USBHID.cpp 
	#TODO ./driver/connectivity/usb/USBDevice/USBHID/USBMouseKeyboard.cpp
	#TODO ./driver/connectivity/usb/USBDevice/USBHID/USBKeyboard.cpp
	#TODO ./driver/connectivity/usb/USBDevice/USBHID/USBMouse.cpp
	#TODO ./driver/connectivity/usb/USBDevice/USBMIDI/USBMIDI.cpp
	./driver/storage/sd/USBMSD_VolumeHandler.cpp
)


endif()

if(${PLATFORM} STREQUAL "hosted")

endif()



