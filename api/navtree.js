var NAVTREE =
[
  [ "XPCC", "index.html", [
    [ "Cross Platform Component Communication", "index.html", [
      [ "0. Why should i use this library?", "why_use_xpcc.html", null ],
      [ "1. Get the Code", "get_code.html", null ],
      [ "2. Install the build environment", "installation.html", [
        [ "2.1 Installation on Linux systems", "installation_linux.html", null ],
        [ "2.2 Installation on Windows", "installation_windows.html", null ],
        [ "2.2.1 Configure your favorite IDE", "configure_ide.html", null ]
      ] ],
      [ "3. Build system", "build_system.html", [
        [ "3.1 Universal SConstruct template", "sconstruct_template.html", null ],
        [ "3.2 SConstruct files: An introduction", "sconstruct.html", null ],
        [ "3.3 SConstruct files: Advanced topics", "sconstruct_advanced.html", null ],
        [ "3.4 SCons Tools", "scons_tools.html", [
          [ "Tool: xpcc", "tool_xpcc.html", null ],
          [ "Tool: configfile", "tool_configfile.html", null ],
          [ "Tool: helper", "tool_helper.html", null ],
          [ "Tool: unittest", "tool_unittest.html", null ],
          [ "Tool: template", "tool_template.html", null ],
          [ "Tool: system_design", "tool_system_design.html", null ],
          [ "Tool: avr", "tool_avr.html", null ],
          [ "Tool: avrdude", "tool_avrdude.html", null ],
          [ "Tool: pc", "tool_pc.html", null ],
          [ "Tool: arm", "tool_arm.html", null ],
          [ "Tool: openocd", "tool_openocd.html", null ],
          [ "Tool: doxygen", "tool_doxygen.html", null ]
        ] ],
        [ "3.5 Configuration files", "configuration_files.html", null ]
      ] ],
      [ "4. Getting started: How to use this library", "getting_started.html", [
        [ "4.1 Create a project using SCons", "project_sconstruct.html", null ],
        [ "4.2 Create a project using Make", "project_makefile.html", null ]
      ] ],
      [ "5. Developer", "developer.html", [
        [ "5.1 Design decisions", "design_decisions.html", null ],
        [ "5.2 Coding Conventions", "coding_conventions.html", null ],
        [ "5.3 A few words about unittests ...", "unittests.html", null ],
        [ "5.4 License", "license.html", null ]
      ] ],
      [ "6. System Design", "system_design.html", [
        [ "6.1 Parser", "system_design_parser.html", null ],
        [ "6.2 Builder", "system_design_builder.html", null ],
        [ "6.3 Xml", "system_design_xml.html", null ],
        [ "6.4 Components", "system_design_components.html", null ]
      ] ]
    ] ],
    [ "Related Pages", "pages.html", [
      [ "Todo List", "todo.html", null ],
      [ "Bug List", "bug.html", null ]
    ] ],
    [ "Modules", "modules.html", [
      [ "Unit tests", "group__unittest.html", null ],
      [ "Architecture", "group__architecture.html", [
        [ "Predefined boards", "group__board.html", null ],
        [ "Accessor classes", "group__accessor.html", null ],
        [ "Atomic operations and container", "group__atomic.html", null ],
        [ "General purpose input and/or output pins (GPIO)", "group__gpio.html", null ],
        [ "Supported Platforms", "group__platform.html", [
          [ "ARM7TDMI", "group__arm7.html", [
            [ "AT91SAM7xxx", "group__at91.html", null ],
            [ "LPC2xxx", "group__lpc2000.html", null ]
          ] ],
          [ "AVR", "group__avr.html", [
            [ "megaAVR", "group__atmega.html", null ],
            [ "tinyAVR", "group__attiny.html", null ],
            [ "XMEGA", "group__atxmega.html", [
              [ "Analog/Digital Converter Module", "group__atxmega__adc.html", null ],
              [ "Direct Memory Access Controller", "group__atxmega__dma.html", null ],
              [ "I2C Module", "group__atxmega__i2c.html", null ],
              [ "SPI Module", "group__atxmega__spi.html", null ],
              [ "Timer/Counter Module", "group__atxmega__timer.html", null ],
              [ "UART module", "group__atxmega__uart.html", null ]
            ] ]
          ] ],
          [ "Cortex M0", "group__cortex__m0.html", [
            [ "LPC11xx", "group__lpc1100.html", null ]
          ] ],
          [ "Cortex M3", "group__cortex__m3.html", [
            [ "STM32Fxxx", "group__stm32.html", null ]
          ] ],
          [ "Hosted platforms (Linux/Windows/...)", "group__hosted.html", [
            [ "Linux", "group__linux.html", [
              [ "(TIPC) Transparent Inter-Process Communication", "group__tipc.html", null ]
            ] ],
            [ "Windows", "group__windows.html", null ]
          ] ]
        ] ]
      ] ],
      [ "Communication", "group__communication.html", [
        [ "Backend", "group__backend.html", null ]
      ] ],
      [ "Containers", "group__container.html", null ],
      [ "Debugging utilies", "group__debug.html", [
        [ "Logger", "group__logger.html", null ]
      ] ],
      [ "Device drivers", "group__driver.html", [
        [ "Analog/Digital Converters", "group__adc.html", null ],
        [ "Connectivity", "group__connectivity.html", [
          [ "(AMNB) Asynchronous Multi-Node Bus", "group__amnb.html", null ],
          [ "(CAN) Controller Area Network", "group__can.html", [
            [ "MCP2515", "group__mcp2515.html", null ]
          ] ],
          [ "(I2C) Inter-Integrated Circuit", "group__i2c.html", null ],
          [ "1-Wire", "group__wire.html", null ],
          [ "(SAB) Sensor Actor Bus", "group__sab.html", null ],
          [ "(SPI) Serial Peripheral Interface", "group__spi.html", null ]
        ] ],
        [ "Digital/Analog Converters", "group__dac.html", null ],
        [ "General purpose input and/or output pins (GPIO)", "group__gpio.html", null ],
        [ "Inertial measurement", "group__inertial.html", null ],
        [ "Storage", "group__storage.html", null ],
        [ "Other", "group__driver__other.html", null ],
        [ "Pressure sensors", "group__pressure.html", null ],
        [ "Temperature sensors", "group__temperature.html", null ],
        [ "PWM Drivers", "group__pwm.html", null ],
        [ "User interface", "group__ui.html", [
          [ "LCD", "group__lcd.html", [
            [ "Various fonts for graphical displays", "group__font.html", null ],
            [ "Images for graphical displays", "group__image.html", null ]
          ] ],
          [ "LED Indicators", "group__led.html", null ]
        ] ]
      ] ],
      [ "IO-Classes", "group__io.html", null ],
      [ "Math", "group__math.html", [
        [ "Filter", "group__filter.html", null ],
        [ "Geometry", "group__geometry.html", null ],
        [ "Interpolation", "group__interpolation.html", null ],
        [ "Matrix", "group__matrix.html", null ]
      ] ],
      [ "Utilities", "group__utils.html", [
        [ "Allocator", "group__allocator.html", null ],
        [ "Arithmetic Traits", "group__arithmetic__trais.html", null ],
        [ "Template Metaprogramming", "group__tmp.html", null ]
      ] ],
      [ "Workflow", "group__workflow.html", [
        [ "Protothreads", "group__protothread.html", null ],
        [ "FreeRTOS", "group__freertos.html", null ]
      ] ]
    ] ],
    [ "Class List", "annotated.html", [
      [ "_INTAT", "union___i_n_t_a_t.html", null ],
      [ "_PCLKSEL0", "union___p_c_l_k_s_e_l0.html", null ],
      [ "_PCLKSEL1", "union___p_c_l_k_s_e_l1.html", null ],
      [ "_PCONP", "union___p_c_o_n_p.html", null ],
      [ "_TxCCR", "struct___tx_c_c_r.html", null ],
      [ "_TxCTCR", "struct___tx_c_t_c_r.html", null ],
      [ "_TxEMR", "struct___tx_e_m_r.html", null ],
      [ "_TxIR", "struct___tx_i_r.html", null ],
      [ "_TxMCR", "union___tx_m_c_r.html", null ],
      [ "_TxTCR", "struct___tx_t_c_r.html", null ],
      [ "AbortContext", "struct_abort_context.html", null ],
      [ "xpcc::AbstractComponent", "classxpcc_1_1_abstract_component.html", null ],
      [ "xpcc::sab::Action", "structxpcc_1_1sab_1_1_action.html", null ],
      [ "xpcc::amnb::Action", "structxpcc_1_1amnb_1_1_action.html", null ],
      [ "xpcc::ActionCallback", "classxpcc_1_1_action_callback.html", null ],
      [ "xpcc::AD840x", "classxpcc_1_1_a_d840x.html", null ],
      [ "xpcc::at91::Adc", "classxpcc_1_1at91_1_1_adc.html", null ],
      [ "xpcc::atmega::Adc", "classxpcc_1_1atmega_1_1_adc.html", null ],
      [ "xpcc::atxmega::AdcA", "classxpcc_1_1atxmega_1_1_adc_a.html", null ],
      [ "xpcc::atxmega::AdcB", "classxpcc_1_1atxmega_1_1_adc_b.html", null ],
      [ "xpcc::atxmega::AdcChannelA0", "classxpcc_1_1atxmega_1_1_adc_channel_a0.html", null ],
      [ "xpcc::atxmega::AdcChannelA1", "classxpcc_1_1atxmega_1_1_adc_channel_a1.html", null ],
      [ "xpcc::atxmega::AdcChannelA2", "classxpcc_1_1atxmega_1_1_adc_channel_a2.html", null ],
      [ "xpcc::atxmega::AdcChannelA3", "classxpcc_1_1atxmega_1_1_adc_channel_a3.html", null ],
      [ "xpcc::atxmega::AdcChannelB0", "classxpcc_1_1atxmega_1_1_adc_channel_b0.html", null ],
      [ "xpcc::atxmega::AdcChannelB1", "classxpcc_1_1atxmega_1_1_adc_channel_b1.html", null ],
      [ "xpcc::atxmega::AdcChannelB2", "classxpcc_1_1atxmega_1_1_adc_channel_b2.html", null ],
      [ "xpcc::atxmega::AdcChannelB3", "classxpcc_1_1atxmega_1_1_adc_channel_b3.html", null ],
      [ "xpcc::atmega::AdcInterrupt", "classxpcc_1_1atmega_1_1_adc_interrupt.html", null ],
      [ "xpcc::stm32::AdvancedControlTimer", "classxpcc_1_1stm32_1_1_advanced_control_timer.html", null ],
      [ "xpcc::ADXL345", "classxpcc_1_1_a_d_x_l345.html", null ],
      [ "xpcc::at91::Aic", "classxpcc_1_1at91_1_1_aic.html", null ],
      [ "std::allocator", "classstd_1_1allocator.html", null ],
      [ "xpcc::allocator::AllocatorBase", "classxpcc_1_1allocator_1_1_allocator_base.html", null ],
      [ "xpcc::stm32::AlternateMode", "structxpcc_1_1stm32_1_1_alternate_mode.html", null ],
      [ "xpcc::stm32::AlternateMode< P, N, false >", "structxpcc_1_1stm32_1_1_alternate_mode_3_01_p_00_01_n_00_01false_01_4.html", null ],
      [ "xpcc::atmega::AnalogSensors", "classxpcc_1_1atmega_1_1_analog_sensors.html", null ],
      [ "xpcc::Angle", "classxpcc_1_1_angle.html", null ],
      [ "xpcc::ArithmeticTraits", "structxpcc_1_1_arithmetic_traits.html", null ],
      [ "xpcc::ArithmeticTraits< char >", "structxpcc_1_1_arithmetic_traits_3_01char_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< double >", "structxpcc_1_1_arithmetic_traits_3_01double_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< float >", "structxpcc_1_1_arithmetic_traits_3_01float_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< int16_t >", "structxpcc_1_1_arithmetic_traits_3_01int16__t_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< int32_t >", "structxpcc_1_1_arithmetic_traits_3_01int32__t_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< int64_t >", "structxpcc_1_1_arithmetic_traits_3_01int64__t_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< signed char >", "structxpcc_1_1_arithmetic_traits_3_01signed_01char_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< uint16_t >", "structxpcc_1_1_arithmetic_traits_3_01uint16__t_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< uint32_t >", "structxpcc_1_1_arithmetic_traits_3_01uint32__t_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< uint64_t >", "structxpcc_1_1_arithmetic_traits_3_01uint64__t_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< unsigned char >", "structxpcc_1_1_arithmetic_traits_3_01unsigned_01char_01_4.html", null ],
      [ "xpcc::atmega::AsynchronousI2cMaster", "classxpcc_1_1atmega_1_1_asynchronous_i2c_master.html", null ],
      [ "xpcc::atxmega::AsynchronousI2cMasterC", "classxpcc_1_1atxmega_1_1_asynchronous_i2c_master_c.html", null ],
      [ "xpcc::atxmega::AsynchronousI2cMasterD", "classxpcc_1_1atxmega_1_1_asynchronous_i2c_master_d.html", null ],
      [ "xpcc::atxmega::AsynchronousI2cMasterE", "classxpcc_1_1atxmega_1_1_asynchronous_i2c_master_e.html", null ],
      [ "xpcc::atxmega::AsynchronousI2cMasterF", "classxpcc_1_1atxmega_1_1_asynchronous_i2c_master_f.html", null ],
      [ "xpcc::At45db0x1d", "classxpcc_1_1_at45db0x1d.html", null ],
      [ "std::auto_ptr", "classstd_1_1auto__ptr.html", null ],
      [ "xpcc::BackendInterface", "classxpcc_1_1_backend_interface.html", null ],
      [ "std::bad_alloc", "classstd_1_1bad__alloc.html", null ],
      [ "std::bad_cast", "classstd_1_1bad__cast.html", null ],
      [ "std::bad_exception", "classstd_1_1bad__exception.html", null ],
      [ "std::bad_typeid", "classstd_1_1bad__typeid.html", null ],
      [ "std::basic_fstream", "classstd_1_1basic__fstream.html", null ],
      [ "std::basic_ifstream", "classstd_1_1basic__ifstream.html", null ],
      [ "std::basic_ios", "classstd_1_1basic__ios.html", null ],
      [ "std::basic_iostream", "classstd_1_1basic__iostream.html", null ],
      [ "std::basic_istream", "classstd_1_1basic__istream.html", null ],
      [ "std::basic_istringstream", "classstd_1_1basic__istringstream.html", null ],
      [ "std::basic_ofstream", "classstd_1_1basic__ofstream.html", null ],
      [ "std::basic_ostream", "classstd_1_1basic__ostream.html", null ],
      [ "std::basic_ostringstream", "classstd_1_1basic__ostringstream.html", null ],
      [ "std::basic_string", "classstd_1_1basic__string.html", null ],
      [ "std::basic_stringstream", "classstd_1_1basic__stringstream.html", null ],
      [ "xpcc::stm32::BasicTimer", "classxpcc_1_1stm32_1_1_basic_timer.html", null ],
      [ "xpcc::rtos::BinarySemaphore", "classxpcc_1_1rtos_1_1_binary_semaphore.html", null ],
      [ "std::bitset", "classstd_1_1bitset.html", null ],
      [ "xpcc::allocator::Block", "classxpcc_1_1allocator_1_1_block.html", null ],
      [ "xpcc::BMA180", "classxpcc_1_1_b_m_a180.html", null ],
      [ "xpcc::BMP085", "classxpcc_1_1_b_m_p085.html", null ],
      [ "xpcc::BoundedDeque", "classxpcc_1_1_bounded_deque.html", null ],
      [ "xpcc::BoundedQueue", "classxpcc_1_1_bounded_queue.html", null ],
      [ "xpcc::BoundedStack", "classxpcc_1_1_bounded_stack.html", null ],
      [ "xpcc::InterconnectInterface::Buffer", "structxpcc_1_1_interconnect_interface_1_1_buffer.html", null ],
      [ "xpcc::BufferedGraphicDisplay", "classxpcc_1_1_buffered_graphic_display.html", null ],
      [ "xpcc::atmega::BufferedUart0", "classxpcc_1_1atmega_1_1_buffered_uart0.html", null ],
      [ "xpcc::atmega::BufferedUart1", "classxpcc_1_1atmega_1_1_buffered_uart1.html", null ],
      [ "xpcc::atmega::BufferedUart2", "classxpcc_1_1atmega_1_1_buffered_uart2.html", null ],
      [ "xpcc::atmega::BufferedUart3", "classxpcc_1_1atmega_1_1_buffered_uart3.html", null ],
      [ "xpcc::stm32::BufferedUart4", "classxpcc_1_1stm32_1_1_buffered_uart4.html", null ],
      [ "xpcc::stm32::BufferedUart5", "classxpcc_1_1stm32_1_1_buffered_uart5.html", null ],
      [ "xpcc::stm32::BufferedUsart1", "classxpcc_1_1stm32_1_1_buffered_usart1.html", null ],
      [ "xpcc::stm32::BufferedUsart2", "classxpcc_1_1stm32_1_1_buffered_usart2.html", null ],
      [ "xpcc::stm32::BufferedUsart3", "classxpcc_1_1stm32_1_1_buffered_usart3.html", null ],
      [ "xpcc::stm32::BufferedUsart6", "classxpcc_1_1stm32_1_1_buffered_usart6.html", null ],
      [ "xpcc::Button", "classxpcc_1_1_button.html", null ],
      [ "xpcc::ButtonGroup", "classxpcc_1_1_button_group.html", null ],
      [ "xpcc::sab::Callable", "structxpcc_1_1sab_1_1_callable.html", null ],
      [ "xpcc::amnb::Callable", "structxpcc_1_1amnb_1_1_callable.html", null ],
      [ "xpcc::Dispatcher::CallbackEntry", "classxpcc_1_1_dispatcher_1_1_callback_entry.html", null ],
      [ "xpcc::stm32::Can1", "classxpcc_1_1stm32_1_1_can1.html", null ],
      [ "xpcc::CanConnector", "classxpcc_1_1_can_connector.html", null ],
      [ "xpcc::CanConnectorBase", "classxpcc_1_1_can_connector_base.html", null ],
      [ "xpcc::stm32::CanFilter", "classxpcc_1_1stm32_1_1_can_filter.html", null ],
      [ "xpcc::CanUsb", "classxpcc_1_1_can_usb.html", null ],
      [ "xpcc::CanUsbFormater", "classxpcc_1_1_can_usb_formater.html", null ],
      [ "xpcc::CharacterDisplay", "classxpcc_1_1_character_display.html", null ],
      [ "xpcc::Circle2D", "classxpcc_1_1_circle2_d.html", null ],
      [ "xpcc::stm32::Clock", "classxpcc_1_1stm32_1_1_clock.html", null ],
      [ "xpcc::lpc::Clock", "classxpcc_1_1lpc_1_1_clock.html", null ],
      [ "xpcc::Clock", "classxpcc_1_1_clock.html", null ],
      [ "xpcc::Communicatable", "classxpcc_1_1_communicatable.html", null ],
      [ "xpcc::CommunicatableTask", "classxpcc_1_1_communicatable_task.html", null ],
      [ "xpcc::Communicator", "classxpcc_1_1_communicator.html", null ],
      [ "std::complex", "classstd_1_1complex.html", null ],
      [ "xpcc::BoundedDeque::const_iterator", "classxpcc_1_1_bounded_deque_1_1const__iterator.html", null ],
      [ "xpcc::DoublyLinkedList::const_iterator", "classxpcc_1_1_doubly_linked_list_1_1const__iterator.html", null ],
      [ "xpcc::DynamicArray::const_iterator", "classxpcc_1_1_dynamic_array_1_1const__iterator.html", null ],
      [ "xpcc::LinkedList::const_iterator", "classxpcc_1_1_linked_list_1_1const__iterator.html", null ],
      [ "std::basic_string::const_iterator", "classstd_1_1basic__string_1_1const__iterator.html", null ],
      [ "std::string::const_iterator", "classstd_1_1string_1_1const__iterator.html", null ],
      [ "std::wstring::const_iterator", "classstd_1_1wstring_1_1const__iterator.html", null ],
      [ "std::deque::const_iterator", "classstd_1_1deque_1_1const__iterator.html", null ],
      [ "std::list::const_iterator", "classstd_1_1list_1_1const__iterator.html", null ],
      [ "std::map::const_iterator", "classstd_1_1map_1_1const__iterator.html", null ],
      [ "std::multimap::const_iterator", "classstd_1_1multimap_1_1const__iterator.html", null ],
      [ "std::set::const_iterator", "classstd_1_1set_1_1const__iterator.html", null ],
      [ "std::multiset::const_iterator", "classstd_1_1multiset_1_1const__iterator.html", null ],
      [ "std::vector::const_iterator", "classstd_1_1vector_1_1const__iterator.html", null ],
      [ "std::basic_string::const_reverse_iterator", "classstd_1_1basic__string_1_1const__reverse__iterator.html", null ],
      [ "std::string::const_reverse_iterator", "classstd_1_1string_1_1const__reverse__iterator.html", null ],
      [ "std::wstring::const_reverse_iterator", "classstd_1_1wstring_1_1const__reverse__iterator.html", null ],
      [ "std::deque::const_reverse_iterator", "classstd_1_1deque_1_1const__reverse__iterator.html", null ],
      [ "std::list::const_reverse_iterator", "classstd_1_1list_1_1const__reverse__iterator.html", null ],
      [ "std::map::const_reverse_iterator", "classstd_1_1map_1_1const__reverse__iterator.html", null ],
      [ "std::multimap::const_reverse_iterator", "classstd_1_1multimap_1_1const__reverse__iterator.html", null ],
      [ "std::set::const_reverse_iterator", "classstd_1_1set_1_1const__reverse__iterator.html", null ],
      [ "std::multiset::const_reverse_iterator", "classstd_1_1multiset_1_1const__reverse__iterator.html", null ],
      [ "std::vector::const_reverse_iterator", "classstd_1_1vector_1_1const__reverse__iterator.html", null ],
      [ "xpcc::atomic::Container", "classxpcc_1_1atomic_1_1_container.html", null ],
      [ "unittest::Controller", "classunittest_1_1_controller.html", null ],
      [ "xpcc::tmp::Conversion", "classxpcc_1_1tmp_1_1_conversion.html", null ],
      [ "xpcc::tmp::Conversion< T, T >", "structxpcc_1_1tmp_1_1_conversion_3_01_t_00_01_t_01_4.html", null ],
      [ "xpcc::tmp::Conversion< T, void >", "structxpcc_1_1tmp_1_1_conversion_3_01_t_00_01void_01_4.html", null ],
      [ "xpcc::tmp::Conversion< void, T >", "structxpcc_1_1tmp_1_1_conversion_3_01void_00_01_t_01_4.html", null ],
      [ "xpcc::tmp::Conversion< void, void >", "structxpcc_1_1tmp_1_1_conversion_3_01void_00_01void_01_4.html", null ],
      [ "xpcc::stm32::Core", "classxpcc_1_1stm32_1_1_core.html", null ],
      [ "xpcc::at91::Core", "classxpcc_1_1at91_1_1_core.html", null ],
      [ "unittest::CountType", "classunittest_1_1_count_type.html", null ],
      [ "xpcc::CpuBoard2", "classxpcc_1_1_cpu_board2.html", null ],
      [ "xpcc::CpuBoard2Slave", "classxpcc_1_1_cpu_board2_slave.html", null ],
      [ "xpcc::filter::Debounce", "classxpcc_1_1filter_1_1_debounce.html", null ],
      [ "xpcc::at91::Debug", "classxpcc_1_1at91_1_1_debug.html", null ],
      [ "xpcc::log::DefaultStyle", "classxpcc_1_1log_1_1_default_style.html", null ],
      [ "xpcc::i2c::Delegate", "classxpcc_1_1i2c_1_1_delegate.html", null ],
      [ "std::deque", "classstd_1_1deque.html", null ],
      [ "xpcc::fat::Directory", "classxpcc_1_1fat_1_1_directory.html", null ],
      [ "xpcc::Dispatcher", "classxpcc_1_1_dispatcher.html", null ],
      [ "xpcc::atxmega::DmaChannel0", "classxpcc_1_1atxmega_1_1_dma_channel0.html", null ],
      [ "xpcc::atxmega::DmaChannel1", "classxpcc_1_1atxmega_1_1_dma_channel1.html", null ],
      [ "xpcc::atxmega::DmaChannel2", "classxpcc_1_1atxmega_1_1_dma_channel2.html", null ],
      [ "xpcc::atxmega::DmaChannel3", "classxpcc_1_1atxmega_1_1_dma_channel3.html", null ],
      [ "xpcc::atxmega::DmaController", "classxpcc_1_1atxmega_1_1_dma_controller.html", null ],
      [ "xpcc::DogL128", "classxpcc_1_1_dog_l128.html", null ],
      [ "xpcc::DogM081", "classxpcc_1_1_dog_m081.html", null ],
      [ "xpcc::DogM128", "classxpcc_1_1_dog_m128.html", null ],
      [ "xpcc::DogM132", "classxpcc_1_1_dog_m132.html", null ],
      [ "xpcc::DogM162", "classxpcc_1_1_dog_m162.html", null ],
      [ "xpcc::DogM163", "classxpcc_1_1_dog_m163.html", null ],
      [ "xpcc::DogS102", "classxpcc_1_1_dog_s102.html", null ],
      [ "std::domain_error", "classstd_1_1domain__error.html", null ],
      [ "xpcc::led::DoubleIndicator", "classxpcc_1_1led_1_1_double_indicator.html", null ],
      [ "xpcc::DoublyLinkedList", "classxpcc_1_1_doubly_linked_list.html", null ],
      [ "xpcc::Ds1631", "classxpcc_1_1_ds1631.html", null ],
      [ "xpcc::Ds18b20", "classxpcc_1_1_ds18b20.html", null ],
      [ "xpcc::allocator::Dynamic", "classxpcc_1_1allocator_1_1_dynamic.html", null ],
      [ "xpcc::DynamicArray", "classxpcc_1_1_dynamic_array.html", null ],
      [ "xpcc::DynamicPostman", "classxpcc_1_1_dynamic_postman.html", null ],
      [ "xpcc::Empty", "structxpcc_1_1_empty.html", null ],
      [ "xpcc::tmp::EnableIf", "structxpcc_1_1tmp_1_1_enable_if.html", null ],
      [ "xpcc::tmp::EnableIfCondition", "structxpcc_1_1tmp_1_1_enable_if_condition.html", null ],
      [ "xpcc::tmp::EnableIfCondition< false, T >", "structxpcc_1_1tmp_1_1_enable_if_condition_3_01false_00_01_t_01_4.html", null ],
      [ "xpcc::Dispatcher::Entry", "classxpcc_1_1_dispatcher_1_1_entry.html", null ],
      [ "xpcc::amnb::ErrorHandler", "structxpcc_1_1amnb_1_1_error_handler.html", null ],
      [ "xpcc::ErrorReport", "classxpcc_1_1_error_report.html", null ],
      [ "std::exception", "classstd_1_1exception.html", null ],
      [ "xpcc::stm32::CanFilter::ExtendedFilterMask", "structxpcc_1_1stm32_1_1_can_filter_1_1_extended_filter_mask.html", null ],
      [ "xpcc::stm32::CanFilter::ExtendedFilterMaskShort", "structxpcc_1_1stm32_1_1_can_filter_1_1_extended_filter_mask_short.html", null ],
      [ "xpcc::stm32::CanFilter::ExtendedIdentifier", "structxpcc_1_1stm32_1_1_can_filter_1_1_extended_identifier.html", null ],
      [ "xpcc::stm32::CanFilter::ExtendedIdentifierShort", "structxpcc_1_1stm32_1_1_can_filter_1_1_extended_identifier_short.html", null ],
      [ "std::ios_base::failure", "classstd_1_1ios__base_1_1failure.html", null ],
      [ "xpcc::fat::File", "classxpcc_1_1fat_1_1_file.html", null ],
      [ "xpcc::fat::FileInfo", "classxpcc_1_1fat_1_1_file_info.html", null ],
      [ "xpcc::fat::FileSystem", "classxpcc_1_1fat_1_1_file_system.html", null ],
      [ "xpcc::atomic::Flag", "classxpcc_1_1atomic_1_1_flag.html", null ],
      [ "xpcc::can::Message::Flags", "structxpcc_1_1can_1_1_message_1_1_flags.html", null ],
      [ "xpcc::at91::Flash", "classxpcc_1_1at91_1_1_flash.html", null ],
      [ "xpcc::accessor::Flash", "classxpcc_1_1accessor_1_1_flash.html", null ],
      [ "std::fstream", "classstd_1_1fstream.html", null ],
      [ "xpcc::Ft245", "classxpcc_1_1_ft245.html", null ],
      [ "xpcc::stm32::GeneralPurposeTimer", "classxpcc_1_1stm32_1_1_general_purpose_timer.html", null ],
      [ "xpcc::GeometricTraits", "structxpcc_1_1_geometric_traits.html", null ],
      [ "xpcc::GeometricTraits< double >", "structxpcc_1_1_geometric_traits_3_01double_01_4.html", null ],
      [ "xpcc::GeometricTraits< float >", "structxpcc_1_1_geometric_traits_3_01float_01_4.html", null ],
      [ "xpcc::GeometricTraits< int16_t >", "structxpcc_1_1_geometric_traits_3_01int16__t_01_4.html", null ],
      [ "xpcc::GeometricTraits< int32_t >", "structxpcc_1_1_geometric_traits_3_01int32__t_01_4.html", null ],
      [ "xpcc::GeometricTraits< int8_t >", "structxpcc_1_1_geometric_traits_3_01int8__t_01_4.html", null ],
      [ "xpcc::GeometricTraits< uint8_t >", "structxpcc_1_1_geometric_traits_3_01uint8__t_01_4.html", null ],
      [ "xpcc::stm32::GpioMode", "structxpcc_1_1stm32_1_1_gpio_mode.html", null ],
      [ "xpcc::stm32::GpioMode< P, N, false >", "structxpcc_1_1stm32_1_1_gpio_mode_3_01_p_00_01_n_00_01false_01_4.html", null ],
      [ "xpcc::GraphicDisplay", "classxpcc_1_1_graphic_display.html", null ],
      [ "xpcc::HCLA12X5", "classxpcc_1_1_h_c_l_a12_x5.html", null ],
      [ "xpcc::Hd44780", "classxpcc_1_1_hd44780.html", null ],
      [ "xpcc::tipc::Header", "structxpcc_1_1tipc_1_1_header.html", null ],
      [ "xpcc::Header", "structxpcc_1_1_header.html", null ],
      [ "xpcc::atxmega::HighResC", "classxpcc_1_1atxmega_1_1_high_res_c.html", null ],
      [ "xpcc::atxmega::HighResD", "classxpcc_1_1atxmega_1_1_high_res_d.html", null ],
      [ "xpcc::atxmega::HighResE", "classxpcc_1_1atxmega_1_1_high_res_e.html", null ],
      [ "xpcc::atxmega::HighResF", "classxpcc_1_1atxmega_1_1_high_res_f.html", null ],
      [ "xpcc::HMC58", "classxpcc_1_1_h_m_c58.html", null ],
      [ "xpcc::HMC5843", "classxpcc_1_1_h_m_c5843.html", null ],
      [ "xpcc::HMC5883l", "classxpcc_1_1_h_m_c5883l.html", null ],
      [ "xpcc::stm32::I2c1", "classxpcc_1_1stm32_1_1_i2c1.html", null ],
      [ "xpcc::stm32::I2c2", "classxpcc_1_1stm32_1_1_i2c2.html", null ],
      [ "xpcc::stm32::I2c3", "classxpcc_1_1stm32_1_1_i2c3.html", null ],
      [ "xpcc::I2cEeprom", "classxpcc_1_1_i2c_eeprom.html", null ],
      [ "xpcc::stm32::CanFilter::Identifier", "structxpcc_1_1stm32_1_1_can_filter_1_1_identifier.html", null ],
      [ "xpcc::stm32::CanFilter::IdentifierShort", "structxpcc_1_1stm32_1_1_can_filter_1_1_identifier_short.html", null ],
      [ "std::ifstream", "classstd_1_1ifstream.html", null ],
      [ "xpcc::led::Indicator", "classxpcc_1_1led_1_1_indicator.html", null ],
      [ "xpcc::atxmega::InputCapture", "classxpcc_1_1atxmega_1_1_input_capture.html", null ],
      [ "xpcc::InterconnectInterface", "classxpcc_1_1_interconnect_interface.html", null ],
      [ "xpcc::InterconnectLink", "structxpcc_1_1_interconnect_link.html", null ],
      [ "xpcc::sab2::Interface", "classxpcc_1_1sab2_1_1_interface.html", null ],
      [ "xpcc::Interface", "classxpcc_1_1_interface.html", null ],
      [ "xpcc::amnb::Interface", "classxpcc_1_1amnb_1_1_interface.html", null ],
      [ "xpcc::sab::Interface", "classxpcc_1_1sab_1_1_interface.html", null ],
      [ "std::invalid_argument", "classstd_1_1invalid__argument.html", null ],
      [ "xpcc::gpio::Invert", "classxpcc_1_1gpio_1_1_invert.html", null ],
      [ "xpcc::IODevice", "classxpcc_1_1_i_o_device.html", null ],
      [ "xpcc::IODeviceWrapper", "classxpcc_1_1_i_o_device_wrapper.html", null ],
      [ "std::ios", "classstd_1_1ios.html", null ],
      [ "std::ios_base", "classstd_1_1ios__base.html", null ],
      [ "xpcc::IOStream", "classxpcc_1_1_i_o_stream.html", null ],
      [ "std::istream", "classstd_1_1istream.html", null ],
      [ "std::istringstream", "classstd_1_1istringstream.html", null ],
      [ "std::map::iterator", "classstd_1_1map_1_1iterator.html", null ],
      [ "std::multiset::iterator", "classstd_1_1multiset_1_1iterator.html", null ],
      [ "std::wstring::iterator", "classstd_1_1wstring_1_1iterator.html", null ],
      [ "std::list::iterator", "classstd_1_1list_1_1iterator.html", null ],
      [ "xpcc::DoublyLinkedList::iterator", "classxpcc_1_1_doubly_linked_list_1_1iterator.html", null ],
      [ "xpcc::DynamicArray::iterator", "classxpcc_1_1_dynamic_array_1_1iterator.html", null ],
      [ "xpcc::LinkedList::iterator", "classxpcc_1_1_linked_list_1_1iterator.html", null ],
      [ "std::basic_string::iterator", "classstd_1_1basic__string_1_1iterator.html", null ],
      [ "std::string::iterator", "classstd_1_1string_1_1iterator.html", null ],
      [ "std::deque::iterator", "classstd_1_1deque_1_1iterator.html", null ],
      [ "std::multimap::iterator", "classstd_1_1multimap_1_1iterator.html", null ],
      [ "std::set::iterator", "classstd_1_1set_1_1iterator.html", null ],
      [ "std::vector::iterator", "classstd_1_1vector_1_1iterator.html", null ],
      [ "xpcc::ITG3200", "classxpcc_1_1_i_t_g3200.html", null ],
      [ "xpcc::Ks0108", "classxpcc_1_1_ks0108.html", null ],
      [ "xpcc::interpolation::Lagrange", "classxpcc_1_1interpolation_1_1_lagrange.html", null ],
      [ "xpcc::led::Led", "classxpcc_1_1led_1_1_led.html", null ],
      [ "std::length_error", "classstd_1_1length__error.html", null ],
      [ "xpcc::Line2D", "classxpcc_1_1_line2_d.html", null ],
      [ "xpcc::interpolation::Linear", "classxpcc_1_1interpolation_1_1_linear.html", null ],
      [ "xpcc::LineSegment2D", "classxpcc_1_1_line_segment2_d.html", null ],
      [ "xpcc::LinkedList", "classxpcc_1_1_linked_list.html", null ],
      [ "std::list", "classstd_1_1list.html", null ],
      [ "xpcc::amnb::Listener", "structxpcc_1_1amnb_1_1_listener.html", null ],
      [ "xpcc::LM75", "classxpcc_1_1_l_m75.html", null ],
      [ "xpcc::Location2D", "classxpcc_1_1_location2_d.html", null ],
      [ "xpcc::atomic::Lock", "classxpcc_1_1atomic_1_1_lock.html", null ],
      [ "xpcc::rtos::Thread::Lock", "classxpcc_1_1rtos_1_1_thread_1_1_lock.html", null ],
      [ "xpcc::log::Logger", "classxpcc_1_1log_1_1_logger.html", null ],
      [ "std::logic_error", "classstd_1_1logic__error.html", null ],
      [ "xpcc::LUDecomposition", "classxpcc_1_1_l_u_decomposition.html", null ],
      [ "xpcc::LUDecomposition::LUSubDecomposition", "classxpcc_1_1_l_u_decomposition_1_1_l_u_sub_decomposition.html", null ],
      [ "xpcc::LUDecomposition::LUSubDecomposition< T, OFFSET, WIDTH, OFFSET >", "classxpcc_1_1_l_u_decomposition_1_1_l_u_sub_decomposition_3_01_t_00_01_o_f_f_s_e_t_00_01_w_i_d_t_h_00_01_o_f_f_s_e_t_01_4.html", null ],
      [ "std::map", "classstd_1_1map.html", null ],
      [ "xpcc::i2c::Master", "classxpcc_1_1i2c_1_1_master.html", null ],
      [ "xpcc::sab::Master", "classxpcc_1_1sab_1_1_master.html", null ],
      [ "xpcc::Matrix", "classxpcc_1_1_matrix.html", null ],
      [ "xpcc::Mcp23s08", "classxpcc_1_1_mcp23s08.html", null ],
      [ "xpcc::Mcp23s17", "classxpcc_1_1_mcp23s17.html", null ],
      [ "xpcc::Mcp2515", "classxpcc_1_1_mcp2515.html", null ],
      [ "xpcc::Mcp4922", "classxpcc_1_1_mcp4922.html", null ],
      [ "xpcc::filter::Median", "classxpcc_1_1filter_1_1_median.html", null ],
      [ "xpcc::can::Message", "structxpcc_1_1can_1_1_message.html", null ],
      [ "xpcc::MovingAverage", "classxpcc_1_1_moving_average.html", null ],
      [ "std::multimap", "classstd_1_1multimap.html", null ],
      [ "std::multiset", "classstd_1_1multiset.html", null ],
      [ "xpcc::rtos::Mutex", "classxpcc_1_1rtos_1_1_mutex.html", null ],
      [ "xpcc::rtos::MutexGuard", "classxpcc_1_1rtos_1_1_mutex_guard.html", null ],
      [ "my_namespace::MyClass", "classmy__namespace_1_1_my_class.html", null ],
      [ "xpcc::gpio::Nibble", "classxpcc_1_1gpio_1_1_nibble.html", null ],
      [ "xpcc::DoublyLinkedList::Node", "structxpcc_1_1_doubly_linked_list_1_1_node.html", null ],
      [ "xpcc::LinkedList::Node", "structxpcc_1_1_linked_list_1_1_node.html", null ],
      [ "xpcc::amnb::Node", "classxpcc_1_1amnb_1_1_node.html", null ],
      [ "xpcc::Nokia6610", "classxpcc_1_1_nokia6610.html", null ],
      [ "xpcc::tmp::NullType", "classxpcc_1_1tmp_1_1_null_type.html", null ],
      [ "std::ofstream", "classstd_1_1ofstream.html", null ],
      [ "std::ostream", "classstd_1_1ostream.html", null ],
      [ "std::ostringstream", "classstd_1_1ostringstream.html", null ],
      [ "std::out_of_range", "classstd_1_1out__of__range.html", null ],
      [ "std::overflow_error", "classstd_1_1overflow__error.html", null ],
      [ "xpcc::Pair", "classxpcc_1_1_pair.html", null ],
      [ "xpcc::Pid::Parameter", "structxpcc_1_1_pid_1_1_parameter.html", null ],
      [ "xpcc::SCurveController::Parameter", "structxpcc_1_1_s_curve_controller_1_1_parameter.html", null ],
      [ "xpcc::PeriodicTimer", "classxpcc_1_1_periodic_timer.html", null ],
      [ "xpcc::fat::PhysicalVolume", "classxpcc_1_1fat_1_1_physical_volume.html", null ],
      [ "xpcc::Pid", "classxpcc_1_1_pid.html", null ],
      [ "xpcc::at91::Pio", "classxpcc_1_1at91_1_1_pio.html", null ],
      [ "xpcc::at91::Pit", "classxpcc_1_1at91_1_1_pit.html", null ],
      [ "xpcc::PointSet2D", "classxpcc_1_1_point_set2_d.html", null ],
      [ "xpcc::Polygon2D", "classxpcc_1_1_polygon2_d.html", null ],
      [ "xpcc::gpio::Port", "classxpcc_1_1gpio_1_1_port.html", null ],
      [ "xpcc::Postman", "classxpcc_1_1_postman.html", null ],
      [ "xpcc::Pow", "classxpcc_1_1_pow.html", null ],
      [ "xpcc::Pow< B, 0 >", "classxpcc_1_1_pow_3_01_b_00_010_01_4.html", null ],
      [ "xpcc::log::Prefix", "classxpcc_1_1log_1_1_prefix.html", null ],
      [ "std::priority_queue", "classstd_1_1priority__queue.html", null ],
      [ "xpcc::pt::Protothread", "classxpcc_1_1pt_1_1_protothread.html", null ],
      [ "xpcc::led::Pulse", "classxpcc_1_1led_1_1_pulse.html", null ],
      [ "xpcc::at91::Pwm", "classxpcc_1_1at91_1_1_pwm.html", null ],
      [ "xpcc::at91::PwmChannel", "classxpcc_1_1at91_1_1_pwm_channel.html", null ],
      [ "xpcc::Quaternion", "classxpcc_1_1_quaternion.html", null ],
      [ "xpcc::Queue", "classxpcc_1_1_queue.html", null ],
      [ "xpcc::atomic::Queue", "classxpcc_1_1atomic_1_1_queue.html", null ],
      [ "xpcc::rtos::Queue< T >", "classxpcc_1_1rtos_1_1_queue.html", null ],
      [ "std::queue", "classstd_1_1queue.html", null ],
      [ "xpcc::rtos::QueueBase", "classxpcc_1_1rtos_1_1_queue_base.html", null ],
      [ "xpcc::accessor::Ram", "classxpcc_1_1accessor_1_1_ram.html", null ],
      [ "xpcc::filter::Ramp", "classxpcc_1_1filter_1_1_ramp.html", null ],
      [ "std::range_error", "classstd_1_1range__error.html", null ],
      [ "xpcc::allocator::Block::rebind", "structxpcc_1_1allocator_1_1_block_1_1rebind.html", null ],
      [ "xpcc::allocator::Dynamic::rebind", "structxpcc_1_1allocator_1_1_dynamic_1_1rebind.html", null ],
      [ "xpcc::allocator::Static::rebind", "structxpcc_1_1allocator_1_1_static_1_1rebind.html", null ],
      [ "xpcc::CanConnector::ReceiveListItem", "classxpcc_1_1_can_connector_1_1_receive_list_item.html", null ],
      [ "xpcc::tipc::Receiver", "classxpcc_1_1tipc_1_1_receiver.html", null ],
      [ "xpcc::tipc::ReceiverSocket", "classxpcc_1_1tipc_1_1_receiver_socket.html", null ],
      [ "unittest::Reporter", "classunittest_1_1_reporter.html", null ],
      [ "xpcc::amnb::Response", "classxpcc_1_1amnb_1_1_response.html", null ],
      [ "xpcc::sab::Response", "classxpcc_1_1sab_1_1_response.html", null ],
      [ "xpcc::ResponseCallback", "classxpcc_1_1_response_callback.html", null ],
      [ "xpcc::ResponseHandle", "classxpcc_1_1_response_handle.html", null ],
      [ "std::multimap::reverse_iterator", "classstd_1_1multimap_1_1reverse__iterator.html", null ],
      [ "std::string::reverse_iterator", "classstd_1_1string_1_1reverse__iterator.html", null ],
      [ "std::vector::reverse_iterator", "classstd_1_1vector_1_1reverse__iterator.html", null ],
      [ "std::set::reverse_iterator", "classstd_1_1set_1_1reverse__iterator.html", null ],
      [ "std::deque::reverse_iterator", "classstd_1_1deque_1_1reverse__iterator.html", null ],
      [ "std::list::reverse_iterator", "classstd_1_1list_1_1reverse__iterator.html", null ],
      [ "std::basic_string::reverse_iterator", "classstd_1_1basic__string_1_1reverse__iterator.html", null ],
      [ "std::wstring::reverse_iterator", "classstd_1_1wstring_1_1reverse__iterator.html", null ],
      [ "std::map::reverse_iterator", "classstd_1_1map_1_1reverse__iterator.html", null ],
      [ "std::multiset::reverse_iterator", "classstd_1_1multiset_1_1reverse__iterator.html", null ],
      [ "xpcc::led::Rgb", "classxpcc_1_1led_1_1_rgb.html", null ],
      [ "xpcc::LUDecomposition::RowOperation", "classxpcc_1_1_l_u_decomposition_1_1_row_operation.html", null ],
      [ "xpcc::LUDecomposition::RowOperation< T, 0 >", "classxpcc_1_1_l_u_decomposition_1_1_row_operation_3_01_t_00_010_01_4.html", null ],
      [ "std::runtime_error", "classstd_1_1runtime__error.html", null ],
      [ "xpcc::tmp::SameType", "structxpcc_1_1tmp_1_1_same_type.html", null ],
      [ "xpcc::tmp::SameType< T, T >", "structxpcc_1_1tmp_1_1_same_type_3_01_t_00_01_t_01_4.html", null ],
      [ "xpcc::Saturated", "classxpcc_1_1_saturated.html", null ],
      [ "xpcc::rtos::Scheduler", "classxpcc_1_1rtos_1_1_scheduler.html", null ],
      [ "xpcc::Scheduler", "classxpcc_1_1_scheduler.html", null ],
      [ "xpcc::SCP1000", "classxpcc_1_1_s_c_p1000.html", null ],
      [ "xpcc::SCurveController", "classxpcc_1_1_s_curve_controller.html", null ],
      [ "xpcc::SCurveGenerator", "classxpcc_1_1_s_curve_generator.html", null ],
      [ "xpcc::tmp::Select", "structxpcc_1_1tmp_1_1_select.html", null ],
      [ "xpcc::tmp::Select< false, T, U >", "structxpcc_1_1tmp_1_1_select_3_01false_00_01_t_00_01_u_01_4.html", null ],
      [ "xpcc::pt::Semaphore", "classxpcc_1_1pt_1_1_semaphore.html", null ],
      [ "xpcc::rtos::Semaphore", "classxpcc_1_1rtos_1_1_semaphore.html", null ],
      [ "xpcc::rtos::SemaphoreBase", "classxpcc_1_1rtos_1_1_semaphore_base.html", null ],
      [ "xpcc::CanConnector::SendListItem", "classxpcc_1_1_can_connector_1_1_send_list_item.html", null ],
      [ "xpcc::pc::SerialInterface", "classxpcc_1_1pc_1_1_serial_interface.html", null ],
      [ "xpcc::pc::SerialPort", "classxpcc_1_1pc_1_1_serial_port.html", null ],
      [ "std::set", "classstd_1_1set.html", null ],
      [ "xpcc::ShiftRegisterInput", "classxpcc_1_1_shift_register_input.html", null ],
      [ "xpcc::ShiftRegisterOutput", "classxpcc_1_1_shift_register_output.html", null ],
      [ "xpcc::sab::Slave", "classxpcc_1_1sab_1_1_slave.html", null ],
      [ "xpcc::SmartPointer", "classxpcc_1_1_smart_pointer.html", null ],
      [ "xpcc::SoftwareI2C", "classxpcc_1_1_software_i2_c.html", null ],
      [ "xpcc::SoftwareOneWire", "classxpcc_1_1_software_one_wire.html", null ],
      [ "xpcc::SoftwareSpi", "classxpcc_1_1_software_spi.html", null ],
      [ "xpcc::at91::Spi0", "classxpcc_1_1at91_1_1_spi0.html", null ],
      [ "xpcc::stm32::Spi1", "classxpcc_1_1stm32_1_1_spi1.html", null ],
      [ "xpcc::at91::Spi1", "classxpcc_1_1at91_1_1_spi1.html", null ],
      [ "xpcc::stm32::Spi2", "classxpcc_1_1stm32_1_1_spi2.html", null ],
      [ "xpcc::stm32::Spi3", "classxpcc_1_1stm32_1_1_spi3.html", null ],
      [ "xpcc::SpiMaster", "classxpcc_1_1_spi_master.html", null ],
      [ "xpcc::atmega::SpiMaster", "classxpcc_1_1atmega_1_1_spi_master.html", null ],
      [ "xpcc::atxmega::SpiMasterC", "classxpcc_1_1atxmega_1_1_spi_master_c.html", null ],
      [ "xpcc::atxmega::SpiMasterD", "classxpcc_1_1atxmega_1_1_spi_master_d.html", null ],
      [ "xpcc::atxmega::SpiMasterE", "classxpcc_1_1atxmega_1_1_spi_master_e.html", null ],
      [ "xpcc::atxmega::SpiMasterF", "classxpcc_1_1atxmega_1_1_spi_master_f.html", null ],
      [ "xpcc::SpiRam", "classxpcc_1_1_spi_ram.html", null ],
      [ "xpcc::at91::Ssc", "classxpcc_1_1at91_1_1_ssc.html", null ],
      [ "xpcc::St7036", "classxpcc_1_1_st7036.html", null ],
      [ "xpcc::St7565", "classxpcc_1_1_st7565.html", null ],
      [ "std::stack", "classstd_1_1stack.html", null ],
      [ "xpcc::Stack", "classxpcc_1_1_stack.html", null ],
      [ "xpcc::stm32::CanFilter::StandardFilterMask", "structxpcc_1_1stm32_1_1_can_filter_1_1_standard_filter_mask.html", null ],
      [ "xpcc::stm32::CanFilter::StandardFilterMaskShort", "structxpcc_1_1stm32_1_1_can_filter_1_1_standard_filter_mask_short.html", null ],
      [ "xpcc::stm32::CanFilter::StandardIdentifier", "structxpcc_1_1stm32_1_1_can_filter_1_1_standard_identifier.html", null ],
      [ "xpcc::stm32::CanFilter::StandardIdentifierShort", "structxpcc_1_1stm32_1_1_can_filter_1_1_standard_identifier_short.html", null ],
      [ "xpcc::allocator::Static", "classxpcc_1_1allocator_1_1_static.html", null ],
      [ "xpcc::tmp::static_assert_test", "structxpcc_1_1tmp_1_1static__assert__test.html", null ],
      [ "xpcc::tmp::STATIC_ASSERTION_FAILURE< true >", "structxpcc_1_1tmp_1_1_s_t_a_t_i_c___a_s_s_e_r_t_i_o_n___f_a_i_l_u_r_e_3_01true_01_4.html", null ],
      [ "xpcc::log::StdColour", "classxpcc_1_1log_1_1_std_colour.html", null ],
      [ "std::string", "classstd_1_1string.html", null ],
      [ "std::stringstream", "classstd_1_1stringstream.html", null ],
      [ "xpcc::log::Style", "classxpcc_1_1log_1_1_style.html", null ],
      [ "xpcc::log::StyleWrapper", "classxpcc_1_1log_1_1_style_wrapper.html", null ],
      [ "xpcc::tmp::SuperSubclass", "structxpcc_1_1tmp_1_1_super_subclass.html", null ],
      [ "xpcc::tmp::SuperSubclass< T, void >", "structxpcc_1_1tmp_1_1_super_subclass_3_01_t_00_01void_01_4.html", null ],
      [ "xpcc::tmp::SuperSubclass< void, U >", "structxpcc_1_1tmp_1_1_super_subclass_3_01void_00_01_u_01_4.html", null ],
      [ "xpcc::tmp::SuperSubclass< void, void >", "structxpcc_1_1tmp_1_1_super_subclass_3_01void_00_01void_01_4.html", null ],
      [ "xpcc::tmp::SuperSubclassStrict", "structxpcc_1_1tmp_1_1_super_subclass_strict.html", null ],
      [ "xpcc::i2c::SyncMaster", "classxpcc_1_1i2c_1_1_sync_master.html", null ],
      [ "xpcc::stm32::SysTickTimer", "classxpcc_1_1stm32_1_1_sys_tick_timer.html", null ],
      [ "xpcc::Task", "classxpcc_1_1_task.html", null ],
      [ "xpcc::Scheduler::Task", "classxpcc_1_1_scheduler_1_1_task.html", null ],
      [ "xpcc::Scheduler::TaskListItem", "structxpcc_1_1_scheduler_1_1_task_list_item.html", null ],
      [ "xpcc::pc::Terminal", "classxpcc_1_1pc_1_1_terminal.html", null ],
      [ "unittest::TestSuite", "classunittest_1_1_test_suite.html", null ],
      [ "xpcc::rtos::Thread", "classxpcc_1_1rtos_1_1_thread.html", null ],
      [ "xpcc::Timeout", "classxpcc_1_1_timeout.html", null ],
      [ "xpcc::at91::Timer", "classxpcc_1_1at91_1_1_timer.html", null ],
      [ "xpcc::at91::Timer0", "classxpcc_1_1at91_1_1_timer0.html", null ],
      [ "xpcc::stm32::Timer1", "classxpcc_1_1stm32_1_1_timer1.html", null ],
      [ "xpcc::at91::Timer1", "classxpcc_1_1at91_1_1_timer1.html", null ],
      [ "xpcc::at91::Timer2", "classxpcc_1_1at91_1_1_timer2.html", null ],
      [ "xpcc::stm32::Timer2", "classxpcc_1_1stm32_1_1_timer2.html", null ],
      [ "xpcc::stm32::Timer3", "classxpcc_1_1stm32_1_1_timer3.html", null ],
      [ "xpcc::stm32::Timer4", "classxpcc_1_1stm32_1_1_timer4.html", null ],
      [ "xpcc::stm32::Timer5", "classxpcc_1_1stm32_1_1_timer5.html", null ],
      [ "xpcc::stm32::Timer6", "classxpcc_1_1stm32_1_1_timer6.html", null ],
      [ "xpcc::stm32::Timer7", "classxpcc_1_1stm32_1_1_timer7.html", null ],
      [ "xpcc::stm32::Timer8", "classxpcc_1_1stm32_1_1_timer8.html", null ],
      [ "xpcc::atxmega::TimerC0", "classxpcc_1_1atxmega_1_1_timer_c0.html", null ],
      [ "xpcc::atxmega::TimerC1", "classxpcc_1_1atxmega_1_1_timer_c1.html", null ],
      [ "xpcc::atxmega::TimerD0", "classxpcc_1_1atxmega_1_1_timer_d0.html", null ],
      [ "xpcc::atxmega::TimerD1", "classxpcc_1_1atxmega_1_1_timer_d1.html", null ],
      [ "xpcc::atxmega::TimerE0", "classxpcc_1_1atxmega_1_1_timer_e0.html", null ],
      [ "xpcc::atxmega::TimerE1", "classxpcc_1_1atxmega_1_1_timer_e1.html", null ],
      [ "xpcc::atxmega::TimerF0", "classxpcc_1_1atxmega_1_1_timer_f0.html", null ],
      [ "xpcc::atxmega::TimerF1", "classxpcc_1_1atxmega_1_1_timer_f1.html", null ],
      [ "xpcc::atxmega::TimerInterruptC0", "classxpcc_1_1atxmega_1_1_timer_interrupt_c0.html", null ],
      [ "xpcc::atxmega::TimerInterruptC1", "classxpcc_1_1atxmega_1_1_timer_interrupt_c1.html", null ],
      [ "xpcc::atxmega::TimerInterruptD0", "classxpcc_1_1atxmega_1_1_timer_interrupt_d0.html", null ],
      [ "xpcc::atxmega::TimerInterruptD1", "classxpcc_1_1atxmega_1_1_timer_interrupt_d1.html", null ],
      [ "xpcc::atxmega::TimerInterruptE0", "classxpcc_1_1atxmega_1_1_timer_interrupt_e0.html", null ],
      [ "xpcc::atxmega::TimerInterruptE1", "classxpcc_1_1atxmega_1_1_timer_interrupt_e1.html", null ],
      [ "xpcc::atxmega::TimerInterruptF0", "classxpcc_1_1atxmega_1_1_timer_interrupt_f0.html", null ],
      [ "xpcc::atxmega::TimerInterruptF1", "classxpcc_1_1atxmega_1_1_timer_interrupt_f1.html", null ],
      [ "xpcc::Timestamp", "classxpcc_1_1_timestamp.html", null ],
      [ "xpcc::TipcConnector", "classxpcc_1_1_tipc_connector.html", null ],
      [ "xpcc::TLC594X", "classxpcc_1_1_t_l_c594_x.html", null ],
      [ "xpcc::led::TLC594XLed", "classxpcc_1_1led_1_1_t_l_c594_x_led.html", null ],
      [ "xpcc::TMP102", "classxpcc_1_1_t_m_p102.html", null ],
      [ "xpcc::tipc::Transmitter", "classxpcc_1_1tipc_1_1_transmitter.html", null ],
      [ "xpcc::sab::Transmitter", "classxpcc_1_1sab_1_1_transmitter.html", null ],
      [ "xpcc::amnb::Transmitter", "classxpcc_1_1amnb_1_1_transmitter.html", null ],
      [ "xpcc::tipc::TransmitterSocket", "classxpcc_1_1tipc_1_1_transmitter_socket.html", null ],
      [ "xpcc::at91::Twi", "classxpcc_1_1at91_1_1_twi.html", null ],
      [ "xpcc::at91::Uart0", "classxpcc_1_1at91_1_1_uart0.html", null ],
      [ "xpcc::Uart0", "classxpcc_1_1_uart0.html", null ],
      [ "xpcc::Uart1", "classxpcc_1_1_uart1.html", null ],
      [ "xpcc::at91::Uart1", "classxpcc_1_1at91_1_1_uart1.html", null ],
      [ "xpcc::Uart2", "classxpcc_1_1_uart2.html", null ],
      [ "xpcc::Uart3", "classxpcc_1_1_uart3.html", null ],
      [ "xpcc::stm32::Uart4", "classxpcc_1_1stm32_1_1_uart4.html", null ],
      [ "xpcc::stm32::Uart5", "classxpcc_1_1stm32_1_1_uart5.html", null ],
      [ "xpcc::stm32::UartBase", "classxpcc_1_1stm32_1_1_uart_base.html", null ],
      [ "xpcc::atxmega::UartBase", "classxpcc_1_1atxmega_1_1_uart_base.html", null ],
      [ "UartInterface", "class_uart_interface.html", null ],
      [ "xpcc::atxmega::UartSpiMasterC0", "classxpcc_1_1atxmega_1_1_uart_spi_master_c0.html", null ],
      [ "xpcc::atxmega::UartSpiMasterC1", "classxpcc_1_1atxmega_1_1_uart_spi_master_c1.html", null ],
      [ "xpcc::atxmega::UartSpiMasterD0", "classxpcc_1_1atxmega_1_1_uart_spi_master_d0.html", null ],
      [ "xpcc::atxmega::UartSpiMasterD1", "classxpcc_1_1atxmega_1_1_uart_spi_master_d1.html", null ],
      [ "xpcc::atxmega::UartSpiMasterE0", "classxpcc_1_1atxmega_1_1_uart_spi_master_e0.html", null ],
      [ "xpcc::atxmega::UartSpiMasterE1", "classxpcc_1_1atxmega_1_1_uart_spi_master_e1.html", null ],
      [ "xpcc::atxmega::UartSpiMasterF0", "classxpcc_1_1atxmega_1_1_uart_spi_master_f0.html", null ],
      [ "xpcc::atxmega::UartSpiMasterF1", "classxpcc_1_1atxmega_1_1_uart_spi_master_f1.html", null ],
      [ "std::underflow_error", "classstd_1_1underflow__error.html", null ],
      [ "xpcc::atomic::Unlock", "classxpcc_1_1atomic_1_1_unlock.html", null ],
      [ "xpcc::gpio::Unused", "classxpcc_1_1gpio_1_1_unused.html", null ],
      [ "xpcc::stm32::Usart1", "classxpcc_1_1stm32_1_1_usart1.html", null ],
      [ "xpcc::stm32::Usart2", "classxpcc_1_1stm32_1_1_usart2.html", null ],
      [ "xpcc::stm32::Usart3", "classxpcc_1_1stm32_1_1_usart3.html", null ],
      [ "xpcc::stm32::Usart6", "classxpcc_1_1stm32_1_1_usart6.html", null ],
      [ "xpcc::at91::Usb", "classxpcc_1_1at91_1_1_usb.html", null ],
      [ "std::valarray", "classstd_1_1valarray.html", null ],
      [ "xpcc::Vector", "classxpcc_1_1_vector.html", null ],
      [ "std::vector", "classstd_1_1vector.html", null ],
      [ "xpcc::Vector< T, 1 >", "classxpcc_1_1_vector_3_01_t_00_011_01_4.html", null ],
      [ "xpcc::Vector< T, 2 >", "classxpcc_1_1_vector_3_01_t_00_012_01_4.html", null ],
      [ "xpcc::Vector< T, 3 >", "classxpcc_1_1_vector_3_01_t_00_013_01_4.html", null ],
      [ "xpcc::Vector< T, 4 >", "classxpcc_1_1_vector_3_01_t_00_014_01_4.html", null ],
      [ "xpcc::lpc::Vic", "classxpcc_1_1lpc_1_1_vic.html", null ],
      [ "xpcc::at91::Watchdog", "classxpcc_1_1at91_1_1_watchdog.html", null ],
      [ "xpcc::atxmega::WaveformC", "classxpcc_1_1atxmega_1_1_waveform_c.html", null ],
      [ "xpcc::atxmega::WaveformD", "classxpcc_1_1atxmega_1_1_waveform_d.html", null ],
      [ "xpcc::atxmega::WaveformE", "classxpcc_1_1atxmega_1_1_waveform_e.html", null ],
      [ "xpcc::atxmega::WaveformF", "classxpcc_1_1atxmega_1_1_waveform_f.html", null ],
      [ "std::wfstream", "classstd_1_1wfstream.html", null ],
      [ "std::wifstream", "classstd_1_1wifstream.html", null ],
      [ "std::wios", "classstd_1_1wios.html", null ],
      [ "std::wistream", "classstd_1_1wistream.html", null ],
      [ "std::wistringstream", "classstd_1_1wistringstream.html", null ],
      [ "std::wofstream", "classstd_1_1wofstream.html", null ],
      [ "std::wostream", "classstd_1_1wostream.html", null ],
      [ "std::wostringstream", "classstd_1_1wostringstream.html", null ],
      [ "xpcc::CharacterDisplay::Writer", "classxpcc_1_1_character_display_1_1_writer.html", null ],
      [ "xpcc::GraphicDisplay::Writer", "classxpcc_1_1_graphic_display_1_1_writer.html", null ],
      [ "std::wstring", "classstd_1_1wstring.html", null ],
      [ "std::wstringstream", "classstd_1_1wstringstream.html", null ]
    ] ],
    [ "Class Index", "classes.html", null ],
    [ "Class Hierarchy", "hierarchy.html", [
      [ "_INTAT", "union___i_n_t_a_t.html", null ],
      [ "_PCLKSEL0", "union___p_c_l_k_s_e_l0.html", null ],
      [ "_PCLKSEL1", "union___p_c_l_k_s_e_l1.html", null ],
      [ "_PCONP", "union___p_c_o_n_p.html", null ],
      [ "_TxCCR", "struct___tx_c_c_r.html", null ],
      [ "_TxCTCR", "struct___tx_c_t_c_r.html", null ],
      [ "_TxEMR", "struct___tx_e_m_r.html", null ],
      [ "_TxIR", "struct___tx_i_r.html", null ],
      [ "_TxMCR", "union___tx_m_c_r.html", null ],
      [ "_TxTCR", "struct___tx_t_c_r.html", null ],
      [ "AbortContext", "struct_abort_context.html", null ],
      [ "xpcc::sab::Action", "structxpcc_1_1sab_1_1_action.html", null ],
      [ "xpcc::amnb::Action", "structxpcc_1_1amnb_1_1_action.html", null ],
      [ "xpcc::ActionCallback", "classxpcc_1_1_action_callback.html", null ],
      [ "xpcc::AD840x", "classxpcc_1_1_a_d840x.html", null ],
      [ "xpcc::at91::Adc", "classxpcc_1_1at91_1_1_adc.html", null ],
      [ "xpcc::atmega::Adc", "classxpcc_1_1atmega_1_1_adc.html", [
        [ "xpcc::atmega::AdcInterrupt", "classxpcc_1_1atmega_1_1_adc_interrupt.html", null ]
      ] ],
      [ "xpcc::atxmega::AdcA", "classxpcc_1_1atxmega_1_1_adc_a.html", [
        [ "xpcc::atxmega::AdcChannelA0", "classxpcc_1_1atxmega_1_1_adc_channel_a0.html", null ],
        [ "xpcc::atxmega::AdcChannelA1", "classxpcc_1_1atxmega_1_1_adc_channel_a1.html", null ],
        [ "xpcc::atxmega::AdcChannelA2", "classxpcc_1_1atxmega_1_1_adc_channel_a2.html", null ],
        [ "xpcc::atxmega::AdcChannelA3", "classxpcc_1_1atxmega_1_1_adc_channel_a3.html", null ]
      ] ],
      [ "xpcc::atxmega::AdcB", "classxpcc_1_1atxmega_1_1_adc_b.html", [
        [ "xpcc::atxmega::AdcChannelB0", "classxpcc_1_1atxmega_1_1_adc_channel_b0.html", null ],
        [ "xpcc::atxmega::AdcChannelB1", "classxpcc_1_1atxmega_1_1_adc_channel_b1.html", null ],
        [ "xpcc::atxmega::AdcChannelB2", "classxpcc_1_1atxmega_1_1_adc_channel_b2.html", null ],
        [ "xpcc::atxmega::AdcChannelB3", "classxpcc_1_1atxmega_1_1_adc_channel_b3.html", null ]
      ] ],
      [ "xpcc::at91::Aic", "classxpcc_1_1at91_1_1_aic.html", null ],
      [ "std::allocator", "classstd_1_1allocator.html", null ],
      [ "xpcc::allocator::AllocatorBase", "classxpcc_1_1allocator_1_1_allocator_base.html", null ],
      [ "xpcc::allocator::AllocatorBase< T >", "classxpcc_1_1allocator_1_1_allocator_base.html", [
        [ "xpcc::allocator::Block", "classxpcc_1_1allocator_1_1_block.html", null ],
        [ "xpcc::allocator::Dynamic", "classxpcc_1_1allocator_1_1_dynamic.html", null ],
        [ "xpcc::allocator::Static", "classxpcc_1_1allocator_1_1_static.html", null ]
      ] ],
      [ "xpcc::stm32::AlternateMode", "structxpcc_1_1stm32_1_1_alternate_mode.html", null ],
      [ "xpcc::stm32::AlternateMode< P, N, false >", "structxpcc_1_1stm32_1_1_alternate_mode_3_01_p_00_01_n_00_01false_01_4.html", null ],
      [ "xpcc::atmega::AnalogSensors", "classxpcc_1_1atmega_1_1_analog_sensors.html", null ],
      [ "xpcc::Angle", "classxpcc_1_1_angle.html", null ],
      [ "xpcc::ArithmeticTraits", "structxpcc_1_1_arithmetic_traits.html", null ],
      [ "xpcc::ArithmeticTraits< char >", "structxpcc_1_1_arithmetic_traits_3_01char_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< double >", "structxpcc_1_1_arithmetic_traits_3_01double_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< float >", "structxpcc_1_1_arithmetic_traits_3_01float_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< int16_t >", "structxpcc_1_1_arithmetic_traits_3_01int16__t_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< int32_t >", "structxpcc_1_1_arithmetic_traits_3_01int32__t_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< int64_t >", "structxpcc_1_1_arithmetic_traits_3_01int64__t_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< signed char >", "structxpcc_1_1_arithmetic_traits_3_01signed_01char_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< uint16_t >", "structxpcc_1_1_arithmetic_traits_3_01uint16__t_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< uint32_t >", "structxpcc_1_1_arithmetic_traits_3_01uint32__t_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< uint64_t >", "structxpcc_1_1_arithmetic_traits_3_01uint64__t_01_4.html", null ],
      [ "xpcc::ArithmeticTraits< unsigned char >", "structxpcc_1_1_arithmetic_traits_3_01unsigned_01char_01_4.html", null ],
      [ "xpcc::At45db0x1d", "classxpcc_1_1_at45db0x1d.html", null ],
      [ "std::auto_ptr", "classstd_1_1auto__ptr.html", null ],
      [ "xpcc::BackendInterface", "classxpcc_1_1_backend_interface.html", [
        [ "xpcc::CanConnector", "classxpcc_1_1_can_connector.html", null ],
        [ "xpcc::TipcConnector", "classxpcc_1_1_tipc_connector.html", null ]
      ] ],
      [ "std::basic_fstream< char >", "classstd_1_1basic__fstream.html", [
        [ "std::fstream", "classstd_1_1fstream.html", null ]
      ] ],
      [ "std::basic_fstream< wchar_t >", "classstd_1_1basic__fstream.html", [
        [ "std::wfstream", "classstd_1_1wfstream.html", null ]
      ] ],
      [ "std::basic_ifstream< char >", "classstd_1_1basic__ifstream.html", [
        [ "std::ifstream", "classstd_1_1ifstream.html", null ]
      ] ],
      [ "std::basic_ifstream< wchar_t >", "classstd_1_1basic__ifstream.html", [
        [ "std::wifstream", "classstd_1_1wifstream.html", null ]
      ] ],
      [ "std::basic_ios< Char >", "classstd_1_1basic__ios.html", [
        [ "std::basic_istream", "classstd_1_1basic__istream.html", null ],
        [ "std::basic_ostream", "classstd_1_1basic__ostream.html", null ]
      ] ],
      [ "std::basic_ios< char >", "classstd_1_1basic__ios.html", [
        [ "std::ios", "classstd_1_1ios.html", null ]
      ] ],
      [ "std::basic_ios< wchar_t >", "classstd_1_1basic__ios.html", [
        [ "std::wios", "classstd_1_1wios.html", null ]
      ] ],
      [ "std::basic_iostream< Char >", "classstd_1_1basic__iostream.html", [
        [ "std::basic_fstream", "classstd_1_1basic__fstream.html", null ],
        [ "std::basic_stringstream", "classstd_1_1basic__stringstream.html", null ]
      ] ],
      [ "std::basic_istream< Char >", "classstd_1_1basic__istream.html", [
        [ "std::basic_ifstream", "classstd_1_1basic__ifstream.html", null ],
        [ "std::basic_iostream", "classstd_1_1basic__iostream.html", null ],
        [ "std::basic_istringstream", "classstd_1_1basic__istringstream.html", null ]
      ] ],
      [ "std::basic_istream< char >", "classstd_1_1basic__istream.html", [
        [ "std::istream", "classstd_1_1istream.html", null ]
      ] ],
      [ "std::basic_istream< wchar_t >", "classstd_1_1basic__istream.html", [
        [ "std::wistream", "classstd_1_1wistream.html", null ]
      ] ],
      [ "std::basic_istringstream< char >", "classstd_1_1basic__istringstream.html", [
        [ "std::istringstream", "classstd_1_1istringstream.html", null ]
      ] ],
      [ "std::basic_istringstream< wchar_t >", "classstd_1_1basic__istringstream.html", [
        [ "std::wistringstream", "classstd_1_1wistringstream.html", null ]
      ] ],
      [ "std::basic_ofstream< char >", "classstd_1_1basic__ofstream.html", [
        [ "std::ofstream", "classstd_1_1ofstream.html", null ]
      ] ],
      [ "std::basic_ofstream< wchar_t >", "classstd_1_1basic__ofstream.html", [
        [ "std::wofstream", "classstd_1_1wofstream.html", null ]
      ] ],
      [ "std::basic_ostream< Char >", "classstd_1_1basic__ostream.html", [
        [ "std::basic_iostream", "classstd_1_1basic__iostream.html", null ],
        [ "std::basic_ofstream", "classstd_1_1basic__ofstream.html", null ],
        [ "std::basic_ostringstream", "classstd_1_1basic__ostringstream.html", null ]
      ] ],
      [ "std::basic_ostream< char >", "classstd_1_1basic__ostream.html", [
        [ "std::ostream", "classstd_1_1ostream.html", null ]
      ] ],
      [ "std::basic_ostream< wchar_t >", "classstd_1_1basic__ostream.html", [
        [ "std::wostream", "classstd_1_1wostream.html", null ]
      ] ],
      [ "std::basic_ostringstream< char >", "classstd_1_1basic__ostringstream.html", [
        [ "std::ostringstream", "classstd_1_1ostringstream.html", null ]
      ] ],
      [ "std::basic_ostringstream< wchar_t >", "classstd_1_1basic__ostringstream.html", [
        [ "std::wostringstream", "classstd_1_1wostringstream.html", null ]
      ] ],
      [ "std::basic_string", "classstd_1_1basic__string.html", null ],
      [ "std::basic_string< char >", "classstd_1_1basic__string.html", [
        [ "std::string", "classstd_1_1string.html", null ]
      ] ],
      [ "std::basic_string< wchar_t >", "classstd_1_1basic__string.html", [
        [ "std::wstring", "classstd_1_1wstring.html", null ]
      ] ],
      [ "std::basic_stringstream< char >", "classstd_1_1basic__stringstream.html", [
        [ "std::stringstream", "classstd_1_1stringstream.html", null ]
      ] ],
      [ "std::basic_stringstream< wchar_t >", "classstd_1_1basic__stringstream.html", [
        [ "std::wstringstream", "classstd_1_1wstringstream.html", null ]
      ] ],
      [ "xpcc::stm32::BasicTimer", "classxpcc_1_1stm32_1_1_basic_timer.html", [
        [ "xpcc::stm32::GeneralPurposeTimer", "classxpcc_1_1stm32_1_1_general_purpose_timer.html", [
          [ "xpcc::stm32::AdvancedControlTimer", "classxpcc_1_1stm32_1_1_advanced_control_timer.html", [
            [ "xpcc::stm32::Timer1", "classxpcc_1_1stm32_1_1_timer1.html", null ],
            [ "xpcc::stm32::Timer8", "classxpcc_1_1stm32_1_1_timer8.html", null ]
          ] ],
          [ "xpcc::stm32::Timer2", "classxpcc_1_1stm32_1_1_timer2.html", null ],
          [ "xpcc::stm32::Timer3", "classxpcc_1_1stm32_1_1_timer3.html", null ],
          [ "xpcc::stm32::Timer4", "classxpcc_1_1stm32_1_1_timer4.html", null ],
          [ "xpcc::stm32::Timer5", "classxpcc_1_1stm32_1_1_timer5.html", null ]
        ] ],
        [ "xpcc::stm32::Timer6", "classxpcc_1_1stm32_1_1_timer6.html", null ],
        [ "xpcc::stm32::Timer7", "classxpcc_1_1stm32_1_1_timer7.html", null ]
      ] ],
      [ "std::bitset", "classstd_1_1bitset.html", null ],
      [ "xpcc::BoundedDeque", "classxpcc_1_1_bounded_deque.html", null ],
      [ "xpcc::InterconnectInterface::Buffer", "structxpcc_1_1_interconnect_interface_1_1_buffer.html", null ],
      [ "xpcc::BufferedGraphicDisplay< 128, 64 >", "classxpcc_1_1_buffered_graphic_display.html", [
        [ "xpcc::Ks0108", "classxpcc_1_1_ks0108.html", null ]
      ] ],
      [ "xpcc::BufferedGraphicDisplay< 130, 128 >", "classxpcc_1_1_buffered_graphic_display.html", [
        [ "xpcc::Nokia6610", "classxpcc_1_1_nokia6610.html", null ]
      ] ],
      [ "xpcc::BufferedGraphicDisplay< Width, Height >", "classxpcc_1_1_buffered_graphic_display.html", [
        [ "xpcc::St7565", "classxpcc_1_1_st7565.html", null ]
      ] ],
      [ "xpcc::atmega::BufferedUart0", "classxpcc_1_1atmega_1_1_buffered_uart0.html", null ],
      [ "xpcc::atmega::BufferedUart1", "classxpcc_1_1atmega_1_1_buffered_uart1.html", null ],
      [ "xpcc::atmega::BufferedUart2", "classxpcc_1_1atmega_1_1_buffered_uart2.html", null ],
      [ "xpcc::atmega::BufferedUart3", "classxpcc_1_1atmega_1_1_buffered_uart3.html", null ],
      [ "xpcc::Button", "classxpcc_1_1_button.html", null ],
      [ "xpcc::ButtonGroup", "classxpcc_1_1_button_group.html", null ],
      [ "xpcc::sab::Callable", "structxpcc_1_1sab_1_1_callable.html", null ],
      [ "xpcc::amnb::Callable", "structxpcc_1_1amnb_1_1_callable.html", null ],
      [ "xpcc::stm32::Can1", "classxpcc_1_1stm32_1_1_can1.html", null ],
      [ "xpcc::CanConnectorBase", "classxpcc_1_1_can_connector_base.html", [
        [ "xpcc::CanConnector", "classxpcc_1_1_can_connector.html", null ]
      ] ],
      [ "xpcc::stm32::CanFilter", "classxpcc_1_1stm32_1_1_can_filter.html", null ],
      [ "xpcc::CanUsb", "classxpcc_1_1_can_usb.html", null ],
      [ "xpcc::CanUsbFormater", "classxpcc_1_1_can_usb_formater.html", null ],
      [ "xpcc::Circle2D", "classxpcc_1_1_circle2_d.html", null ],
      [ "xpcc::stm32::Clock", "classxpcc_1_1stm32_1_1_clock.html", null ],
      [ "xpcc::lpc::Clock", "classxpcc_1_1lpc_1_1_clock.html", null ],
      [ "xpcc::Clock", "classxpcc_1_1_clock.html", null ],
      [ "xpcc::Communicatable", "classxpcc_1_1_communicatable.html", [
        [ "xpcc::AbstractComponent", "classxpcc_1_1_abstract_component.html", null ],
        [ "xpcc::CommunicatableTask", "classxpcc_1_1_communicatable_task.html", null ],
        [ "xpcc::Communicator", "classxpcc_1_1_communicator.html", null ]
      ] ],
      [ "std::complex", "classstd_1_1complex.html", null ],
      [ "xpcc::BoundedDeque::const_iterator", "classxpcc_1_1_bounded_deque_1_1const__iterator.html", null ],
      [ "xpcc::DoublyLinkedList::const_iterator", "classxpcc_1_1_doubly_linked_list_1_1const__iterator.html", null ],
      [ "xpcc::DynamicArray::const_iterator", "classxpcc_1_1_dynamic_array_1_1const__iterator.html", null ],
      [ "xpcc::LinkedList::const_iterator", "classxpcc_1_1_linked_list_1_1const__iterator.html", null ],
      [ "std::basic_string::const_iterator", "classstd_1_1basic__string_1_1const__iterator.html", null ],
      [ "std::string::const_iterator", "classstd_1_1string_1_1const__iterator.html", null ],
      [ "std::wstring::const_iterator", "classstd_1_1wstring_1_1const__iterator.html", null ],
      [ "std::deque::const_iterator", "classstd_1_1deque_1_1const__iterator.html", null ],
      [ "std::list::const_iterator", "classstd_1_1list_1_1const__iterator.html", null ],
      [ "std::map::const_iterator", "classstd_1_1map_1_1const__iterator.html", null ],
      [ "std::multimap::const_iterator", "classstd_1_1multimap_1_1const__iterator.html", null ],
      [ "std::set::const_iterator", "classstd_1_1set_1_1const__iterator.html", null ],
      [ "std::multiset::const_iterator", "classstd_1_1multiset_1_1const__iterator.html", null ],
      [ "std::vector::const_iterator", "classstd_1_1vector_1_1const__iterator.html", null ],
      [ "std::basic_string::const_reverse_iterator", "classstd_1_1basic__string_1_1const__reverse__iterator.html", null ],
      [ "std::string::const_reverse_iterator", "classstd_1_1string_1_1const__reverse__iterator.html", null ],
      [ "std::wstring::const_reverse_iterator", "classstd_1_1wstring_1_1const__reverse__iterator.html", null ],
      [ "std::deque::const_reverse_iterator", "classstd_1_1deque_1_1const__reverse__iterator.html", null ],
      [ "std::list::const_reverse_iterator", "classstd_1_1list_1_1const__reverse__iterator.html", null ],
      [ "std::map::const_reverse_iterator", "classstd_1_1map_1_1const__reverse__iterator.html", null ],
      [ "std::multimap::const_reverse_iterator", "classstd_1_1multimap_1_1const__reverse__iterator.html", null ],
      [ "std::set::const_reverse_iterator", "classstd_1_1set_1_1const__reverse__iterator.html", null ],
      [ "std::multiset::const_reverse_iterator", "classstd_1_1multiset_1_1const__reverse__iterator.html", null ],
      [ "std::vector::const_reverse_iterator", "classstd_1_1vector_1_1const__reverse__iterator.html", null ],
      [ "xpcc::atomic::Container", "classxpcc_1_1atomic_1_1_container.html", null ],
      [ "unittest::Controller", "classunittest_1_1_controller.html", null ],
      [ "xpcc::tmp::Conversion", "classxpcc_1_1tmp_1_1_conversion.html", null ],
      [ "xpcc::tmp::Conversion< T, T >", "structxpcc_1_1tmp_1_1_conversion_3_01_t_00_01_t_01_4.html", null ],
      [ "xpcc::tmp::Conversion< T, void >", "structxpcc_1_1tmp_1_1_conversion_3_01_t_00_01void_01_4.html", null ],
      [ "xpcc::tmp::Conversion< void, T >", "structxpcc_1_1tmp_1_1_conversion_3_01void_00_01_t_01_4.html", null ],
      [ "xpcc::tmp::Conversion< void, void >", "structxpcc_1_1tmp_1_1_conversion_3_01void_00_01void_01_4.html", null ],
      [ "xpcc::stm32::Core", "classxpcc_1_1stm32_1_1_core.html", null ],
      [ "xpcc::at91::Core", "classxpcc_1_1at91_1_1_core.html", null ],
      [ "unittest::CountType", "classunittest_1_1_count_type.html", null ],
      [ "xpcc::CpuBoard2", "classxpcc_1_1_cpu_board2.html", null ],
      [ "xpcc::CpuBoard2Slave", "classxpcc_1_1_cpu_board2_slave.html", null ],
      [ "xpcc::filter::Debounce", "classxpcc_1_1filter_1_1_debounce.html", null ],
      [ "xpcc::at91::Debug", "classxpcc_1_1at91_1_1_debug.html", null ],
      [ "xpcc::log::DefaultStyle", "classxpcc_1_1log_1_1_default_style.html", null ],
      [ "xpcc::i2c::Delegate", "classxpcc_1_1i2c_1_1_delegate.html", [
        [ "xpcc::ADXL345", "classxpcc_1_1_a_d_x_l345.html", null ],
        [ "xpcc::BMA180", "classxpcc_1_1_b_m_a180.html", null ],
        [ "xpcc::BMP085", "classxpcc_1_1_b_m_p085.html", null ],
        [ "xpcc::HCLA12X5", "classxpcc_1_1_h_c_l_a12_x5.html", null ],
        [ "xpcc::HMC58", "classxpcc_1_1_h_m_c58.html", null ],
        [ "xpcc::ITG3200", "classxpcc_1_1_i_t_g3200.html", null ],
        [ "xpcc::TMP102", "classxpcc_1_1_t_m_p102.html", null ]
      ] ],
      [ "std::deque", "classstd_1_1deque.html", null ],
      [ "xpcc::fat::Directory", "classxpcc_1_1fat_1_1_directory.html", null ],
      [ "xpcc::Dispatcher", "classxpcc_1_1_dispatcher.html", null ],
      [ "xpcc::atxmega::DmaChannel0", "classxpcc_1_1atxmega_1_1_dma_channel0.html", null ],
      [ "xpcc::atxmega::DmaChannel1", "classxpcc_1_1atxmega_1_1_dma_channel1.html", null ],
      [ "xpcc::atxmega::DmaChannel2", "classxpcc_1_1atxmega_1_1_dma_channel2.html", null ],
      [ "xpcc::atxmega::DmaChannel3", "classxpcc_1_1atxmega_1_1_dma_channel3.html", null ],
      [ "xpcc::atxmega::DmaController", "classxpcc_1_1atxmega_1_1_dma_controller.html", null ],
      [ "xpcc::led::DoubleIndicator", "classxpcc_1_1led_1_1_double_indicator.html", null ],
      [ "xpcc::DoublyLinkedList", "classxpcc_1_1_doubly_linked_list.html", null ],
      [ "xpcc::Ds1631", "classxpcc_1_1_ds1631.html", null ],
      [ "xpcc::Ds18b20", "classxpcc_1_1_ds18b20.html", null ],
      [ "xpcc::DynamicArray", "classxpcc_1_1_dynamic_array.html", null ],
      [ "xpcc::DynamicArray< Vector< T, 2 > >", "classxpcc_1_1_dynamic_array.html", null ],
      [ "xpcc::Empty", "structxpcc_1_1_empty.html", null ],
      [ "xpcc::tmp::EnableIfCondition", "structxpcc_1_1tmp_1_1_enable_if_condition.html", null ],
      [ "xpcc::tmp::EnableIfCondition< Conditional::value, T >", "structxpcc_1_1tmp_1_1_enable_if_condition.html", [
        [ "xpcc::tmp::EnableIf", "structxpcc_1_1tmp_1_1_enable_if.html", null ]
      ] ],
      [ "xpcc::tmp::EnableIfCondition< false, T >", "structxpcc_1_1tmp_1_1_enable_if_condition_3_01false_00_01_t_01_4.html", null ],
      [ "xpcc::Dispatcher::Entry", "classxpcc_1_1_dispatcher_1_1_entry.html", [
        [ "xpcc::Dispatcher::CallbackEntry", "classxpcc_1_1_dispatcher_1_1_callback_entry.html", null ]
      ] ],
      [ "xpcc::amnb::ErrorHandler", "structxpcc_1_1amnb_1_1_error_handler.html", null ],
      [ "xpcc::ErrorReport", "classxpcc_1_1_error_report.html", null ],
      [ "std::exception", "classstd_1_1exception.html", [
        [ "std::bad_alloc", "classstd_1_1bad__alloc.html", null ],
        [ "std::bad_cast", "classstd_1_1bad__cast.html", null ],
        [ "std::bad_exception", "classstd_1_1bad__exception.html", null ],
        [ "std::bad_typeid", "classstd_1_1bad__typeid.html", null ],
        [ "std::ios_base::failure", "classstd_1_1ios__base_1_1failure.html", null ],
        [ "std::logic_error", "classstd_1_1logic__error.html", [
          [ "std::domain_error", "classstd_1_1domain__error.html", null ],
          [ "std::invalid_argument", "classstd_1_1invalid__argument.html", null ],
          [ "std::length_error", "classstd_1_1length__error.html", null ],
          [ "std::out_of_range", "classstd_1_1out__of__range.html", null ]
        ] ],
        [ "std::runtime_error", "classstd_1_1runtime__error.html", [
          [ "std::overflow_error", "classstd_1_1overflow__error.html", null ],
          [ "std::range_error", "classstd_1_1range__error.html", null ],
          [ "std::underflow_error", "classstd_1_1underflow__error.html", null ]
        ] ]
      ] ],
      [ "xpcc::stm32::CanFilter::ExtendedFilterMask", "structxpcc_1_1stm32_1_1_can_filter_1_1_extended_filter_mask.html", null ],
      [ "xpcc::stm32::CanFilter::ExtendedFilterMaskShort", "structxpcc_1_1stm32_1_1_can_filter_1_1_extended_filter_mask_short.html", null ],
      [ "xpcc::fat::File", "classxpcc_1_1fat_1_1_file.html", null ],
      [ "xpcc::fat::FileInfo", "classxpcc_1_1fat_1_1_file_info.html", null ],
      [ "xpcc::fat::FileSystem", "classxpcc_1_1fat_1_1_file_system.html", null ],
      [ "xpcc::atomic::Flag", "classxpcc_1_1atomic_1_1_flag.html", null ],
      [ "xpcc::can::Message::Flags", "structxpcc_1_1can_1_1_message_1_1_flags.html", null ],
      [ "xpcc::at91::Flash", "classxpcc_1_1at91_1_1_flash.html", null ],
      [ "xpcc::accessor::Flash", "classxpcc_1_1accessor_1_1_flash.html", null ],
      [ "xpcc::Ft245", "classxpcc_1_1_ft245.html", null ],
      [ "xpcc::GeometricTraits", "structxpcc_1_1_geometric_traits.html", null ],
      [ "xpcc::GeometricTraits< double >", "structxpcc_1_1_geometric_traits_3_01double_01_4.html", null ],
      [ "xpcc::GeometricTraits< float >", "structxpcc_1_1_geometric_traits_3_01float_01_4.html", null ],
      [ "xpcc::GeometricTraits< int16_t >", "structxpcc_1_1_geometric_traits_3_01int16__t_01_4.html", null ],
      [ "xpcc::GeometricTraits< int32_t >", "structxpcc_1_1_geometric_traits_3_01int32__t_01_4.html", null ],
      [ "xpcc::GeometricTraits< int8_t >", "structxpcc_1_1_geometric_traits_3_01int8__t_01_4.html", null ],
      [ "xpcc::GeometricTraits< uint8_t >", "structxpcc_1_1_geometric_traits_3_01uint8__t_01_4.html", null ],
      [ "xpcc::stm32::GpioMode", "structxpcc_1_1stm32_1_1_gpio_mode.html", null ],
      [ "xpcc::stm32::GpioMode< P, N, false >", "structxpcc_1_1stm32_1_1_gpio_mode_3_01_p_00_01_n_00_01false_01_4.html", null ],
      [ "xpcc::tipc::Header", "structxpcc_1_1tipc_1_1_header.html", null ],
      [ "xpcc::Header", "structxpcc_1_1_header.html", null ],
      [ "xpcc::atxmega::HighResC", "classxpcc_1_1atxmega_1_1_high_res_c.html", [
        [ "xpcc::atxmega::TimerC0", "classxpcc_1_1atxmega_1_1_timer_c0.html", [
          [ "xpcc::atxmega::TimerInterruptC0", "classxpcc_1_1atxmega_1_1_timer_interrupt_c0.html", null ]
        ] ],
        [ "xpcc::atxmega::TimerC1", "classxpcc_1_1atxmega_1_1_timer_c1.html", [
          [ "xpcc::atxmega::TimerInterruptC1", "classxpcc_1_1atxmega_1_1_timer_interrupt_c1.html", null ]
        ] ]
      ] ],
      [ "xpcc::atxmega::HighResD", "classxpcc_1_1atxmega_1_1_high_res_d.html", [
        [ "xpcc::atxmega::TimerD0", "classxpcc_1_1atxmega_1_1_timer_d0.html", [
          [ "xpcc::atxmega::TimerInterruptD0", "classxpcc_1_1atxmega_1_1_timer_interrupt_d0.html", null ]
        ] ],
        [ "xpcc::atxmega::TimerD1", "classxpcc_1_1atxmega_1_1_timer_d1.html", [
          [ "xpcc::atxmega::TimerInterruptD1", "classxpcc_1_1atxmega_1_1_timer_interrupt_d1.html", null ]
        ] ]
      ] ],
      [ "xpcc::atxmega::HighResE", "classxpcc_1_1atxmega_1_1_high_res_e.html", [
        [ "xpcc::atxmega::TimerE0", "classxpcc_1_1atxmega_1_1_timer_e0.html", [
          [ "xpcc::atxmega::TimerInterruptE0", "classxpcc_1_1atxmega_1_1_timer_interrupt_e0.html", null ]
        ] ],
        [ "xpcc::atxmega::TimerE1", "classxpcc_1_1atxmega_1_1_timer_e1.html", [
          [ "xpcc::atxmega::TimerInterruptE1", "classxpcc_1_1atxmega_1_1_timer_interrupt_e1.html", null ]
        ] ]
      ] ],
      [ "xpcc::atxmega::HighResF", "classxpcc_1_1atxmega_1_1_high_res_f.html", [
        [ "xpcc::atxmega::TimerF0", "classxpcc_1_1atxmega_1_1_timer_f0.html", [
          [ "xpcc::atxmega::TimerInterruptF0", "classxpcc_1_1atxmega_1_1_timer_interrupt_f0.html", null ]
        ] ],
        [ "xpcc::atxmega::TimerF1", "classxpcc_1_1atxmega_1_1_timer_f1.html", [
          [ "xpcc::atxmega::TimerInterruptF1", "classxpcc_1_1atxmega_1_1_timer_interrupt_f1.html", null ]
        ] ]
      ] ],
      [ "xpcc::HMC58< TwiMaster >", "classxpcc_1_1_h_m_c58.html", [
        [ "xpcc::HMC5843", "classxpcc_1_1_h_m_c5843.html", null ],
        [ "xpcc::HMC5883l", "classxpcc_1_1_h_m_c5883l.html", null ]
      ] ],
      [ "xpcc::I2cEeprom", "classxpcc_1_1_i2c_eeprom.html", null ],
      [ "xpcc::stm32::CanFilter::Identifier", "structxpcc_1_1stm32_1_1_can_filter_1_1_identifier.html", [
        [ "xpcc::stm32::CanFilter::ExtendedIdentifier", "structxpcc_1_1stm32_1_1_can_filter_1_1_extended_identifier.html", null ],
        [ "xpcc::stm32::CanFilter::StandardIdentifier", "structxpcc_1_1stm32_1_1_can_filter_1_1_standard_identifier.html", null ]
      ] ],
      [ "xpcc::stm32::CanFilter::IdentifierShort", "structxpcc_1_1stm32_1_1_can_filter_1_1_identifier_short.html", [
        [ "xpcc::stm32::CanFilter::ExtendedIdentifierShort", "structxpcc_1_1stm32_1_1_can_filter_1_1_extended_identifier_short.html", null ],
        [ "xpcc::stm32::CanFilter::StandardIdentifierShort", "structxpcc_1_1stm32_1_1_can_filter_1_1_standard_identifier_short.html", null ]
      ] ],
      [ "xpcc::led::Indicator", "classxpcc_1_1led_1_1_indicator.html", null ],
      [ "xpcc::atxmega::InputCapture", "classxpcc_1_1atxmega_1_1_input_capture.html", null ],
      [ "xpcc::InterconnectInterface", "classxpcc_1_1_interconnect_interface.html", null ],
      [ "xpcc::InterconnectLink", "structxpcc_1_1_interconnect_link.html", null ],
      [ "xpcc::sab2::Interface", "classxpcc_1_1sab2_1_1_interface.html", null ],
      [ "xpcc::Interface", "classxpcc_1_1_interface.html", [
        [ "xpcc::i2c::Master", "classxpcc_1_1i2c_1_1_master.html", [
          [ "xpcc::atmega::AsynchronousI2cMaster", "classxpcc_1_1atmega_1_1_asynchronous_i2c_master.html", null ],
          [ "xpcc::atxmega::AsynchronousI2cMasterC", "classxpcc_1_1atxmega_1_1_asynchronous_i2c_master_c.html", null ],
          [ "xpcc::atxmega::AsynchronousI2cMasterD", "classxpcc_1_1atxmega_1_1_asynchronous_i2c_master_d.html", null ],
          [ "xpcc::atxmega::AsynchronousI2cMasterE", "classxpcc_1_1atxmega_1_1_asynchronous_i2c_master_e.html", null ],
          [ "xpcc::atxmega::AsynchronousI2cMasterF", "classxpcc_1_1atxmega_1_1_asynchronous_i2c_master_f.html", null ],
          [ "xpcc::SoftwareI2C", "classxpcc_1_1_software_i2_c.html", null ],
          [ "xpcc::stm32::I2c1", "classxpcc_1_1stm32_1_1_i2c1.html", null ],
          [ "xpcc::stm32::I2c2", "classxpcc_1_1stm32_1_1_i2c2.html", null ],
          [ "xpcc::stm32::I2c3", "classxpcc_1_1stm32_1_1_i2c3.html", null ]
        ] ],
        [ "xpcc::SpiMaster", "classxpcc_1_1_spi_master.html", [
          [ "xpcc::atmega::SpiMaster", "classxpcc_1_1atmega_1_1_spi_master.html", null ],
          [ "xpcc::atxmega::SpiMasterC", "classxpcc_1_1atxmega_1_1_spi_master_c.html", null ],
          [ "xpcc::atxmega::SpiMasterD", "classxpcc_1_1atxmega_1_1_spi_master_d.html", null ],
          [ "xpcc::atxmega::SpiMasterE", "classxpcc_1_1atxmega_1_1_spi_master_e.html", null ],
          [ "xpcc::atxmega::SpiMasterF", "classxpcc_1_1atxmega_1_1_spi_master_f.html", null ],
          [ "xpcc::atxmega::UartSpiMasterC0", "classxpcc_1_1atxmega_1_1_uart_spi_master_c0.html", null ],
          [ "xpcc::atxmega::UartSpiMasterC1", "classxpcc_1_1atxmega_1_1_uart_spi_master_c1.html", null ],
          [ "xpcc::atxmega::UartSpiMasterD0", "classxpcc_1_1atxmega_1_1_uart_spi_master_d0.html", null ],
          [ "xpcc::atxmega::UartSpiMasterD1", "classxpcc_1_1atxmega_1_1_uart_spi_master_d1.html", null ],
          [ "xpcc::atxmega::UartSpiMasterE0", "classxpcc_1_1atxmega_1_1_uart_spi_master_e0.html", null ],
          [ "xpcc::atxmega::UartSpiMasterE1", "classxpcc_1_1atxmega_1_1_uart_spi_master_e1.html", null ],
          [ "xpcc::atxmega::UartSpiMasterF0", "classxpcc_1_1atxmega_1_1_uart_spi_master_f0.html", null ],
          [ "xpcc::atxmega::UartSpiMasterF1", "classxpcc_1_1atxmega_1_1_uart_spi_master_f1.html", null ],
          [ "xpcc::SoftwareSpi", "classxpcc_1_1_software_spi.html", null ]
        ] ]
      ] ],
      [ "xpcc::amnb::Interface", "classxpcc_1_1amnb_1_1_interface.html", null ],
      [ "xpcc::sab::Interface", "classxpcc_1_1sab_1_1_interface.html", null ],
      [ "xpcc::gpio::Invert", "classxpcc_1_1gpio_1_1_invert.html", null ],
      [ "xpcc::IODevice", "classxpcc_1_1_i_o_device.html", [
        [ "xpcc::CharacterDisplay::Writer", "classxpcc_1_1_character_display_1_1_writer.html", null ],
        [ "xpcc::GraphicDisplay::Writer", "classxpcc_1_1_graphic_display_1_1_writer.html", null ],
        [ "xpcc::IODeviceWrapper", "classxpcc_1_1_i_o_device_wrapper.html", null ],
        [ "xpcc::log::StyleWrapper", "classxpcc_1_1log_1_1_style_wrapper.html", null ],
        [ "xpcc::pc::SerialInterface", "classxpcc_1_1pc_1_1_serial_interface.html", null ],
        [ "xpcc::pc::SerialPort", "classxpcc_1_1pc_1_1_serial_port.html", null ],
        [ "xpcc::pc::Terminal", "classxpcc_1_1pc_1_1_terminal.html", null ]
      ] ],
      [ "std::ios_base", "classstd_1_1ios__base.html", [
        [ "std::basic_ios", "classstd_1_1basic__ios.html", null ]
      ] ],
      [ "xpcc::IOStream", "classxpcc_1_1_i_o_stream.html", [
        [ "xpcc::CharacterDisplay", "classxpcc_1_1_character_display.html", [
          [ "xpcc::Hd44780", "classxpcc_1_1_hd44780.html", null ],
          [ "xpcc::St7036", "classxpcc_1_1_st7036.html", null ]
        ] ],
        [ "xpcc::GraphicDisplay", "classxpcc_1_1_graphic_display.html", [
          [ "xpcc::BufferedGraphicDisplay", "classxpcc_1_1_buffered_graphic_display.html", null ]
        ] ],
        [ "xpcc::log::Logger", "classxpcc_1_1log_1_1_logger.html", null ]
      ] ],
      [ "std::map::iterator", "classstd_1_1map_1_1iterator.html", null ],
      [ "std::multiset::iterator", "classstd_1_1multiset_1_1iterator.html", null ],
      [ "std::wstring::iterator", "classstd_1_1wstring_1_1iterator.html", null ],
      [ "std::list::iterator", "classstd_1_1list_1_1iterator.html", null ],
      [ "xpcc::DoublyLinkedList::iterator", "classxpcc_1_1_doubly_linked_list_1_1iterator.html", null ],
      [ "xpcc::DynamicArray::iterator", "classxpcc_1_1_dynamic_array_1_1iterator.html", null ],
      [ "xpcc::LinkedList::iterator", "classxpcc_1_1_linked_list_1_1iterator.html", null ],
      [ "std::basic_string::iterator", "classstd_1_1basic__string_1_1iterator.html", null ],
      [ "std::string::iterator", "classstd_1_1string_1_1iterator.html", null ],
      [ "std::deque::iterator", "classstd_1_1deque_1_1iterator.html", null ],
      [ "std::multimap::iterator", "classstd_1_1multimap_1_1iterator.html", null ],
      [ "std::set::iterator", "classstd_1_1set_1_1iterator.html", null ],
      [ "std::vector::iterator", "classstd_1_1vector_1_1iterator.html", null ],
      [ "xpcc::interpolation::Lagrange", "classxpcc_1_1interpolation_1_1_lagrange.html", null ],
      [ "xpcc::led::Led", "classxpcc_1_1led_1_1_led.html", [
        [ "xpcc::led::TLC594XLed", "classxpcc_1_1led_1_1_t_l_c594_x_led.html", null ]
      ] ],
      [ "xpcc::Line2D", "classxpcc_1_1_line2_d.html", null ],
      [ "xpcc::interpolation::Linear", "classxpcc_1_1interpolation_1_1_linear.html", null ],
      [ "xpcc::LineSegment2D", "classxpcc_1_1_line_segment2_d.html", null ],
      [ "xpcc::LinkedList", "classxpcc_1_1_linked_list.html", null ],
      [ "std::list", "classstd_1_1list.html", null ],
      [ "xpcc::amnb::Listener", "structxpcc_1_1amnb_1_1_listener.html", null ],
      [ "xpcc::LM75", "classxpcc_1_1_l_m75.html", null ],
      [ "xpcc::Location2D", "classxpcc_1_1_location2_d.html", null ],
      [ "xpcc::atomic::Lock", "classxpcc_1_1atomic_1_1_lock.html", null ],
      [ "xpcc::rtos::Thread::Lock", "classxpcc_1_1rtos_1_1_thread_1_1_lock.html", null ],
      [ "xpcc::LUDecomposition", "classxpcc_1_1_l_u_decomposition.html", null ],
      [ "xpcc::LUDecomposition::LUSubDecomposition", "classxpcc_1_1_l_u_decomposition_1_1_l_u_sub_decomposition.html", null ],
      [ "xpcc::LUDecomposition::LUSubDecomposition< T, OFFSET, WIDTH, OFFSET >", "classxpcc_1_1_l_u_decomposition_1_1_l_u_sub_decomposition_3_01_t_00_01_o_f_f_s_e_t_00_01_w_i_d_t_h_00_01_o_f_f_s_e_t_01_4.html", null ],
      [ "std::map", "classstd_1_1map.html", null ],
      [ "xpcc::sab::Master", "classxpcc_1_1sab_1_1_master.html", null ],
      [ "xpcc::Matrix", "classxpcc_1_1_matrix.html", null ],
      [ "xpcc::Mcp23s08", "classxpcc_1_1_mcp23s08.html", null ],
      [ "xpcc::Mcp23s17", "classxpcc_1_1_mcp23s17.html", null ],
      [ "xpcc::Mcp2515", "classxpcc_1_1_mcp2515.html", null ],
      [ "xpcc::Mcp4922", "classxpcc_1_1_mcp4922.html", null ],
      [ "xpcc::filter::Median", "classxpcc_1_1filter_1_1_median.html", null ],
      [ "xpcc::can::Message", "structxpcc_1_1can_1_1_message.html", null ],
      [ "xpcc::MovingAverage", "classxpcc_1_1_moving_average.html", null ],
      [ "std::multimap", "classstd_1_1multimap.html", null ],
      [ "std::multiset", "classstd_1_1multiset.html", null ],
      [ "xpcc::rtos::Mutex", "classxpcc_1_1rtos_1_1_mutex.html", null ],
      [ "xpcc::rtos::MutexGuard", "classxpcc_1_1rtos_1_1_mutex_guard.html", null ],
      [ "my_namespace::MyClass", "classmy__namespace_1_1_my_class.html", null ],
      [ "xpcc::gpio::Nibble", "classxpcc_1_1gpio_1_1_nibble.html", null ],
      [ "xpcc::DoublyLinkedList::Node", "structxpcc_1_1_doubly_linked_list_1_1_node.html", null ],
      [ "xpcc::LinkedList::Node", "structxpcc_1_1_linked_list_1_1_node.html", null ],
      [ "xpcc::tmp::NullType", "classxpcc_1_1tmp_1_1_null_type.html", null ],
      [ "xpcc::Pair", "classxpcc_1_1_pair.html", null ],
      [ "xpcc::Pid::Parameter", "structxpcc_1_1_pid_1_1_parameter.html", null ],
      [ "xpcc::SCurveController::Parameter", "structxpcc_1_1_s_curve_controller_1_1_parameter.html", null ],
      [ "xpcc::PeriodicTimer", "classxpcc_1_1_periodic_timer.html", null ],
      [ "xpcc::fat::PhysicalVolume", "classxpcc_1_1fat_1_1_physical_volume.html", null ],
      [ "xpcc::Pid", "classxpcc_1_1_pid.html", null ],
      [ "xpcc::at91::Pio", "classxpcc_1_1at91_1_1_pio.html", null ],
      [ "xpcc::at91::Pit", "classxpcc_1_1at91_1_1_pit.html", null ],
      [ "xpcc::PointSet2D", "classxpcc_1_1_point_set2_d.html", null ],
      [ "xpcc::PointSet2D< T >", "classxpcc_1_1_point_set2_d.html", [
        [ "xpcc::Polygon2D", "classxpcc_1_1_polygon2_d.html", null ]
      ] ],
      [ "xpcc::gpio::Port", "classxpcc_1_1gpio_1_1_port.html", null ],
      [ "xpcc::Postman", "classxpcc_1_1_postman.html", [
        [ "xpcc::DynamicPostman", "classxpcc_1_1_dynamic_postman.html", null ]
      ] ],
      [ "xpcc::Pow", "classxpcc_1_1_pow.html", null ],
      [ "xpcc::Pow< B, 0 >", "classxpcc_1_1_pow_3_01_b_00_010_01_4.html", null ],
      [ "std::priority_queue", "classstd_1_1priority__queue.html", null ],
      [ "xpcc::pt::Protothread", "classxpcc_1_1pt_1_1_protothread.html", null ],
      [ "xpcc::led::Pulse", "classxpcc_1_1led_1_1_pulse.html", null ],
      [ "xpcc::at91::Pwm", "classxpcc_1_1at91_1_1_pwm.html", null ],
      [ "xpcc::at91::PwmChannel", "classxpcc_1_1at91_1_1_pwm_channel.html", null ],
      [ "xpcc::Quaternion", "classxpcc_1_1_quaternion.html", null ],
      [ "xpcc::Queue", "classxpcc_1_1_queue.html", null ],
      [ "xpcc::atomic::Queue", "classxpcc_1_1atomic_1_1_queue.html", null ],
      [ "std::queue", "classstd_1_1queue.html", null ],
      [ "xpcc::Queue< T, Container >", "classxpcc_1_1_queue.html", [
        [ "xpcc::BoundedQueue", "classxpcc_1_1_bounded_queue.html", null ]
      ] ],
      [ "xpcc::rtos::QueueBase", "classxpcc_1_1rtos_1_1_queue_base.html", [
        [ "xpcc::rtos::Queue< T >", "classxpcc_1_1rtos_1_1_queue.html", null ]
      ] ],
      [ "xpcc::accessor::Ram", "classxpcc_1_1accessor_1_1_ram.html", null ],
      [ "xpcc::filter::Ramp", "classxpcc_1_1filter_1_1_ramp.html", null ],
      [ "xpcc::allocator::Block::rebind", "structxpcc_1_1allocator_1_1_block_1_1rebind.html", null ],
      [ "xpcc::allocator::Dynamic::rebind", "structxpcc_1_1allocator_1_1_dynamic_1_1rebind.html", null ],
      [ "xpcc::allocator::Static::rebind", "structxpcc_1_1allocator_1_1_static_1_1rebind.html", null ],
      [ "xpcc::CanConnector::ReceiveListItem", "classxpcc_1_1_can_connector_1_1_receive_list_item.html", null ],
      [ "xpcc::tipc::Receiver", "classxpcc_1_1tipc_1_1_receiver.html", null ],
      [ "xpcc::tipc::ReceiverSocket", "classxpcc_1_1tipc_1_1_receiver_socket.html", null ],
      [ "unittest::Reporter", "classunittest_1_1_reporter.html", null ],
      [ "xpcc::amnb::Response", "classxpcc_1_1amnb_1_1_response.html", null ],
      [ "xpcc::sab::Response", "classxpcc_1_1sab_1_1_response.html", null ],
      [ "xpcc::ResponseCallback", "classxpcc_1_1_response_callback.html", null ],
      [ "xpcc::ResponseHandle", "classxpcc_1_1_response_handle.html", null ],
      [ "std::multimap::reverse_iterator", "classstd_1_1multimap_1_1reverse__iterator.html", null ],
      [ "std::string::reverse_iterator", "classstd_1_1string_1_1reverse__iterator.html", null ],
      [ "std::vector::reverse_iterator", "classstd_1_1vector_1_1reverse__iterator.html", null ],
      [ "std::set::reverse_iterator", "classstd_1_1set_1_1reverse__iterator.html", null ],
      [ "std::deque::reverse_iterator", "classstd_1_1deque_1_1reverse__iterator.html", null ],
      [ "std::list::reverse_iterator", "classstd_1_1list_1_1reverse__iterator.html", null ],
      [ "std::basic_string::reverse_iterator", "classstd_1_1basic__string_1_1reverse__iterator.html", null ],
      [ "std::wstring::reverse_iterator", "classstd_1_1wstring_1_1reverse__iterator.html", null ],
      [ "std::map::reverse_iterator", "classstd_1_1map_1_1reverse__iterator.html", null ],
      [ "std::multiset::reverse_iterator", "classstd_1_1multiset_1_1reverse__iterator.html", null ],
      [ "xpcc::led::Rgb", "classxpcc_1_1led_1_1_rgb.html", null ],
      [ "xpcc::LUDecomposition::RowOperation", "classxpcc_1_1_l_u_decomposition_1_1_row_operation.html", null ],
      [ "xpcc::LUDecomposition::RowOperation< T, 0 >", "classxpcc_1_1_l_u_decomposition_1_1_row_operation_3_01_t_00_010_01_4.html", null ],
      [ "xpcc::tmp::SameType", "structxpcc_1_1tmp_1_1_same_type.html", null ],
      [ "xpcc::tmp::SameType< T, T >", "structxpcc_1_1tmp_1_1_same_type_3_01_t_00_01_t_01_4.html", null ],
      [ "xpcc::Saturated", "classxpcc_1_1_saturated.html", null ],
      [ "xpcc::rtos::Scheduler", "classxpcc_1_1rtos_1_1_scheduler.html", null ],
      [ "xpcc::Scheduler", "classxpcc_1_1_scheduler.html", null ],
      [ "xpcc::SCP1000", "classxpcc_1_1_s_c_p1000.html", null ],
      [ "xpcc::SCurveController", "classxpcc_1_1_s_curve_controller.html", null ],
      [ "xpcc::SCurveGenerator", "classxpcc_1_1_s_curve_generator.html", null ],
      [ "xpcc::tmp::Select", "structxpcc_1_1tmp_1_1_select.html", null ],
      [ "xpcc::tmp::Select< false, T, U >", "structxpcc_1_1tmp_1_1_select_3_01false_00_01_t_00_01_u_01_4.html", null ],
      [ "xpcc::pt::Semaphore", "classxpcc_1_1pt_1_1_semaphore.html", null ],
      [ "xpcc::rtos::SemaphoreBase", "classxpcc_1_1rtos_1_1_semaphore_base.html", [
        [ "xpcc::rtos::BinarySemaphore", "classxpcc_1_1rtos_1_1_binary_semaphore.html", null ],
        [ "xpcc::rtos::Semaphore", "classxpcc_1_1rtos_1_1_semaphore.html", [
          [ "xpcc::rtos::BinarySemaphore", "classxpcc_1_1rtos_1_1_binary_semaphore.html", null ]
        ] ]
      ] ],
      [ "xpcc::CanConnector::SendListItem", "classxpcc_1_1_can_connector_1_1_send_list_item.html", null ],
      [ "std::set", "classstd_1_1set.html", null ],
      [ "xpcc::ShiftRegisterInput", "classxpcc_1_1_shift_register_input.html", null ],
      [ "xpcc::ShiftRegisterOutput", "classxpcc_1_1_shift_register_output.html", null ],
      [ "xpcc::SmartPointer", "classxpcc_1_1_smart_pointer.html", null ],
      [ "xpcc::SoftwareOneWire", "classxpcc_1_1_software_one_wire.html", null ],
      [ "xpcc::at91::Spi0", "classxpcc_1_1at91_1_1_spi0.html", null ],
      [ "xpcc::stm32::Spi1", "classxpcc_1_1stm32_1_1_spi1.html", null ],
      [ "xpcc::at91::Spi1", "classxpcc_1_1at91_1_1_spi1.html", null ],
      [ "xpcc::stm32::Spi2", "classxpcc_1_1stm32_1_1_spi2.html", null ],
      [ "xpcc::stm32::Spi3", "classxpcc_1_1stm32_1_1_spi3.html", null ],
      [ "xpcc::SpiRam", "classxpcc_1_1_spi_ram.html", null ],
      [ "xpcc::at91::Ssc", "classxpcc_1_1at91_1_1_ssc.html", null ],
      [ "xpcc::St7036< SPI, CS, RS, 16, 2 >", "classxpcc_1_1_st7036.html", [
        [ "xpcc::DogM162", "classxpcc_1_1_dog_m162.html", null ]
      ] ],
      [ "xpcc::St7036< SPI, CS, RS, 16, 3 >", "classxpcc_1_1_st7036.html", [
        [ "xpcc::DogM163", "classxpcc_1_1_dog_m163.html", null ]
      ] ],
      [ "xpcc::St7036< SPI, CS, RS, 8, 1 >", "classxpcc_1_1_st7036.html", [
        [ "xpcc::DogM081", "classxpcc_1_1_dog_m081.html", null ]
      ] ],
      [ "xpcc::St7565< SPI, CS, A0, Reset, 102, 64, TopView >", "classxpcc_1_1_st7565.html", [
        [ "xpcc::DogS102", "classxpcc_1_1_dog_s102.html", null ]
      ] ],
      [ "xpcc::St7565< SPI, CS, A0, Reset, 128, 64, TopView >", "classxpcc_1_1_st7565.html", [
        [ "xpcc::DogL128", "classxpcc_1_1_dog_l128.html", null ],
        [ "xpcc::DogM128", "classxpcc_1_1_dog_m128.html", null ]
      ] ],
      [ "xpcc::St7565< SPI, CS, A0, Reset, 132, 32, TopView >", "classxpcc_1_1_st7565.html", [
        [ "xpcc::DogM132", "classxpcc_1_1_dog_m132.html", null ]
      ] ],
      [ "std::stack", "classstd_1_1stack.html", null ],
      [ "xpcc::Stack", "classxpcc_1_1_stack.html", null ],
      [ "xpcc::Stack< T, Container >", "classxpcc_1_1_stack.html", [
        [ "xpcc::BoundedStack", "classxpcc_1_1_bounded_stack.html", null ]
      ] ],
      [ "xpcc::stm32::CanFilter::StandardFilterMask", "structxpcc_1_1stm32_1_1_can_filter_1_1_standard_filter_mask.html", null ],
      [ "xpcc::stm32::CanFilter::StandardFilterMaskShort", "structxpcc_1_1stm32_1_1_can_filter_1_1_standard_filter_mask_short.html", null ],
      [ "xpcc::tmp::static_assert_test", "structxpcc_1_1tmp_1_1static__assert__test.html", null ],
      [ "xpcc::tmp::STATIC_ASSERTION_FAILURE< true >", "structxpcc_1_1tmp_1_1_s_t_a_t_i_c___a_s_s_e_r_t_i_o_n___f_a_i_l_u_r_e_3_01true_01_4.html", null ],
      [ "xpcc::log::Style", "classxpcc_1_1log_1_1_style.html", null ],
      [ "xpcc::log::Style< STYLE >", "classxpcc_1_1log_1_1_style.html", [
        [ "xpcc::log::Prefix", "classxpcc_1_1log_1_1_prefix.html", null ],
        [ "xpcc::log::StdColour", "classxpcc_1_1log_1_1_std_colour.html", null ]
      ] ],
      [ "xpcc::tmp::SuperSubclass", "structxpcc_1_1tmp_1_1_super_subclass.html", null ],
      [ "xpcc::tmp::SuperSubclass< T, void >", "structxpcc_1_1tmp_1_1_super_subclass_3_01_t_00_01void_01_4.html", null ],
      [ "xpcc::tmp::SuperSubclass< void, U >", "structxpcc_1_1tmp_1_1_super_subclass_3_01void_00_01_u_01_4.html", null ],
      [ "xpcc::tmp::SuperSubclass< void, void >", "structxpcc_1_1tmp_1_1_super_subclass_3_01void_00_01void_01_4.html", null ],
      [ "xpcc::tmp::SuperSubclassStrict", "structxpcc_1_1tmp_1_1_super_subclass_strict.html", null ],
      [ "xpcc::i2c::SyncMaster", "classxpcc_1_1i2c_1_1_sync_master.html", null ],
      [ "xpcc::stm32::SysTickTimer", "classxpcc_1_1stm32_1_1_sys_tick_timer.html", null ],
      [ "xpcc::Task", "classxpcc_1_1_task.html", [
        [ "xpcc::CommunicatableTask", "classxpcc_1_1_communicatable_task.html", null ]
      ] ],
      [ "xpcc::Scheduler::Task", "classxpcc_1_1_scheduler_1_1_task.html", null ],
      [ "xpcc::Scheduler::TaskListItem", "structxpcc_1_1_scheduler_1_1_task_list_item.html", null ],
      [ "unittest::TestSuite", "classunittest_1_1_test_suite.html", null ],
      [ "xpcc::rtos::Thread", "classxpcc_1_1rtos_1_1_thread.html", null ],
      [ "xpcc::Timeout", "classxpcc_1_1_timeout.html", null ],
      [ "xpcc::at91::Timer", "classxpcc_1_1at91_1_1_timer.html", [
        [ "xpcc::at91::Timer0", "classxpcc_1_1at91_1_1_timer0.html", null ],
        [ "xpcc::at91::Timer1", "classxpcc_1_1at91_1_1_timer1.html", null ],
        [ "xpcc::at91::Timer2", "classxpcc_1_1at91_1_1_timer2.html", null ]
      ] ],
      [ "xpcc::Timestamp", "classxpcc_1_1_timestamp.html", null ],
      [ "xpcc::TLC594X", "classxpcc_1_1_t_l_c594_x.html", null ],
      [ "xpcc::tipc::Transmitter", "classxpcc_1_1tipc_1_1_transmitter.html", null ],
      [ "xpcc::sab::Transmitter", "classxpcc_1_1sab_1_1_transmitter.html", [
        [ "xpcc::sab::Slave", "classxpcc_1_1sab_1_1_slave.html", null ]
      ] ],
      [ "xpcc::amnb::Transmitter", "classxpcc_1_1amnb_1_1_transmitter.html", [
        [ "xpcc::amnb::Node", "classxpcc_1_1amnb_1_1_node.html", null ]
      ] ],
      [ "xpcc::tipc::TransmitterSocket", "classxpcc_1_1tipc_1_1_transmitter_socket.html", null ],
      [ "xpcc::at91::Twi", "classxpcc_1_1at91_1_1_twi.html", null ],
      [ "xpcc::at91::Uart0", "classxpcc_1_1at91_1_1_uart0.html", null ],
      [ "xpcc::Uart0", "classxpcc_1_1_uart0.html", null ],
      [ "xpcc::Uart1", "classxpcc_1_1_uart1.html", null ],
      [ "xpcc::at91::Uart1", "classxpcc_1_1at91_1_1_uart1.html", null ],
      [ "xpcc::Uart2", "classxpcc_1_1_uart2.html", null ],
      [ "xpcc::Uart3", "classxpcc_1_1_uart3.html", null ],
      [ "xpcc::stm32::UartBase", "classxpcc_1_1stm32_1_1_uart_base.html", [
        [ "xpcc::stm32::BufferedUart4", "classxpcc_1_1stm32_1_1_buffered_uart4.html", null ],
        [ "xpcc::stm32::BufferedUart5", "classxpcc_1_1stm32_1_1_buffered_uart5.html", null ],
        [ "xpcc::stm32::BufferedUsart1", "classxpcc_1_1stm32_1_1_buffered_usart1.html", null ],
        [ "xpcc::stm32::BufferedUsart2", "classxpcc_1_1stm32_1_1_buffered_usart2.html", null ],
        [ "xpcc::stm32::BufferedUsart3", "classxpcc_1_1stm32_1_1_buffered_usart3.html", null ],
        [ "xpcc::stm32::BufferedUsart6", "classxpcc_1_1stm32_1_1_buffered_usart6.html", null ],
        [ "xpcc::stm32::Uart4", "classxpcc_1_1stm32_1_1_uart4.html", null ],
        [ "xpcc::stm32::Uart5", "classxpcc_1_1stm32_1_1_uart5.html", null ],
        [ "xpcc::stm32::Usart1", "classxpcc_1_1stm32_1_1_usart1.html", null ],
        [ "xpcc::stm32::Usart2", "classxpcc_1_1stm32_1_1_usart2.html", null ],
        [ "xpcc::stm32::Usart3", "classxpcc_1_1stm32_1_1_usart3.html", null ],
        [ "xpcc::stm32::Usart6", "classxpcc_1_1stm32_1_1_usart6.html", null ]
      ] ],
      [ "xpcc::atxmega::UartBase", "classxpcc_1_1atxmega_1_1_uart_base.html", null ],
      [ "UartInterface", "class_uart_interface.html", null ],
      [ "xpcc::atomic::Unlock", "classxpcc_1_1atomic_1_1_unlock.html", null ],
      [ "xpcc::gpio::Unused", "classxpcc_1_1gpio_1_1_unused.html", null ],
      [ "xpcc::at91::Usb", "classxpcc_1_1at91_1_1_usb.html", null ],
      [ "std::valarray", "classstd_1_1valarray.html", null ],
      [ "xpcc::Vector", "classxpcc_1_1_vector.html", null ],
      [ "std::vector", "classstd_1_1vector.html", null ],
      [ "xpcc::Vector< T, 1 >", "classxpcc_1_1_vector_3_01_t_00_011_01_4.html", null ],
      [ "xpcc::Vector< T, 2 >", "classxpcc_1_1_vector_3_01_t_00_012_01_4.html", null ],
      [ "xpcc::Vector< T, 3 >", "classxpcc_1_1_vector_3_01_t_00_013_01_4.html", null ],
      [ "xpcc::Vector< T, 4 >", "classxpcc_1_1_vector_3_01_t_00_014_01_4.html", null ],
      [ "xpcc::lpc::Vic", "classxpcc_1_1lpc_1_1_vic.html", null ],
      [ "xpcc::at91::Watchdog", "classxpcc_1_1at91_1_1_watchdog.html", null ],
      [ "xpcc::atxmega::WaveformC", "classxpcc_1_1atxmega_1_1_waveform_c.html", [
        [ "xpcc::atxmega::TimerC0", "classxpcc_1_1atxmega_1_1_timer_c0.html", null ],
        [ "xpcc::atxmega::TimerC1", "classxpcc_1_1atxmega_1_1_timer_c1.html", null ]
      ] ],
      [ "xpcc::atxmega::WaveformD", "classxpcc_1_1atxmega_1_1_waveform_d.html", [
        [ "xpcc::atxmega::TimerD0", "classxpcc_1_1atxmega_1_1_timer_d0.html", null ],
        [ "xpcc::atxmega::TimerD1", "classxpcc_1_1atxmega_1_1_timer_d1.html", null ]
      ] ],
      [ "xpcc::atxmega::WaveformE", "classxpcc_1_1atxmega_1_1_waveform_e.html", [
        [ "xpcc::atxmega::TimerE0", "classxpcc_1_1atxmega_1_1_timer_e0.html", null ],
        [ "xpcc::atxmega::TimerE1", "classxpcc_1_1atxmega_1_1_timer_e1.html", null ]
      ] ],
      [ "xpcc::atxmega::WaveformF", "classxpcc_1_1atxmega_1_1_waveform_f.html", [
        [ "xpcc::atxmega::TimerF0", "classxpcc_1_1atxmega_1_1_timer_f0.html", null ],
        [ "xpcc::atxmega::TimerF1", "classxpcc_1_1atxmega_1_1_timer_f1.html", null ]
      ] ]
    ] ],
    [ "Class Members", "functions.html", null ],
    [ "Namespace List", "namespaces.html", [
      [ "std", "namespacestd.html", null ],
      [ "xpcc::at45db", "namespacexpcc_1_1at45db.html", null ],
      [ "xpcc::at91", "namespacexpcc_1_1at91.html", null ],
      [ "xpcc::atomic", "namespacexpcc_1_1atomic.html", null ],
      [ "xpcc::atxmega::adc", "namespacexpcc_1_1atxmega_1_1adc.html", null ],
      [ "xpcc::atxmega::dma", "namespacexpcc_1_1atxmega_1_1dma.html", null ],
      [ "xpcc::avr", "namespacexpcc_1_1avr.html", null ],
      [ "xpcc::ds1631", "namespacexpcc_1_1ds1631.html", null ],
      [ "xpcc::gpio", "namespacexpcc_1_1gpio.html", null ],
      [ "xpcc::i2c", "namespacexpcc_1_1i2c.html", null ],
      [ "xpcc::log", "namespacexpcc_1_1log.html", null ],
      [ "xpcc::one_wire", "namespacexpcc_1_1one__wire.html", null ],
      [ "xpcc::tmp", "namespacexpcc_1_1tmp.html", null ],
      [ "xpcc::x86", "namespacexpcc_1_1x86.html", null ]
    ] ],
    [ "Namespace Members", "namespacemembers.html", null ],
    [ "File List", "files.html", [
      [ "abort.h", null, null ],
      [ "abstract_component.hpp", null, null ],
      [ "accessor.hpp", null, null ],
      [ "action_callback.hpp", null, null ],
      [ "ad840x.hpp", null, null ],
      [ "arm7/at91/adc.hpp", null, null ],
      [ "avr/atmega/adc.hpp", null, null ],
      [ "avr/atxmega/adc/adc.hpp", null, null ],
      [ "avr/atxmega/adc.hpp", null, null ],
      [ "adc_interrupt.hpp", null, null ],
      [ "adca.hpp", null, null ],
      [ "adca_channel0.hpp", null, null ],
      [ "adca_channel1.hpp", null, null ],
      [ "adca_channel2.hpp", null, null ],
      [ "adca_channel3.hpp", null, null ],
      [ "adcb.hpp", null, null ],
      [ "adcb_channel0.hpp", null, null ],
      [ "adcb_channel1.hpp", null, null ],
      [ "adcb_channel2.hpp", null, null ],
      [ "adcb_channel3.hpp", null, null ],
      [ "adxl345.hpp", null, null ],
      [ "aic.hpp", null, null ],
      [ "algorithm.hpp", null, null ],
      [ "algorithm_base.hpp", null, null ],
      [ "all_caps_3x5.hpp", null, null ],
      [ "allocator.hpp", null, null ],
      [ "allocator_base.hpp", null, null ],
      [ "amnb.hpp", null, null ],
      [ "analog_sensors.hpp", null, null ],
      [ "angle.hpp", null, null ],
      [ "arcade_classic.hpp", null, null ],
      [ "architecture.hpp", null, null ],
      [ "arithmetic_traits.hpp", null, null ],
      [ "arm7.hpp", null, null ],
      [ "assertion.hpp", null, null ],
      [ "at45db0x1d.hpp", null, null ],
      [ "at91.hpp", null, null ],
      [ "atmega.hpp", null, null ],
      [ "atomic.hpp", null, null ],
      [ "attiny.hpp", null, null ],
      [ "atxmega.hpp", null, null ],
      [ "avr.hpp", null, null ],
      [ "awex_c.hpp", null, null ],
      [ "awex_d.hpp", null, null ],
      [ "awex_e.hpp", null, null ],
      [ "awex_f.hpp", null, null ],
      [ "backend_interface.hpp", null, null ],
      [ "bit_operation.hpp", null, null ],
      [ "block.hpp", null, null ],
      [ "bma180.hpp", null, null ],
      [ "bmp085.hpp", null, null ],
      [ "board.hpp", null, null ],
      [ "buffered_graphic_display.hpp", null, null ],
      [ "button.hpp", null, null ],
      [ "button_group.hpp", null, null ],
      [ "can.hpp", null, null ],
      [ "can_1.hpp", null, null ],
      [ "can_2.hpp", null, null ],
      [ "can_connector.hpp", null, null ],
      [ "can_filter.hpp", null, null ],
      [ "canusb.hpp", null, null ],
      [ "canusb_formater.hpp", null, null ],
      [ "character_display.hpp", null, null ],
      [ "circle_2d.hpp", null, null ],
      [ "driver/clock.hpp", null, null ],
      [ "platform/arm7/lpc/clock.hpp", null, null ],
      [ "platform/avr/atxmega/clock.hpp", null, null ],
      [ "platform/cortex_m3/stm32/clock.hpp", null, null ],
      [ "platform/cortex_m3/stm32/stm32f1/clock.hpp", null, null ],
      [ "platform/cortex_m3/stm32/stm32f2_4/clock.hpp", null, null ],
      [ "communicatable.hpp", null, null ],
      [ "communicatable_task.hpp", null, null ],
      [ "communication.hpp", null, null ],
      [ "communicator.hpp", null, null ],
      [ "constants.h", "constants_8h.html", null ],
      [ "amnb/constants.hpp", null, null ],
      [ "i2c/constants.hpp", null, null ],
      [ "sab/constants.hpp", null, null ],
      [ "sab2/constants.hpp", null, null ],
      [ "architecture/driver/atomic/container.hpp", null, null ],
      [ "container.hpp", null, null ],
      [ "controller.hpp", null, null ],
      [ "arm7/at91/core.hpp", null, null ],
      [ "cortex_m3/stm32/core.hpp", null, null ],
      [ "cortex_m0.hpp", null, null ],
      [ "cortex_m3.hpp", null, null ],
      [ "count_type.hpp", null, null ],
      [ "debounce.hpp", null, null ],
      [ "architecture/platform/arm7/at91/debug.hpp", null, null ],
      [ "debug.hpp", null, null ],
      [ "default_handler.h", null, null ],
      [ "delay.hpp", null, null ],
      [ "delegate.hpp", null, null ],
      [ "deque.hpp", null, null ],
      [ "detect.hpp", null, null ],
      [ "arm7/at91/device.h", null, null ],
      [ "cortex_m3/stm32/device.h", null, null ],
      [ "dispatcher.hpp", null, null ],
      [ "display.hpp", null, null ],
      [ "dma/dma.hpp", null, null ],
      [ "dma.hpp", null, null ],
      [ "dma_channel0.hpp", null, null ],
      [ "dma_channel1.hpp", null, null ],
      [ "dma_channel2.hpp", null, null ],
      [ "dma_channel3.hpp", null, null ],
      [ "dmac.hpp", null, null ],
      [ "double_indicator.hpp", null, null ],
      [ "doubly_linked_list.hpp", null, null ],
      [ "driver.hpp", null, null ],
      [ "ds1631.hpp", null, null ],
      [ "ds18b20.hpp", null, null ],
      [ "dummy.hpp", null, null ],
      [ "dynamic.hpp", null, null ],
      [ "dynamic_array.hpp", null, null ],
      [ "dynamic_postman.hpp", null, null ],
      [ "ea_dog.hpp", null, null ],
      [ "error_code.hpp", null, null ],
      [ "error_report.hpp", null, null ],
      [ "fat.hpp", null, null ],
      [ "filter.hpp", null, null ],
      [ "fixed_width_5x8.hpp", null, null ],
      [ "flag.hpp", null, null ],
      [ "driver/accessor/flash.hpp", null, null ],
      [ "platform/arm7/at91/flash.hpp", null, null ],
      [ "flash_reader.hpp", null, null ],
      [ "font.hpp", null, null ],
      [ "freertos.hpp", null, null ],
      [ "ft245.hpp", null, null ],
      [ "geometric_traits.hpp", null, null ],
      [ "geometry.hpp", null, null ],
      [ "architecture/driver/gpio.hpp", null, null ],
      [ "architecture/platform/arm7/at91/gpio.hpp", null, null ],
      [ "architecture/platform/arm7/lpc/gpio.hpp", null, null ],
      [ "architecture/platform/avr/atmega/gpio.hpp", null, null ],
      [ "architecture/platform/avr/atxmega/gpio.hpp", null, null ],
      [ "architecture/platform/cortex_m0/lpc/gpio.hpp", null, null ],
      [ "architecture/platform/cortex_m3/lpc/gpio.hpp", null, null ],
      [ "architecture/platform/cortex_m3/stm32/gpio.hpp", null, null ],
      [ "architecture/platform/cortex_m3/stm32/stm32f1/gpio.hpp", null, null ],
      [ "architecture/platform/cortex_m3/stm32/stm32f2_4/gpio.hpp", null, null ],
      [ "architecture/platform/hosted/gpio.hpp", null, null ],
      [ "driver/gpio.hpp", null, null ],
      [ "graphic_display.hpp", null, null ],
      [ "harness.hpp", null, null ],
      [ "hcla12x5.hpp", null, null ],
      [ "hd44780.hpp", null, null ],
      [ "architecture/platform/hosted/linux/tipc/header.hpp", null, null ],
      [ "communication/backend/header.hpp", null, null ],
      [ "hires_c.hpp", null, null ],
      [ "hires_d.hpp", null, null ],
      [ "hires_e.hpp", null, null ],
      [ "hires_f.hpp", null, null ],
      [ "hmc58.hpp", null, null ],
      [ "hmc5843.hpp", null, null ],
      [ "hmc5883l.hpp", null, null ],
      [ "home_16x16.hpp", null, null ],
      [ "hosted.hpp", null, null ],
      [ "architecture/platform/avr/atxmega/i2c.hpp", null, null ],
      [ "driver/connectivity/i2c.hpp", null, null ],
      [ "i2c_1.hpp", null, null ],
      [ "i2c_2.hpp", null, null ],
      [ "i2c_3.hpp", null, null ],
      [ "i2c_eeprom.hpp", null, null ],
      [ "i2c_master.hpp", null, null ],
      [ "i2c_master_c.hpp", null, null ],
      [ "i2c_master_d.hpp", null, null ],
      [ "i2c_master_e.hpp", null, null ],
      [ "i2c_master_f.hpp", null, null ],
      [ "image.hpp", null, null ],
      [ "indicator.hpp", null, null ],
      [ "input_capture.hpp", null, null ],
      [ "interconnect_interface.hpp", null, null ],
      [ "interconnect_link.hpp", null, null ],
      [ "connectivity/amnb/interface.hpp", null, null ],
      [ "connectivity/sab/interface.hpp", null, null ],
      [ "connectivity/sab2/interface.hpp", null, null ],
      [ "interface.hpp", null, null ],
      [ "interpolation.hpp", null, null ],
      [ "io.hpp", null, null ],
      [ "iodevice.hpp", null, null ],
      [ "iodevice_wrapper.hpp", null, null ],
      [ "iostream.hpp", null, null ],
      [ "ir_sensor.hpp", null, null ],
      [ "itg3200.hpp", null, null ],
      [ "ks0108.hpp", null, null ],
      [ "lagrange.hpp", null, null ],
      [ "led/led.hpp", null, null ],
      [ "led.hpp", null, null ],
      [ "led_table.hpp", null, null ],
      [ "level.hpp", null, null ],
      [ "line_2d.hpp", null, null ],
      [ "line_segment_2d.hpp", null, null ],
      [ "linear.hpp", null, null ],
      [ "linked_list.hpp", null, null ],
      [ "linux.hpp", null, null ],
      [ "lm75.hpp", null, null ],
      [ "location_2d.hpp", null, null ],
      [ "lock.hpp", null, null ],
      [ "logger/logger.hpp", null, null ],
      [ "logger.hpp", null, null ],
      [ "logo_eurobot_90x64.hpp", null, null ],
      [ "logo_rca_90x64.hpp", null, null ],
      [ "logo_xpcc_90x64.hpp", null, null ],
      [ "arm7/lpc.hpp", null, null ],
      [ "cortex_m0/lpc.hpp", null, null ],
      [ "cortex_m3/lpc.hpp", null, null ],
      [ "lpc11xx.hpp", null, null ],
      [ "lpc17xx.hpp", null, null ],
      [ "lpc213x.h", null, null ],
      [ "lpc21xx.h", "lpc21xx_8h.html", null ],
      [ "lpc23xx_24xx.h", null, null ],
      [ "lu_decomposition.hpp", null, null ],
      [ "macros.hpp", null, null ],
      [ "architecture/board/cpuboard2/master.hpp", null, null ],
      [ "driver/connectivity/i2c/master.hpp", null, null ],
      [ "driver/connectivity/sab/master.hpp", null, null ],
      [ "math.hpp", null, null ],
      [ "matrix.hpp", null, null ],
      [ "mcp23s08.hpp", null, null ],
      [ "mcp23s17.hpp", null, null ],
      [ "mcp2515.hpp", null, null ],
      [ "mcp2515_definitions.hpp", null, null ],
      [ "mcp4922.hpp", null, null ],
      [ "median.hpp", null, null ],
      [ "message.hpp", null, null ],
      [ "misc.hpp", null, null ],
      [ "moving_average.hpp", null, null ],
      [ "boost/mutex.hpp", null, null ],
      [ "freertos/mutex.hpp", null, null ],
      [ "mutex.hpp", null, null ],
      [ "node.hpp", null, null ],
      [ "nokia6610.hpp", null, null ],
      [ "nokia6610_defines.hpp", null, null ],
      [ "numbers_14x32.hpp", null, null ],
      [ "numbers_40x56.hpp", null, null ],
      [ "numbers_46x64.hpp", null, null ],
      [ "one_wire.hpp", null, null ],
      [ "operator.hpp", null, null ],
      [ "pair.hpp", null, null ],
      [ "periodic_timer.hpp", null, null ],
      [ "pid.hpp", null, null ],
      [ "pio.hpp", null, null ],
      [ "pit.hpp", null, null ],
      [ "platform.hpp", null, null ],
      [ "point_set_2d.hpp", null, null ],
      [ "polygon_2d.hpp", null, null ],
      [ "postman.hpp", null, null ],
      [ "prefix.hpp", null, null ],
      [ "protothread/protothread.hpp", null, null ],
      [ "protothread.hpp", null, null ],
      [ "pulse.hpp", null, null ],
      [ "architecture/platform/arm7/at91/pwm.hpp", null, null ],
      [ "driver/pwm.hpp", null, null ],
      [ "quaternion.hpp", null, null ],
      [ "architecture/driver/atomic/queue.hpp", null, null ],
      [ "container/queue.hpp", null, null ],
      [ "workflow/rtos/boost/queue.hpp", null, null ],
      [ "workflow/rtos/freertos/queue.hpp", null, null ],
      [ "workflow/rtos/queue.hpp", null, null ],
      [ "driver/accessor/ram.hpp", null, null ],
      [ "platform/avr/ram.hpp", null, null ],
      [ "ramp.hpp", null, null ],
      [ "registers.h", null, null ],
      [ "reporter.hpp", null, null ],
      [ "response_callback.hpp", null, null ],
      [ "response_handle.hpp", null, null ],
      [ "rgb.hpp", null, null ],
      [ "rtos.hpp", null, null ],
      [ "s_curve_controller.hpp", null, null ],
      [ "s_curve_generator.hpp", null, null ],
      [ "sab.hpp", null, null ],
      [ "sample_header.hpp", null, null ],
      [ "saturated.hpp", null, null ],
      [ "rtos/boost/scheduler.hpp", null, null ],
      [ "rtos/freertos/scheduler.hpp", null, null ],
      [ "rtos/scheduler.hpp", null, null ],
      [ "scheduler/scheduler.hpp", null, null ],
      [ "scp1000.hpp", null, null ],
      [ "scripto_narrow.hpp", null, null ],
      [ "sd.hpp", null, null ],
      [ "sd_constants.hpp", null, null ],
      [ "protothread/semaphore.hpp", null, null ],
      [ "rtos/boost/semaphore.hpp", null, null ],
      [ "rtos/freertos/semaphore.hpp", null, null ],
      [ "rtos/semaphore.hpp", null, null ],
      [ "serial_interface.hpp", null, null ],
      [ "serial_port.hpp", null, null ],
      [ "servo.hpp", null, null ],
      [ "shift_register_input.hpp", null, null ],
      [ "shift_register_output.hpp", null, null ],
      [ "skull_64x64.hpp", null, null ],
      [ "architecture/board/cpuboard2/slave.hpp", null, null ],
      [ "driver/connectivity/sab/slave.hpp", null, null ],
      [ "smart_pointer.hpp", null, null ],
      [ "software_i2c.hpp", null, null ],
      [ "software_one_wire.hpp", null, null ],
      [ "software_spi.hpp", null, null ],
      [ "architecture/platform/arm7/at91/spi.hpp", null, null ],
      [ "architecture/platform/arm7/lpc/spi.hpp", null, null ],
      [ "architecture/platform/avr/atmega/spi.hpp", null, null ],
      [ "architecture/platform/avr/atxmega/spi/spi.hpp", null, null ],
      [ "architecture/platform/avr/atxmega/spi.hpp", null, null ],
      [ "driver/connectivity/spi.hpp", null, null ],
      [ "spi_0.hpp", null, null ],
      [ "arm7/at91/spi/spi_1.hpp", null, null ],
      [ "cortex_m3/stm32/spi/spi_1.hpp", null, null ],
      [ "spi_2.hpp", null, null ],
      [ "spi_3.hpp", null, null ],
      [ "spi_c.hpp", null, null ],
      [ "spi_d.hpp", null, null ],
      [ "spi_e.hpp", null, null ],
      [ "spi_f.hpp", null, null ],
      [ "spi_master.hpp", null, null ],
      [ "spi_ram.hpp", null, null ],
      [ "ssc.hpp", null, null ],
      [ "st7036.hpp", null, null ],
      [ "st7565.hpp", null, null ],
      [ "st7565_defines.hpp", null, null ],
      [ "stack.hpp", null, null ],
      [ "static.hpp", null, null ],
      [ "std.hpp", null, null ],
      [ "std_colour.hpp", null, null ],
      [ "stm32.hpp", null, null ],
      [ "style.hpp", null, null ],
      [ "style_wrapper.hpp", null, null ],
      [ "swi.h", null, null ],
      [ "sync_master.hpp", null, null ],
      [ "systick_timer.hpp", null, null ],
      [ "task.hpp", null, null ],
      [ "template_metaprogramming.hpp", null, null ],
      [ "terminal.hpp", null, null ],
      [ "testsuite.hpp", null, null ],
      [ "boost/thread.hpp", null, null ],
      [ "freertos/thread.hpp", null, null ],
      [ "thread.hpp", null, null ],
      [ "timeout.hpp", null, null ],
      [ "arm7/at91/timer.hpp", null, null ],
      [ "avr/atxmega/timer/timer.hpp", null, null ],
      [ "avr/atxmega/timer.hpp", null, null ],
      [ "cortex_m3/stm32/timer.hpp", null, null ],
      [ "timer_0.hpp", null, null ],
      [ "arm7/at91/timer/timer_1.hpp", null, null ],
      [ "cortex_m3/stm32/timer/timer_1.hpp", null, null ],
      [ "arm7/at91/timer/timer_2.hpp", null, null ],
      [ "cortex_m3/stm32/timer/timer_2.hpp", null, null ],
      [ "timer_3.hpp", null, null ],
      [ "timer_4.hpp", null, null ],
      [ "timer_5.hpp", null, null ],
      [ "timer_6.hpp", null, null ],
      [ "timer_7.hpp", null, null ],
      [ "timer_8.hpp", null, null ],
      [ "timer_base.hpp", null, null ],
      [ "timer_c0.hpp", null, null ],
      [ "timer_c1.hpp", null, null ],
      [ "timer_d0.hpp", null, null ],
      [ "timer_d1.hpp", null, null ],
      [ "timer_e0.hpp", null, null ],
      [ "timer_e1.hpp", null, null ],
      [ "timer_f0.hpp", null, null ],
      [ "timer_f1.hpp", null, null ],
      [ "timer_interrupt_c0.hpp", null, null ],
      [ "timer_interrupt_c1.hpp", null, null ],
      [ "timer_interrupt_d0.hpp", null, null ],
      [ "timer_interrupt_d1.hpp", null, null ],
      [ "timer_interrupt_e0.hpp", null, null ],
      [ "timer_interrupt_e1.hpp", null, null ],
      [ "timer_interrupt_f0.hpp", null, null ],
      [ "timer_interrupt_f1.hpp", null, null ],
      [ "timestamp.hpp", null, null ],
      [ "architecture/platform/hosted/linux/tipc.hpp", null, null ],
      [ "communication/backend/tipc/tipc.hpp", null, null ],
      [ "tipc_receiver.hpp", null, null ],
      [ "tipc_receiver_socket.hpp", null, null ],
      [ "tipc_transmitter.hpp", null, null ],
      [ "tipc_transmitter_socket.hpp", null, null ],
      [ "tlc594x.hpp", null, null ],
      [ "tlc594x_led.hpp", null, null ],
      [ "tmp102.hpp", null, null ],
      [ "twi.hpp", null, null ],
      [ "uart.h", null, null ],
      [ "arm7/at91/uart.hpp", null, null ],
      [ "arm7/lpc/uart.hpp", null, null ],
      [ "avr/atmega/uart.hpp", null, null ],
      [ "avr/atxmega/uart.hpp", null, null ],
      [ "arm7/lpc/uart/uart0.hpp", null, null ],
      [ "avr/atmega/uart/uart0.hpp", null, null ],
      [ "arm7/lpc/uart/uart1.hpp", null, null ],
      [ "avr/atmega/uart/uart1.hpp", null, null ],
      [ "arm7/lpc/uart/uart2.hpp", null, null ],
      [ "avr/atmega/uart/uart2.hpp", null, null ],
      [ "arm7/lpc/uart/uart3.hpp", null, null ],
      [ "avr/atmega/uart/uart3.hpp", null, null ],
      [ "uart_0.hpp", null, null ],
      [ "uart_1.hpp", null, null ],
      [ "uart_4.hpp", null, null ],
      [ "uart_5.hpp", null, null ],
      [ "uart_base.hpp", null, null ],
      [ "uart_c0.hpp", null, null ],
      [ "uart_c1.hpp", null, null ],
      [ "uart_d0.hpp", null, null ],
      [ "uart_d1.hpp", null, null ],
      [ "uart_defines.h", null, null ],
      [ "uart_e0.hpp", null, null ],
      [ "uart_e1.hpp", null, null ],
      [ "uart_f0.hpp", null, null ],
      [ "uart_f1.hpp", null, null ],
      [ "uart_settings.h", null, null ],
      [ "unittest.hpp", null, null ],
      [ "unix.hpp", null, null ],
      [ "usart_1.hpp", null, null ],
      [ "usart_2.hpp", null, null ],
      [ "usart_3.hpp", null, null ],
      [ "usart_6.hpp", null, null ],
      [ "usb.hpp", null, null ],
      [ "architecture/platform/avr/atxmega/utils.hpp", null, null ],
      [ "architecture/utils.hpp", null, null ],
      [ "math/utils.hpp", null, null ],
      [ "utils.hpp", null, null ],
      [ "vector.hpp", null, null ],
      [ "vector1.hpp", null, null ],
      [ "vector2.hpp", null, null ],
      [ "vector3.hpp", null, null ],
      [ "vector4.hpp", null, null ],
      [ "vic.hpp", null, null ],
      [ "watchdog.hpp", null, null ],
      [ "wdt.hpp", null, null ],
      [ "windows.hpp", null, null ],
      [ "workflow.hpp", null, null ]
    ] ],
    [ "Directories", "dirs.html", [
      [ "examples", "dir_aa9c93782e26928167c2443b7194f1c5.html", null ],
      [ "src", "dir_d40dd980f57df23ff7e2f63a41db3f31.html", [
        [ "stdc++", "dir_ab7a50be99fe86967ac0f41167e33741.html", [
          [ "internal", "dir_8728382fd2928a772d83f0e06644907f.html", null ]
        ] ],
        [ "unittest", "dir_e264c6859cec842d74ef45e3f4f9f7d0.html", [
          [ "type", "dir_94800eedf365d7a0db07fecda9ad46b9.html", null ]
        ] ],
        [ "xpcc", "dir_85f4f6923954e385a001b7abeda2a113.html", [
          [ "architecture", "dir_382616c1f9706b54efa3a07e0e4d6ebe.html", [
            [ "board", "dir_abe6263cc440c8fac810fb400d29616e.html", [
              [ "cpuboard2", "dir_9e948aca0a2fd44213ea27cc57683fd5.html", null ]
            ] ],
            [ "driver", "dir_63e4ec377a82e1234181ed370c1dfb4b.html", [
              [ "accessor", "dir_51233c3e759f5719de407c89e1696b9e.html", null ],
              [ "atomic", "dir_40ec3462bce546e47fd8a6008fa1dc15.html", null ]
            ] ],
            [ "platform", "dir_c3a6c8e4d899baf3e898538ba2ef81e2.html", [
              [ "arm7", "dir_8ff387df49c2b38683326490577d6eb0.html", [
                [ "at91", "dir_a654890fe44e39667279f7cf838cf419.html", [
                  [ "spi", "dir_f9c595c921b94e6edc0badec3ce7c547.html", null ],
                  [ "timer", "dir_a1fd8a4b41edff0710106938d2976ac6.html", null ],
                  [ "uart", "dir_5647e64f9da2fc95e40adc2137738843.html", null ]
                ] ],
                [ "common", "dir_3ff4ea3dd7b3c808d2bb7284efaa6a54.html", null ],
                [ "lpc", "dir_25798ce0505181a8709f6ce93079aff7.html", [
                  [ "device", "dir_1ba3600905f5a2ecfd810b44e6c313f1.html", null ],
                  [ "uart", "dir_7a80084e7c34da8057ef4adcd7169da1.html", null ]
                ] ]
              ] ],
              [ "avr", "dir_76213ae78826cad8ef0433d0085c6d05.html", [
                [ "atmega", "dir_511d4b489dd9ab2fede75d357334a6f4.html", [
                  [ "uart", "dir_24d6288ec001b8f7083cd032a85da65f.html", null ]
                ] ],
                [ "atxmega", "dir_088bb4a76693a75d3cbbc7b900d8fa3a.html", [
                  [ "adc", "dir_5a3a027ad1ca130c4796505229510101.html", null ],
                  [ "dma", "dir_80c49c8dc08e2f9969002239f6f65f12.html", null ],
                  [ "i2c", "dir_c33e5188969beaf5de96d27ee9796e34.html", null ],
                  [ "spi", "dir_314c720fa29fbc81b4a9f373d968f40f.html", null ],
                  [ "timer", "dir_1511c8703298e4b0a89757191e361bcf.html", null ],
                  [ "uart", "dir_25bdad3972940979b838aad1502781e1.html", null ]
                ] ]
              ] ],
              [ "cortex_m0", "dir_d22c0794fb9893804db5f89baddb39b3.html", [
                [ "lpc", "dir_cc399d9b090ead3715dd4fc2396b9207.html", null ]
              ] ],
              [ "cortex_m3", "dir_a4adf0d976e11a8283b4de5de205afbb.html", [
                [ "lpc", "dir_6e71c036c724e92ad011e11392ce9078.html", null ],
                [ "stm32", "dir_13fbdbdbf1e60b2ed53442bb05f5d08a.html", [
                  [ "can", "dir_e5dd2c7fd5fdf46a16527ac7f7322281.html", null ],
                  [ "i2c", "dir_7e8851092c7205ecbaf4f98cdda8df42.html", null ],
                  [ "spi", "dir_8ec0f253219ed7cebd019a56283e7639.html", null ],
                  [ "stm32f1", "dir_78603629ea1fe5044c26c716921eed42.html", null ],
                  [ "stm32f2_4", "dir_7b9ed7409e38787e940bd56186a83401.html", null ],
                  [ "timer", "dir_28b621c6fa7185cc7902459342da9ccf.html", null ],
                  [ "uart", "dir_240c4a314ca87e21b6de68d36c549467.html", null ]
                ] ]
              ] ],
              [ "hosted", "dir_1daf0840e6e64cec3c7c0e6f5d75ab27.html", [
                [ "linux", "dir_d77a52d64ca8f217227e95e0ee3a194b.html", [
                  [ "tipc", "dir_db62a9a964be6cde25601e83d0f7bd4c.html", null ]
                ] ]
              ] ]
            ] ]
          ] ],
          [ "communication", "dir_a0aa247b7959c1e9a882d5dcc26a42cf.html", [
            [ "backend", "dir_4cc5c4c6a8bd6925ea3c723c14cd8753.html", [
              [ "can", "dir_ee4c21a5958d9c8b20e34fa6448e05ea.html", null ],
              [ "tipc", "dir_4e17827dadc890e8a7b8b49ee1c89ab0.html", null ]
            ] ],
            [ "postman", "dir_a7994ef9a9b8cbb3bab1e5863f170024.html", null ]
          ] ],
          [ "container", "dir_7cdd593e678ebe9c1005994cb4d0cc0b.html", null ],
          [ "debug", "dir_062aaf99f1f469f3a1ca9ddd41893aaa.html", [
            [ "logger", "dir_b3c4dbf83d4160707230059d8156cc28.html", [
              [ "imp", "dir_566666af3db54bbd225819d753300519.html", null ],
              [ "style", "dir_79db4a187204e28ad2f6055b9a4e07ca.html", null ]
            ] ]
          ] ],
          [ "driver", "dir_547c20d8d8d2ad175d25ec48447f15d3.html", [
            [ "connectivity", "dir_777e559ad3431bd5a262f3eb16e9f484.html", [
              [ "amnb", "dir_b7ab7aaf9ea9b4858d5a5fa96a7425fb.html", null ],
              [ "can", "dir_52aa485eff977c469f872903e40bb441.html", [
                [ "canusb", "dir_0efda78b0ed9d64a273b4c0fc9144c54.html", null ],
                [ "mcp2515", "dir_ffb668fc6f83eba49f60a10fa82a0f18.html", null ]
              ] ],
              [ "i2c", "dir_fa46840c48764d09ad83924dd2cba324.html", null ],
              [ "interconnect", "dir_a1c3cf5a3de5785da7eb089a87874a64.html", null ],
              [ "sab", "dir_6a26fded5495da9a2020c25bcca3c7fe.html", [
                [ "device", "dir_53b2a79fd21faef7d12e7ad2f1589187.html", null ]
              ] ],
              [ "sab2", "dir_fa4793998fe275a81eaed4dd58280961.html", null ],
              [ "spi", "dir_467edd4a1a7d289ee72317c347c2a00c.html", null ]
            ] ],
            [ "dac", "dir_dccde9f7345b513474a2e9645f85f7a2.html", null ],
            [ "gpio", "dir_0f12b56af8e0c35c56c8af50f3cc7ec0.html", null ],
            [ "inertial", "dir_019b6314a32f7dc3066e3ae50b289b67.html", null ],
            [ "other", "dir_9408f3bd6f79d90f7aa7e564c2910724.html", null ],
            [ "pressure", "dir_6ec90f2843d457e6d3d892317f5f3088.html", null ],
            [ "pwm", "dir_0060740b3b85bfdec31ec93e21343640.html", null ],
            [ "storage", "dir_d393bc64ef41462d89ea393d3c43133c.html", null ],
            [ "temperature", "dir_229a559b0a07fcbb309dc1887569a47d.html", null ],
            [ "ui", "dir_b93f4c615a05ad892d6faf70486344de.html", [
              [ "display", "dir_a2f82854850c4ed6ee9ef5ae2ee02b2a.html", [
                [ "font", "dir_9d1f2bd2eb0c44062f5f7ce0269b531c.html", null ],
                [ "image", "dir_99841e68387d21114bed5af5bc22d982.html", null ]
              ] ],
              [ "led", "dir_e5b599da680a549afc0dcc6cf9b845b9.html", null ]
            ] ]
          ] ],
          [ "io", "dir_3fc4b10346837b6ea48c559455fa7e4d.html", null ],
          [ "math", "dir_f288d852a587a445522d6a47cd390583.html", [
            [ "filter", "dir_42e2e66ea9b8c169e836c1dbe1b6f084.html", null ],
            [ "geometry", "dir_319f1a0e1677c3f3404f2e93405a7e9d.html", null ],
            [ "interpolation", "dir_693a436dc1b392e2940e6ee7213e1046.html", null ],
            [ "saturated", "dir_8fc1f8f4a254775558bf3bc4bf6b3d1d.html", null ],
            [ "utils", "dir_d6a917ffcff273dc80205b5d4d6c2753.html", null ]
          ] ],
          [ "utils", "dir_eafb5121b1b752184703fd84ea0a4a50.html", [
            [ "allocator", "dir_9ce26723bc6a4f68378ccb26ba9ef6f0.html", null ]
          ] ],
          [ "workflow", "dir_8f0a31b3cec8acd27341bb48dfe577b3.html", [
            [ "protothread", "dir_601634961ee43192e512663898855a03.html", null ],
            [ "rtos", "dir_0f7850559a2b8315792a56a616cb8a85.html", [
              [ "boost", "dir_910a52842078ec73f8076d060e5437ce.html", null ],
              [ "freertos", "dir_b0c55a92dfc7771fafb5296b72382c22.html", null ]
            ] ],
            [ "scheduler", "dir_7d8a759e74c3d2ed8c0efb044e007583.html", null ]
          ] ]
        ] ]
      ] ]
    ] ]
  ] ]
];

function createIndent(o,domNode,node,level)
{
  if (node.parentNode && node.parentNode.parentNode)
  {
    createIndent(o,domNode,node.parentNode,level+1);
  }
  var imgNode = document.createElement("img");
  if (level==0 && node.childrenData)
  {
    node.plus_img = imgNode;
    node.expandToggle = document.createElement("a");
    node.expandToggle.href = "javascript:void(0)";
    node.expandToggle.onclick = function() 
    {
      if (node.expanded) 
      {
        $(node.getChildrenUL()).slideUp("fast");
        if (node.isLast)
        {
          node.plus_img.src = node.relpath+"ftv2plastnode.png";
        }
        else
        {
          node.plus_img.src = node.relpath+"ftv2pnode.png";
        }
        node.expanded = false;
      } 
      else 
      {
        expandNode(o, node, false);
      }
    }
    node.expandToggle.appendChild(imgNode);
    domNode.appendChild(node.expandToggle);
  }
  else
  {
    domNode.appendChild(imgNode);
  }
  if (level==0)
  {
    if (node.isLast)
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2plastnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2lastnode.png";
        domNode.appendChild(imgNode);
      }
    }
    else
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2pnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2node.png";
        domNode.appendChild(imgNode);
      }
    }
  }
  else
  {
    if (node.isLast)
    {
      imgNode.src = node.relpath+"ftv2blank.png";
    }
    else
    {
      imgNode.src = node.relpath+"ftv2vertline.png";
    }
  }
  imgNode.border = "0";
}

function newNode(o, po, text, link, childrenData, lastNode)
{
  var node = new Object();
  node.children = Array();
  node.childrenData = childrenData;
  node.depth = po.depth + 1;
  node.relpath = po.relpath;
  node.isLast = lastNode;

  node.li = document.createElement("li");
  po.getChildrenUL().appendChild(node.li);
  node.parentNode = po;

  node.itemDiv = document.createElement("div");
  node.itemDiv.className = "item";

  node.labelSpan = document.createElement("span");
  node.labelSpan.className = "label";

  createIndent(o,node.itemDiv,node,0);
  node.itemDiv.appendChild(node.labelSpan);
  node.li.appendChild(node.itemDiv);

  var a = document.createElement("a");
  node.labelSpan.appendChild(a);
  node.label = document.createTextNode(text);
  a.appendChild(node.label);
  if (link) 
  {
    a.href = node.relpath+link;
  } 
  else 
  {
    if (childrenData != null) 
    {
      a.className = "nolink";
      a.href = "javascript:void(0)";
      a.onclick = node.expandToggle.onclick;
      node.expanded = false;
    }
  }

  node.childrenUL = null;
  node.getChildrenUL = function() 
  {
    if (!node.childrenUL) 
    {
      node.childrenUL = document.createElement("ul");
      node.childrenUL.className = "children_ul";
      node.childrenUL.style.display = "none";
      node.li.appendChild(node.childrenUL);
    }
    return node.childrenUL;
  };

  return node;
}

function showRoot()
{
  var headerHeight = $("#top").height();
  var footerHeight = $("#nav-path").height();
  var windowHeight = $(window).height() - headerHeight - footerHeight;
  navtree.scrollTo('#selected',0,{offset:-windowHeight/2});
}

function expandNode(o, node, imm)
{
  if (node.childrenData && !node.expanded) 
  {
    if (!node.childrenVisited) 
    {
      getNode(o, node);
    }
    if (imm)
    {
      $(node.getChildrenUL()).show();
    } 
    else 
    {
      $(node.getChildrenUL()).slideDown("fast",showRoot);
    }
    if (node.isLast)
    {
      node.plus_img.src = node.relpath+"ftv2mlastnode.png";
    }
    else
    {
      node.plus_img.src = node.relpath+"ftv2mnode.png";
    }
    node.expanded = true;
  }
}

function getNode(o, po)
{
  po.childrenVisited = true;
  var l = po.childrenData.length-1;
  for (var i in po.childrenData) 
  {
    var nodeData = po.childrenData[i];
    po.children[i] = newNode(o, po, nodeData[0], nodeData[1], nodeData[2],
        i==l);
  }
}

function findNavTreePage(url, data)
{
  var nodes = data;
  var result = null;
  for (var i in nodes) 
  {
    var d = nodes[i];
    if (d[1] == url) 
    {
      return new Array(i);
    }
    else if (d[2] != null) // array of children
    {
      result = findNavTreePage(url, d[2]);
      if (result != null) 
      {
        return (new Array(i).concat(result));
      }
    }
  }
  return null;
}

function initNavTree(toroot,relpath)
{
  var o = new Object();
  o.toroot = toroot;
  o.node = new Object();
  o.node.li = document.getElementById("nav-tree-contents");
  o.node.childrenData = NAVTREE;
  o.node.children = new Array();
  o.node.childrenUL = document.createElement("ul");
  o.node.getChildrenUL = function() { return o.node.childrenUL; };
  o.node.li.appendChild(o.node.childrenUL);
  o.node.depth = 0;
  o.node.relpath = relpath;

  getNode(o, o.node);

  o.breadcrumbs = findNavTreePage(toroot, NAVTREE);
  if (o.breadcrumbs == null)
  {
    o.breadcrumbs = findNavTreePage("index.html",NAVTREE);
  }
  if (o.breadcrumbs != null && o.breadcrumbs.length>0)
  {
    var p = o.node;
    for (var i in o.breadcrumbs) 
    {
      var j = o.breadcrumbs[i];
      p = p.children[j];
      expandNode(o,p,true);
    }
    p.itemDiv.className = p.itemDiv.className + " selected";
    p.itemDiv.id = "selected";
    $(window).load(showRoot);
  }
}

