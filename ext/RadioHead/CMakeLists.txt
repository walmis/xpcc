
target_sources(xpcc PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/RHSPIDriver.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHNRFSPIDriver.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RH_ASK.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHRouter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHDatagram.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHCRC.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RH_NRF905.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHSoftwareSPI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHGenericSPI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RH_TCP.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RH_NRF24.cpp
    #${CMAKE_CURRENT_LIST_DIR}STM32ArduinoCompat/HardwareSerial.cpp
    #${CMAKE_CURRENT_LIST_DIR}STM32ArduinoCompat/wirish.cpp
    #${CMAKE_CURRENT_LIST_DIR}STM32ArduinoCompat/HardwareSPI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RH_RF22.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHReliableDatagram.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHMesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHHardwareSPI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHGenericDriver.cpp
    #${CMAKE_CURRENT_LIST_DIR}/tools/simMain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RH_RF69.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RH_RF95.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RH_Serial.cpp

)

#
# Set flags
#
target_include_directories(xpcc PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
