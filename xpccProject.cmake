
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

enable_language(ASM)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

message(STATUS "${Cyan}Configuring XPCC${ColourReset}")
execute_process(COMMAND mkdir -p ${CMAKE_BINARY_DIR}/xpcc.build)

SET(xpcc_DIR ${CMAKE_BINARY_DIR}/xpcc.build)

if(${ENABLE_LTO})
    list(APPEND XPCC_CMAKE_DEFINES -DENABLE_LTO=TRUE)   
endif()

execute_process(
    COMMAND ${CMAKE_COMMAND} -DPLATFORM=${PLATFORM} ${XPCC_CMAKE_DEFINES} ${XPCC_DIR} 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/xpcc.build
)

message(STATUS "${Cyan}Finished Configuring XPCC${ColourReset}")

add_custom_target(xpcc_build
    COMMAND $(MAKE)
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/xpcc.build
)

add_custom_target(xpcc_clean
    COMMAND $(MAKE) clean
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/xpcc.build
)

find_package(xpcc REQUIRED)

include(${XPCC_PLATFORM_INCLUDE})

if(NOT LINKER_SCRIPT)
    if (NOT XPCC_LINKER_SCRIPT STREQUAL "")
        configure_file("${XPCC_LINKER_SCRIPT}" "linkscript.ld" @ONLY)
    endif()

    SET(LINKER_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/linkscript.ld")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_SYSTEM_NAME Generic)

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

SET(CMAKE_BUILD_TYPE MINSIZEREL CACHE INTERNAL "")

SET(CMAKE_C_COMPILER ${XPCC_C_COMPILER} CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER ${XPCC_CXX_COMPILER} CACHE INTERNAL "")
SET(CMAKE_ASM_COMPILER ${XPCC_ASM_COMPILER} CACHE INTERNAL "")
SET(CMAKE_AR ${XPCC_AR} CACHE INTERNAL "")
SET(CMAKE_RANLIB ${XPCC_RANLIB} CACHE INTERNAL "")


SET(CMAKE_C_FLAGS ${XPCC_C_FLAGS} CACHE INTERNAL "")
SET(CMAKE_CXX_FLAGS ${XPCC_CXX_FLAGS} CACHE INTERNAL "")
SET(CMAKE_EXE_LINKER_FLAGS "${XPCC_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}" CACHE INTERNAL "")

SET(ASM_OPTIONS "-x assembler-with-cpp")
SET(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}" CACHE INTERNAL "")

set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-I" CACHE STRING "")
set(CMAKE_INCLUDE_SYSTEM_FLAG_C "-I" CACHE STRING "")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "" CACHE INTERNAL "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "" CACHE INTERNAL "")

SET(CMAKE_C_COMPILER_ARG1 ${XPCC_COMPILER_ARG1}  CACHE STRING "")
SET(CMAKE_CXX_COMPILER_ARG1 ${XPCC_COMPILER_ARG1}  CACHE STRING "")


#project(${PROJECT_NAME} CXX C ASM)

#SET(CMAKE_C_COMPILER_ARG1 "")
#SET(CMAKE_CXX_COMPILER_ARG1 "")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} xpcc ${LIBRARIES})

add_dependencies(${PROJECT_NAME} xpcc_build)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${XPCC_OBJCOPY}
                        ARGS     -O ihex
                                ${PROJECT_NAME}
                                ${PROJECT_NAME}.hex
                        COMMENT "Generating HEX image ${PROJECT_NAME}.hex"
                        VERBATIM)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${XPCC_OBJCOPY}
                        ARGS     -O binary
                                ${PROJECT_NAME}
                                ${PROJECT_NAME}.bin
                        COMMENT "Generating BIN image ${PROJECT_NAME}.bin"
                        VERBATIM)

add_custom_target(dump_assembly
                        COMMAND arm-none-eabi-strip --strip-debug "${PROJECT_NAME}" -o "${PROJECT_NAME}_nodbg"
                        COMMAND ${XPCC_OBJDUMP} -C -D "${PROJECT_NAME}_nodbg" > "${PROJECT_NAME}.S"
                        COMMENT "Dumping Assembly listing"
                        VERBATIM)

add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
                        COMMAND ${XPCC_OBJSIZE}
                        ARGS    ${PROJECT_NAME}
                        COMMENT "Image Size"
                        VERBATIM)
