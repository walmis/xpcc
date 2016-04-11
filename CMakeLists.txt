cmake_minimum_required(VERSION 3.1)
INCLUDE(CMakeForceCompiler)
 
project(RadioHead C CXX ASM)

#
# Build options
#
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(DEFAULT_PLATFORM "linux")
endif()

# options are listed in a table format below
set(opts
    # name: type: default value: string options list : description
    "CMAKE_BUILD_TYPE:STRING:RelWithDebInfo:Debug Release RelWithDebInfo MinSizeRel:Build type."
    "CMAKE_CXX_FLAGS:STRING:::C++ flags."
    "CMAKE_C_FLAGS:STRING:::C flags."
    "PLATFORM:STRING:generic:cortex-m0 cortex-m3 cortex-m4:Platform."
)
foreach(_opt ${opts})
    # arguments are : delimited
    string(REPLACE ":" ";" _opt ${_opt})
    list(GET _opt 0 _name)
    list(GET _opt 1 _type)
    list(GET _opt 2 _default)
    list(GET _opt 3 _options)
    list(GET _opt 4 _descr)
    # options are space delimited
    string(REPLACE " " ";" _options "${_options}")
    # if a default has not already been defined, use default from table
    if(NOT DEFINED DEFAULT_${_name})
        set(DEFAULT_${_name} ${_default})
    endif()
    # option has not been set already or it is empty, set it with the default
    if(NOT DEFINED ${_name} OR ${_name} STREQUAL "")
        set(${_name} ${DEFAULT_${_name}})
    endif()
    # create a cache from the variable and force it to set
    message(STATUS "${_name}\t: ${${_name}} : ${_descr}")
    set("${_name}" "${${_name}}" CACHE "${_type}" "${_descr}" FORCE)
    # if an options list is provided for the cache, set it
    if("${_type}" STREQUAL "STRING" AND NOT "${_options}" STREQUAL "")
        set_property(CACHE ${_name} PROPERTY STRINGS ${_options})
    endif()
endforeach()

SET(SOURCES 
    ./RHSPIDriver.cpp
    ./RHNRFSPIDriver.cpp
    ./RH_ASK.cpp
    ./RHRouter.cpp
    ./RHDatagram.cpp
    ./RHCRC.cpp
    ./RH_NRF905.cpp
    ./RHSoftwareSPI.cpp
    ./RHGenericSPI.cpp
    ./RH_TCP.cpp
    ./RH_NRF24.cpp
    #./STM32ArduinoCompat/HardwareSerial.cpp
    #./STM32ArduinoCompat/wirish.cpp
    #./STM32ArduinoCompat/HardwareSPI.cpp
    ./RH_RF22.cpp
    ./RHReliableDatagram.cpp
    ./RHMesh.cpp
    ./RHHardwareSPI.cpp
    ./RHGenericDriver.cpp
    ./tools/simMain.cpp
    ./RH_RF69.cpp
    ./RH_RF95.cpp
    ./RH_Serial.cpp

)

#
# Set flags
#
include_directories(
    .
)

#enable_language(C++ C ASM)

SET(ARM_COMMON_FLAGS "-mthumb -mthumb-interwork -ffunction-sections -fdata-sections -g -fno-common -fmessage-length=0")

SET(ASM_OPTIONS "-x assembler-with-cpp")
SET(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}" )

# drivers
if (${PLATFORM} STREQUAL "linux")
    message(STATUS "Adding Linux platform driver")


elseif(${PLATFORM} STREQUAL "cortex-m4")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR STM32F407)
    CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
    CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)
    
    SET(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
     
    SET(COMMON_FLAGS "${ARM_COMMON_FLAGS} -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
    
    SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=gnu++11 -fno-rtti -fno-exceptions")
    SET(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99")
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections ")
    
    add_definitions(-DSTM32F407xx)
    add_definitions(-DSTM32F4XX)
    add_definitions(-D__ARM_STM32__)


    message(STATUS "Adding STM32F4 platform driver")

elseif(${PLATFORM} STREQUAL "cortex-m0")  

    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR LPC11xx)
    CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
    CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU) 
    SET(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
     
    SET(COMMON_FLAGS "${ARM_COMMON_FLAGS} -mcpu=cortex-m0 -g")
    
    SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=gnu++11 -fno-rtti -fno-exceptions")
    SET(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99")
    SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections ")
    SET(USB_SUPPORT false)
    
    add_definitions(-D__ARM_LPC11XX__)
    

elseif(${PLATFORM} STREQUAL "cortex-m3")   
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR LPC17xx)
    CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
    CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU) 
    SET(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
     
    SET(COMMON_FLAGS "${ARM_COMMON_FLAGS} -mcpu=cortex-m3 -g")
    
    SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=gnu++11 -fno-rtti -fno-exceptions")
    SET(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99")
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections ")
    
    add_definitions(-D__ARM_LPC17XX__)
    
endif()

add_library(RadioHead STATIC ${SOURCES})

get_property(incDirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
target_include_directories(RadioHead PUBLIC ${incDirs} )



# Add all targets to the build-tree export set
export(TARGETS RadioHead
  FILE "${PROJECT_BINARY_DIR}/RadioHeadConfig.cmake")
 
# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE RadioHead)

# Create the FooBarConfig.cmake and FooBarConfigVersion files
#file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"  "${INSTALL_INCLUDE_DIR}")
   
# ... for the build tree
#set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")

#configure_file(xpccConfig.cmake.in
#  "${PROJECT_BINARY_DIR}/xpccConfig.cmake" @ONLY)
  
# ... for the install tree
#set(CONF_INCLUDE_DIRS "\${FOOBAR_CMAKE_DIR}/${REL_INCLUDE_DIR}")

#configure_file(xpccConfig.cmake.in  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/xpccConfig.cmake" @ONLY)

 

  
# vim: set et ft=cmake fenc=utf-8 ff=unix sts=4 sw=4 ts=4 :

