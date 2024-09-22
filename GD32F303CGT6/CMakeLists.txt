cmake_minimum_required(VERSION 3.28)
set(RTOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../RTOS_V4)
set(CMAKE_TOOLCHAIN_FILE ${RTOS_DIR}/CMake/ARMCC/toolchain.cmake)

project(GD32F303CGT6 C)
enable_language(ASM)

set(CMAKE_SYSTEM_PROCESSOR Cortex-M4.fp.sp)
set(CMAKE_C_STANDARD 11)
set(FW_CPU_TYPE GD32F30X_XD)
set(RTOS_CPU_MODEL ARM-Cortex-M4)
########################################################################################################################
## FIRMWARE
add_executable(${PROJECT_NAME} "")

########################################################################################################################
## DEFINITIONS
add_definitions(-DUSE_STDPERIPH_DRIVER)
add_definitions(-D${FW_CPU_TYPE})
add_definitions(-D__MICROLIB)
add_definitions(-DHXTAL_VALUE=8000000)
add_definitions(-DNDEBUG)
set(RTOS_SDK_ENABLE_RTOS ON)

target_compile_definitions(${PROJECT_NAME} PUBLIC GD32F30X_XD)
target_compile_definitions(${PROJECT_NAME} PUBLIC USE_STDPERIPH_DRIVER)
target_compile_definitions(${PROJECT_NAME} PUBLIC __MICROLIB)
target_compile_definitions(${PROJECT_NAME} PUBLIC HXTAL_VALUE=8000000)
target_compile_definitions(${PROJECT_NAME} PUBLIC NDEBUG)

########################################################################################################################
## COMPILER CONFIGURATION
if (CMAKE_C_COMPILER_ID MATCHES ARMCC)
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} --c99 --gnu -c --cpu ${CMAKE_SYSTEM_PROCESSOR} -g -W -O0 --apcs=interwork --split_sections")
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} --cpu ${CMAKE_SYSTEM_PROCESSOR} -g --apcs=interwork --pd \"__MICROLIB SETA 1\" --pd \"__UVISION_VERSION SETA 537\" --pd \"${FW_CPU_TYPE} SETA 1\" --xref --diag_warning=optimizations")
    target_link_options(${PROJECT_NAME} PUBLIC --cpu ${CMAKE_SYSTEM_PROCESSOR}
            --scatter ${CMAKE_CURRENT_SOURCE_DIR}/ARMCC/link.sct
            --library_type=microlib --strict --summary_stderr
            --info summarysizes --map --xref --callgraph --symbols
    )
endif ()

########################################################################################################################
## RTOS
add_subdirectory(${RTOS_DIR} ${CMAKE_CURRENT_BINARY_DIR}/RTOS_V4)
target_link_libraries(${PROJECT_NAME} PUBLIC RTOS_COMPILER)
target_link_libraries(${PROJECT_NAME} PUBLIC RTOS_CPU)
target_link_libraries(${PROJECT_NAME} PUBLIC RTOS_KERNEL)
target_link_libraries(${PROJECT_NAME} PUBLIC RTOS_SDK)

########################################################################################################################
## FIRMWARE
set(FW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Firmware/GD32F30x_Firmware_Library_V2.2.0/Firmware)
file(GLOB FW_CMSIS_CORE_SOURCES ${FW_DIR}/CMSIS/GD/GD32F30x/Source/*.c)
file(GLOB FW_STD_PERIPH_DRIVER_SOURCES ${FW_DIR}/GD32F30x_standard_peripheral/Source/*.c)

foreach (item IN LISTS FW_CMSIS_DEVICE_SOURCES)
    message(STATUS "[${PROJECT_NAME}] FW_CMSIS_DEVICE_SOURCES: ${item}")
endforeach ()
foreach (item IN LISTS FW_CMSIS_CORE_SOURCES)
    message(STATUS "[${PROJECT_NAME}] FW_CMSIS_CORE_SOURCES: ${item}")
endforeach ()
foreach (item IN LISTS FW_STD_PERIPH_DRIVER_SOURCES)
    message(STATUS "[${PROJECT_NAME}] FW_STD_PERIPH_DRIVER_SOURCES: ${item}")
endforeach ()


target_sources(${PROJECT_NAME} PUBLIC ${FW_CMSIS_CORE_SOURCES})
#target_sources(${PROJECT_NAME} PUBLIC ${FW_CMSIS_DEVICE_SOURCES})
target_sources(${PROJECT_NAME} PUBLIC ${FW_STD_PERIPH_DRIVER_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${FW_DIR}/CMSIS/)
target_include_directories(${PROJECT_NAME} PUBLIC ${FW_DIR}/CMSIS/GD/GD32F30x/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${FW_DIR}/GD32F30x_standard_peripheral/Include)

########################################################################################################################
## STARTUP SOURCE
if (CMAKE_C_COMPILER_ID MATCHES ARMCC)
    set(FW_CMSIS_DEVICE_STARTUP_SOURCE ${FW_DIR}/CMSIS/GD/GD32F30x/Source/ARM/startup_gd32f30x_xd.s)
    target_sources(${PROJECT_NAME} PUBLIC ${FW_CMSIS_DEVICE_STARTUP_SOURCE})
    foreach (item IN LISTS FW_CMSIS_DEVICE_STARTUP_SOURCE)
        message(STATUS "[${PROJECT_NAME}] FW_CMSIS_DEVICE_STARTUP_SOURCE: ${item}")
    endforeach ()
endif ()

#if (CMAKE_C_COMPILER_ID MATCHES GNU)
#    set(FW_CMSIS_DEVICE_STARTUP_SOURCE ${FW_DIR}/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md.s)
#    target_sources(${PROJECT_NAME} PUBLIC ${FW_CMSIS_DEVICE_STARTUP_SOURCE})
#    foreach (item IN LISTS FW_CMSIS_DEVICE_STARTUP_SOURCE)
#        message(STATUS "[${PROJECT_NAME}] FW_CMSIS_DEVICE_STARTUP_SOURCE: ${item}")
#    endforeach ()
#endif ()

########################################################################################################################
## BOARD
file(GLOB BOARD_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Board/*.c)
target_sources(${PROJECT_NAME} PUBLIC ${BOARD_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Board)

foreach (item IN LISTS BOARD_SOURCES)
    message(STATUS "[${PROJECT_NAME}] BOARD_SOURCES: ${item}")
endforeach ()
########################################################################################################################
## APPLICATION
file(GLOB APPLICATION_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Application/*.c)
target_sources(${PROJECT_NAME} PUBLIC ${APPLICATION_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Application)

foreach (item IN LISTS APPLICATION_SOURCES)
    message(STATUS "[${PROJECT_NAME}] APPLICATION_SOURCES: ${item}")
endforeach ()



########################################################################################################################
## POST BUILD
if (CMAKE_C_COMPILER_ID MATCHES ARMCC)
    set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
    set(ASM_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.asm)
    set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} --i32 $<TARGET_FILE:${PROJECT_NAME}> --output=${HEX_FILE}
            COMMAND ${CMAKE_OBJCOPY} -c --cpu=${CMAKE_SYSTEM_PROCESSOR} $<TARGET_FILE:${PROJECT_NAME}> --output=${ASM_FILE}
            COMMAND ${CMAKE_OBJCOPY} --bin $<TARGET_FILE:${PROJECT_NAME}> --output=${BIN_FILE}
            COMMAND ${CMAKE_OBJCOPY} -z $<TARGET_FILE:${PROJECT_NAME}>
            COMMENT "Building ${HEX_FILE}
                    Building ${ASM_FILE}
                    Building ${BIN_FILE}")
endif ()

