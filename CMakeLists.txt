cmake_minimum_required(VERSION 3.5)

set(CMAKE_SYSTEM_NAME Generic CACHE INTERNAL "")
#SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS CACHE INTERNAL "")
#SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS CACHE INTERNAL "")

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

# options are listed in a table format below
set(opts
    # name: type: default value: string options list : description
    "CMAKE_BUILD_TYPE:STRING:MinSizeRel:Debug Release RelWithDebInfo MinSizeRel:Build type."
    "PLATFORM:STRING:generic:generic linux stm32:Platform."
    "CHIBI_RTOS:BOOL:FALSE:false true:Build chibi RTOS support."
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

macro (add_sources)
    file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND SOURCES "${_relPath}/${_src}")
        else()
            list (APPEND SOURCES "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate SRCS to parent directory
        set (SOURCES ${SOURCES} PARENT_SCOPE)
    endif()
endmacro()

#
# Set flags
#
include_directories(
    ./src
    ./ext
)

SET(ARM_COMMON_FLAGS "-ffreestanding -mthumb -ffunction-sections -fdata-sections -g3 -fno-common -fmessage-length=0")
SET(ARM_COMMON_LD_FLAGS "-ffreestanding -nostartfiles -Wl,-gc-sections --specs=nano.specs -u _sbrk")
SET(ASM_OPTIONS "-x assembler-with-cpp")

IF(${ENABLE_LTO})
  SET(ARM_COMMON_FLAGS "${ARM_COMMON_FLAGS} -flto")
  SET(ARM_COMMON_LD_FLAGS "${ARM_COMMON_LD_FLAGS} -flto -Os")
  message(STATUS "Link time optimization ENABLED")
ENDIF()

macro(set_arm_settings CORTEX_TYPE)
    message(STATUS "Configuring for ${CORTEX_TYPE}")
    set(CMAKE_SYSTEM_NAME Generic CACHE INTERNAL "")
    set(CMAKE_SYSTEM_PROCESSOR ${CORTEX_TYPE} CACHE INTERNAL "")

    SET(CMAKE_CXX_COMPILER arm-none-eabi-g++ CACHE INTERNAL "" )
    SET(CMAKE_C_COMPILER arm-none-eabi-gcc CACHE INTERNAL "" )
    SET(CMAKE_ASM_COMPILER arm-none-eabi-gcc CACHE INTERNAL "")
    SET(CMAKE_AR arm-none-eabi-gcc-ar CACHE INTERNAL "")
    SET(CMAKE_RANLIB arm-none-eabi-gcc-ranlib CACHE INTERNAL "")
    SET(CMAKE_OBJCOPY arm-none-eabi-objcopy CACHE INTERNAL "")
    SET(CMAKE_OBJSIZE arm-none-eabi-size CACHE INTERNAL "")
    SET(CMAKE_OBJDUMP arm-none-eabi-objdump CACHE INTERNAL "")

    SET(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}" CACHE INTERNAL "")

    SET(COMMON_FLAGS "${ARM_COMMON_FLAGS} -mcpu=${CORTEX_TYPE}")

    if(${CORTEX_TYPE} STREQUAL "cortex-m4")
        SET(COMMON_FLAGS "${COMMON_FLAGS} -mfloat-abi=hard -mfpu=fpv4-sp-d16")
    endif()

    SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=gnu++11 -fno-rtti -fno-exceptions" CACHE INTERNAL "")
    SET(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99" CACHE INTERNAL "")

    set(CMAKE_EXE_LINKER_FLAGS "${ARM_COMMON_LD_FLAGS}" CACHE INTERNAL "")

endmacro()

# drivers
if (${PLATFORM} STREQUAL "linux")
    message(STATUS "Adding Linux platform driver")


elseif(${PLATFORM} STREQUAL "stm32f4")

    set_arm_settings("cortex-m4")

    #set(CMAKE_C_COMPILER_ARG1 -mthumb -mcpu=cortex-m4)
    #set(CMAKE_CXX_COMPILER_ARG1 -mthumb -mcpu=cortex-m4)

    SET(USB_SUPPORT true)

    add_definitions(-DSTM32F407xx)
    add_definitions(-DSTM32F4XX)
    add_definitions(-D__ARM_STM32__)

    include_directories(
        ext/cmsis/stm32/Include/
        ext/cmsis/stm32/Device/ST/STM32F4xx/Include/
    )

    # if (${CHIBI_RTOS})
    #
    #    include_directories(
    #         src/xpcc/architecture/platform/cortex_m3/stm32/stm32f4/chibi/
    #    )
    #
    # endif()

    message(STATUS "Adding STM32F4 platform driver")
    add_subdirectory(src/xpcc/architecture/platform/cortex_m3/stm32/stm32f4)

elseif(${PLATFORM} STREQUAL "lpc11xx")

    set_arm_settings("cortex-m0")

    SET(USB_SUPPORT false)

    add_definitions(-D__ARM_LPC11XX__)

    include_directories(
       ext/lpc11xx/cmsis/
    )

    add_subdirectory(src/xpcc/architecture/platform/cortex_m0/lpc/lpc11)

    message(STATUS "Adding LPC11xx platform driver")

elseif(${PLATFORM} STREQUAL "lpc11Uxx")

    set_arm_settings("cortex-m0")
    SET(USB_SUPPORT true)

    add_definitions(-D__ARM_LPC11UXX__)

    include_directories(
       ext/lpc11xx/cmsis/
    )

    add_subdirectory(src/xpcc/architecture/platform/cortex_m0/lpc/lpc11u)

    message(STATUS "Adding LPC11Uxx platform driver")


elseif(${PLATFORM} STREQUAL "lpc17xx")
    set_arm_settings("cortex-m3")
    SET(USB_SUPPORT true)

    add_definitions(-D__ARM_LPC17XX__)

    add_subdirectory(src/xpcc/architecture/platform/cortex_m3/lpc/lpc17)

    message(STATUS "Adding LPC17xx platform driver")

else()
    message(SEND_ERROR "Unknown PLATFORM")

endif()

IF(${USB_VENDOR_ID})
add_definitions(-DUSB_VENDOR_ID=${USB_VENDOR_ID})
message(STATUS "USB Vendor ID: ${USB_VENDOR_ID}")
endif()
IF(${USB_PRODUCT_ID})
add_definitions(-DUSB_PRODUCT_ID=${USB_PRODUCT_ID})
message(STATUS "USB Product ID: ${USB_PRODUCT_ID}")
endif()

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} CACHE INTERNAL "")
SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} CACHE INTERNAL "")
SET(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} CACHE INTERNAL "")

SET(XPCC_COMPILER_ARG1 "-mcpu=${CMAKE_SYSTEM_PROCESSOR} -mthumb" CACHE INTERNAL "")

#set cpu flags so compiler defines can be detected on arm-none-eabi
#SET(CMAKE_C_COMPILER_ARG1 -mcpu=${CMAKE_SYSTEM_PROCESSOR})
#SET(CMAKE_CXX_COMPILER_ARG1 -mcpu=${CMAKE_SYSTEM_PROCESSOR})

if (${CHIBI_RTOS})
     add_definitions(-DXPCC_RTOS_CHIBI)
endif()

project(xpcc_lib CXX C ASM)

#SET(CMAKE_C_COMPILER_ARG1 "")
#SET(CMAKE_CXX_COMPILER_ARG1 "")

#add fatfs to build
add_sources(
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/fatfs/ff.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/fatfs/option/unicode.c
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/fatfs
)

add_subdirectory(src/xpcc)

add_library(xpcc STATIC ${SOURCES})

add_subdirectory(ext)

get_property(incDirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
target_include_directories(xpcc PUBLIC ${incDirs} )

get_directory_property( CompDefs COMPILE_DEFINITIONS )
target_compile_definitions(xpcc PUBLIC ${CompDefs})

# Add all targets to the build-tree export set
export(TARGETS xpcc
  FILE "${PROJECT_BINARY_DIR}/xpccTargets.cmake")

configure_file(${PROJECT_SOURCE_DIR}/xpccConfig.cmake.in
    ${PROJECT_BINARY_DIR}/xpccConfig.cmake @ONLY)

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
#export(PACKAGE xpcc)

# vim: set et ft=cmake fenc=utf-8 ff=unix sts=4 sw=4 ts=4 :
