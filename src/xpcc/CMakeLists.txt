
add_sources(
	./container/io_buffer.cpp
	./container/smart_pointer.cpp
	./communication/sab/slave.cpp
	./communication/sab/interface.cpp
	./communication/sab2/interface.cpp
	./communication/rpr/interface.cpp
	#./communication/can/canusb/canusb.cpp
	./communication/can/can_lawicel_formatter/can_lawicel_formatter.cpp
	./communication/xpcc/communicator.cpp
	./communication/xpcc/postman/dynamic_postman/dynamic_postman.cpp
	#./communication/xpcc/backend/tipc/tipc.cpp
	./communication/xpcc/backend/can/can_connector.cpp
	./communication/xpcc/backend/header.cpp
	./communication/xpcc/dispatcher.cpp
	./processing/ticker_task.cpp
        ./processing/timer_registry.cpp
	#./debug/logger/hosted/default_style.cpp add only for hosted platform
	./debug/error_report.cpp
	./utils/crc.cpp
	./utils/dummy.cpp
	./architecture/driver/clock_dummy.cpp
	./architecture/driver/accessor/flash.cpp
	./architecture/driver/clock.cpp
	./architecture/platform/common/syscalls.cpp
	./architecture/platform/common/cxxabi.cpp
	./ui/color.cpp
	./ui/led/tables/table8_256.cpp
	./ui/led/tables/table10_256.cpp
	./ui/led/tables/table7_256.cpp
	./ui/led/tables/table12_256.cpp
	./ui/led/tables/table16_256.cpp
	./ui/display/image/logo_rca_90x64.cpp
	./ui/display/image/logo_xpcc_90x64.cpp
	./ui/display/image/home_16x16.cpp
	./ui/display/image/skull_64x64.cpp
	./ui/display/image/logo_eurobot_90x64.cpp
	./ui/display/virtual_graphic_display.cpp
	./ui/display/character_display.cpp
	./ui/display/graphic_display_text.cpp
	./ui/display/graphic_display_fill.cpp
	./ui/display/font/numbers_46x64.cpp
	./ui/display/font/arcade_classic.cpp
	./ui/display/font/scripto_narrow.cpp
	./ui/display/font/all_caps_3x5.cpp
	./ui/display/font/fixed_width_5x8.cpp
	./ui/display/font/assertion.cpp
	./ui/display/font/numbers_14x32.cpp
	./ui/display/font/numbers_40x56.cpp
	./ui/display/graphic_display.cpp
	./ui/menu/view_stack.cpp
	./ui/menu/abstract_menu.cpp
	./ui/menu/abstract_view.cpp
	./ui/menu/standard_menu.cpp
	./ui/menu/choice_menu_entry.cpp
	./ui/menu/choice_menu.cpp
	./ui/menu/scrollable_text.cpp
	./io/iostream_printf.cpp
	./io/terminal.cpp
	./io/iodevice.cpp
	./io/iostream_float.cpp
	./io/iostream.cpp
	./io/buffered_device.cpp
	./driver/display/dogm132.cpp
	#./driver/display/st7036.cpp
	./driver/display/dogs102.cpp
	./driver/display/dogx128.cpp
	./driver/adc/ad7280a.cpp
	./driver/ui/touchscreen/touchscreen.cpp
	#./driver/can/mcp2515.cpp
	./driver/connectivity/wireless/mac802.15.4/MacFrame.cpp
	./driver/connectivity/wireless/mac802.15.4/aes/AESCCM32.cpp

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



