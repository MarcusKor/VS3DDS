# CMakeList.txt: mediator에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.8)

# 지원되는 경우 MSVC 컴파일러에 대해 핫 다시 로드 사용하도록 설정합니다.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_definitions (-DDEBUG)

project ("MediatorService")

include (TestBigEndian)

if (NOT WIN32)
    if (CMAKE_SIZEOF_VOID_P GREATER 4)
        message (STATUS "[ Setting ${CMAKE_CXX_COMPILE_OPTIONS_PIC} for machine ${CMAKE_HOST_SYSTEM_PROCESSOR} ]")
        set (CMAKE_POSITION_INDEPENDENT_CODE 1)
        set (ARCH_ADDRESS_BITS_X64 TRUE)
        message (STATUS "[ Architecture address 64 bits ]")
    else ()
        set (ARCH_ADDRESS_BITS_X64 FALSE)
        message (STATUS "[ Architecture address 32 bits ]")
    endif ()
else ()
    message (STATUS "[ CMAKE_SIZEOF_VOID_P=${CMAKE_SIZEOF_VOID_P} ]")
    if (CMAKE_SIZEOF_VOID_P GREATER 4)
        set (ARCH_ADDRESS_BITS_X64 TRUE)
        message (STATUS "[ Architecture address 64 bits ]")
    else ()
        set (ARCH_ADDRESS_BITS_X64 FALSE)
        message (STATUS "[ Architecture address 32 bits ]")
    endif ()
endif ()

if (WIN32 AND NOT CYGWIN AND NOT MSYS AND NOT CMAKE_CROSSCOMPILING)
    set (PACKAGE_FIND_STYLE "win32")
else ()
    set (PACKAGE_FIND_STYLE "unix")
endif ()

add_subdirectory ("src")