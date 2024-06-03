# CMakeList.txt: Mediator에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#

if (MSVC)
    add_compile_options(/W4)
else()
    # add_compile_options(-Wall -Wextra -Wpedantic -Wno-error=address-of-packed-member)
    add_compile_options(-Waddress-of-packed-member)
endif()

add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")
add_definitions("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")

if ("${CURRENT_PACKAGES_DIR}" STREQUAL "")
    if (WIN32)
        message (STATUS "[ Find vcpkg installation information ]")
        if ("${VCPKG_INSTALLED_DIR}" STREQUAL "")
            if ("$ENV{VCPKG_ROOT}" STREQUAL "")
                set (vs_vcpkg_path "C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/vcpkg/installed")
                set (respository_vcpkg_path "C:/Repository/vcpkg/installed")
                if (EXISTS vs_vcpkg_path)
                    set (VCPKG_INSTALLED_DIR vs_vcpkg_path) 
                elseif (EXISTS respository_vcpkg_path)
                    set (VCPKG_INSTALLED_DIR respository_vcpkg_path) 
                endif ()
            else ()
                message (STATUS "[ VCPKG_ROOT=$ENV{VCPKG_ROOT} ]")
                set (VCPKG_INSTALLED_DIR $ENV{VCPKG_ROOT}/installed)     
            endif ()
        endif ()
        if ("${VCPKG_INSTALLED_DIR}" STREQUAL "")
            message (STATUS "[ Please, install vcpkg to manage c++ packages. The default location of VCPKG_ROOT is ${vc_vcpkg_path} ]")    
        else ()
            if ("${VCPKG_DEFAULT_HOST_TRIPLET}" STREQUAL "")
                if (ARCH_ADDRESS_BITS_X64)
                    set (VCPKG_DEFAULT_HOST_TRIPLET "x64-windows")
                else ()
                    set (VCPKG_DEFAULT_HOST_TRIPLET "x86-windows")
                endif ()
            endif ()
            set (CURRENT_PACKAGES_DIR ${VCPKG_INSTALLED_DIR}/${VCPKG_DEFAULT_HOST_TRIPLET})
            set (PACKAGE_DIR_EXISTS TRUE)
            set (VCPKG_TOOLCHAIN TRUE)
        endif ()
    else (LINUX)
        set (LINUX_INCLUDE_DIR "/usr/include")
        if (ARCH_ADDRESS_BITS_X64)
            set (LINUX_LIBRARY_DIR "/usr/lib64")
        else ()
            set (LINUX_LIBRARY_DIR "/usr/lib")
        endif ()
        file (READ "/etc/issue" ETC_ISSUE)
        string (REGEX MATCH "Ubuntu|Red Hat|Debian" DIST ${ETC_ISSUE})    
        if (DIST STREQUAL "Debian")        
            message (STATUS "[ Found Debian ]")
        elseif (DIST STREQUAL "Ubuntu")
            message (STATUS "[ Found Ubuntu ]")   
            if (ARCH_ADDRESS_BITS_X64)
                set (LINUX_LIBRARY_DIR "/usr/lib/x86_64-linux-gnu")    
            else ()
                set (LINUX_LIBRARY_DIR "/usr/lib/i386-linux-gnu")    
            endif ()
        else ()
            file (READ "/etc/system-release" ETC_ISSUE)
            string (REGEX MATCH "Ubuntu|Red Hat|Debian" DIST ${ETC_ISSUE})
            if (DIST STREQUAL "Red Hat")
                message (STATUS "[ Found Red Hat ]")
            endif ()
        endif ()
        message (STATUS "[ LINUX_INCLUDE_DIR=${LINUX_INCLUDE_DIR} ]")    
        message (STATUS "[ LINUX_LIBRARY_DIR=${LINUX_LIBRARY_DIR} ]")    
        set (PACKAGE_DIR_EXISTS TRUE)
    endif ()
endif ()

if (${PACKAGE_DIR_EXISTS})
    set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
    set (PROJECT_LIBRARY_DIR_DEBUG ${PROJECT_SOURCE_DIR}/lib/debug)
    set (PROJECT_LIBRARY_DIR_RELEASE ${PROJECT_SOURCE_DIR}/lib/release)
    set (PROJECT_BINARY_DIR_DEBUG ${PROJECT_BINARY_DIR}/debug)
    set (PROJECT_BINARY_DIR_RELEASE ${PROJECT_BINARY_DIR}/release)
    message (STATUS "[ PROJECT_INCLUDE_DIR=${PROJECT_INCLUDE_DIR} ]")    
    message (STATUS "[ PROJECT_LIBRARY_DIR_DEBUG=${PROJECT_LIBRARY_DIR_DEBUG} ]")    
    message (STATUS "[ PROJECT_LIBRARY_DIR_RELEASE=${PROJECT_LIBRARY_DIR_RELEASE} ]")    
    message (STATUS "[ PROJECT_BINARY_DIR_DEBUG=${PROJECT_BINARY_DIR_DEBUG} ]")    
    message (STATUS "[ PROJECT_BINARY_DIR_RELEASE=${PROJECT_BINARY_DIR_RELEASE} ]")    
    message (STATUS "[ CURRENT_PACKAGES_DIR=${CURRENT_PACKAGES_DIR} ]")  

    include_directories (${PROJECT_INCLUDE_DIR})
    if (WIN32)
        set (BOOST_ROOT ${CURRENT_PACKAGES_DIR})
        set (BOOST_INCLUDEDIR ${CURRENT_PACKAGES_DIR}/include)
        set (BOOST_LIBRARYDIR ${CURRENT_PACKAGES_DIR}/lib)
        set (BOOST_BINARY_DIR ${CURRENT_PACKAGES_DIR}/bin)
    else ()
        set (BOOST_INCLUDEDIR ${LINUX_INCLUDE_DIR})
        set (BOOST_LIBRARYDIR ${LINUX_LIBRARY_DIR})
    endif ()
    set (Boost_USE_STATIC_LIBS OFF) 
    set (Boost_USE_MULTITHREADED ON)  
    set (Boost_USE_STATIC_RUNTIME OFF) 
    set (CMAKE_VERBOSE_MAKEFILE ON)    
    set (Boost_NO_WARN_NEW_VERSIONS 1)
    message (STATUS "[ Find boost package installation information ]")    
    find_package (Boost REQUIRED COMPONENTS chrono atomic thread)
    if (${BOOST_FOUND})
        message (STATUS "[ Found boost libraries (version=${Boost_LIB_VERSION}) ]")    
        message (STATUS "[ Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}]")    
        message (STATUS "[ Boost_LIBRARY_DIRS=${Boost_LIBRARY_DIRS}]")
        message (STATUS "[ Boost_LIBRARIES=${Boost_LIBRARIES}]")
        message (STATUS "[ Boost_LIBRARY_DIR_RELEASE=${Boost_LIBRARY_DIR_RELEASE}]")
        message (STATUS "[ Boost_LIBRARY_DIR_DEBUG=${Boost_LIBRARY_DIR_DEBUG}]")
        message (STATUS "[ BOOST_BINARY_DIR=${BOOST_BINARY_DIR}]")
        include_directories (${Boost_INCLUDE_DIRS})
    endif ()
    message (STATUS "[ Find wxWidgets package installation information ]") 
    if (WIN32)
        set (wxWidgets_ROOT_DIR ${CURRENT_PACKAGES_DIR})
        set (wxWidgets_COMMON_LIBRARIES png tiff jpeg zlib regex expat)
        if (NOT wxWidgets_FIND_COMPONENTS)
            if (wxWidgets_USE_MONOLITHIC)
                set (wxWidgets_FIND_COMPONENTS mono)
            else ()
                set (wxWidgets_FIND_COMPONENTS core base)
            endif ()
        endif ()
        if (NOT wxWidgets_EXCLUDE_COMMON_LIBRARIES)
            list (APPEND wxWidgets_FIND_COMPONENTS ${wxWidgets_COMMON_LIBRARIES})
        endif ()
        macro (WX_GET_NAME_COMPONENTS _CONFIGURATION _PF _UNV _UCD _DBG)
            message (STATUS "[ CONFIGURATION=${_CONFIGURATION} ]")
            string (REGEX MATCH "univ" ${_UNV} "${_CONFIGURATION}")
            string (REGEX REPLACE "[msw|qt].*(u)[d]*$" "u" ${_UCD} "${_CONFIGURATION}")
            if (${_UCD} STREQUAL ${_CONFIGURATION})
                set (${_UCD} "")
            endif ()
            string (REGEX MATCH "d$" ${_DBG} "${_CONFIGURATION}")
            string (REGEX MATCH "^[msw|qt]*" ${_PF} "${_CONFIGURATION}")
        endmacro ()
        macro (WX_FIND_LIBS _PF _UNV _UCD _DBG)
            message (STATUS "[ m_unv = ${_UNV} ]")
            message (STATUS "[ m_ucd = ${_UCD} ]")
            message (STATUS "[ m_dbg = ${_DBG} ]")
            foreach (LIB ${wxWidgets_COMMON_LIBRARIES} scintilla)
                find_library (WX_${LIB}${_DBG}
                    NAMES
                        wx${LIB}${_UCD}${_DBG}
                        wx${LIB}${_DBG}
                    PATHS
                        ${WX_LIB_DIR}
                        NO_DEFAULT_PATH)
                mark_as_advanced (WX_${LIB}${_DBG})
            endforeach ()
            find_library (WX_base${_DBG}
                NAMES
                    wxbase32${_UCD}${_DBG}
                    wxbase31${_UCD}${_DBG}
                    wxbase30${_UCD}${_DBG}
                    wxbase29${_UCD}${_DBG}
                    wxbase28${_UCD}${_DBG}
                    wxbase27${_UCD}${_DBG}
                    wxbase26${_UCD}${_DBG}
                    wxbase25${_UCD}${_DBG}
                PATHS
                    ${WX_LIB_DIR}
                    NO_DEFAULT_PATH)
            mark_as_advanced (WX_base${_DBG})
            foreach (LIB net odbc xml)
                find_library (WX_${LIB}${_DBG}
                    NAMES
                        wxbase32${_UCD}${_DBG}_${LIB}
                        wxbase31${_UCD}${_DBG}_${LIB}
                        wxbase30${_UCD}${_DBG}_${LIB}
                        wxbase29${_UCD}${_DBG}_${LIB}
                        wxbase28${_UCD}${_DBG}_${LIB}
                        wxbase27${_UCD}${_DBG}_${LIB}
                        wxbase26${_UCD}${_DBG}_${LIB}
                        wxbase25${_UCD}${_DBG}_${LIB}
                    PATHS
                        ${WX_LIB_DIR}
                        NO_DEFAULT_PATH)
                mark_as_advanced (WX_${LIB}${_DBG})
            endforeach ()
            find_library (WX_mono${_DBG}
                NAMES
                    wx${_PF}${_UNV}32${_UCD}${_DBG}
                    wx${_PF}${_UNV}31${_UCD}${_DBG}
                    wx${_PF}${_UNV}30${_UCD}${_DBG}
                    wx${_PF}${_UNV}29${_UCD}${_DBG}
                    wx${_PF}${_UNV}28${_UCD}${_DBG}
                    wx${_PF}${_UNV}27${_UCD}${_DBG}
                    wx${_PF}${_UNV}26${_UCD}${_DBG}
                    wx${_PF}${_UNV}25${_UCD}${_DBG}
                PATHS ${WX_LIB_DIR}
                    NO_DEFAULT_PATH)
            mark_as_advanced (WX_mono${_DBG})
            foreach (LIB core adv aui html media xrc dbgrid gl qa richtext stc ribbon propgrid webview)
                find_library (WX_${LIB}${_DBG}
                    NAMES
                        wx${_PF}${_UNV}32${_UCD}${_DBG}_${LIB}
                        wx${_PF}${_UNV}31${_UCD}${_DBG}_${LIB}
                        wx${_PF}${_UNV}30${_UCD}${_DBG}_${LIB}
                        wx${_PF}${_UNV}29${_UCD}${_DBG}_${LIB}
                        wx${_PF}${_UNV}28${_UCD}${_DBG}_${LIB}
                        wx${_PF}${_UNV}27${_UCD}${_DBG}_${LIB}
                        wx${_PF}${_UNV}26${_UCD}${_DBG}_${LIB}
                        wx${_PF}${_UNV}25${_UCD}${_DBG}_${LIB}
                    PATHS ${WX_LIB_DIR}
                        NO_DEFAULT_PATH)
                mark_as_advanced (WX_${LIB}${_DBG})
            endforeach ()
        endmacro ()
        macro (WX_CLEAR_LIB _LIB)
            set (${_LIB} "${_LIB}-NOTFOUND" CACHE FILEPATH "Cleared." FORCE)
            set (${_LIB}_FOUND FALSE)
            mark_as_advanced (${_LIB})
        endmacro ()
        macro (WX_CLEAR_ALL_LIBS _DBG)
            foreach (LIB ${wxWidgets_COMMON_LIBRARIES} scintilla)
                WX_CLEAR_LIB (WX_${LIB}${_DBG})
            endforeach ()
            WX_CLEAR_LIB (WX_base${_DBG})
            foreach (LIB net odbc xml)
                WX_CLEAR_LIB (WX_${LIB}${_DBG})
            endforeach ()
            WX_CLEAR_LIB (WX_mono${_DBG})
            foreach (LIB core adv aui html media xrc dbgrid gl qa richtext webview stc ribbon propgrid)
                WX_CLEAR_LIB (WX_${LIB}${_DBG})
            endforeach ()
        endmacro ()
        macro (WX_CLEAR_ALL_DBG_LIBS)
            WX_CLEAR_ALL_LIBS ("d")
        endmacro ()
        macro (WX_CLEAR_ALL_REL_LIBS)
            WX_CLEAR_ALL_LIBS ("")
        endmacro ()
        macro (WX_SET_LIBRARIES _LIBS _DBG)
            message (STATUS "[ Looking for ${${_LIBS}} ]")
            if (WX_USE_REL_AND_DBG)
                foreach (LIB ${${_LIBS}})
                    message (STATUS "[ Searching for ${LIB} and ${LIB}d ]")
                    message (STATUS "[ WX_${LIB}  : ${WX_${LIB}} ]")
                    message (STATUS "[ WX_${LIB}d : ${WX_${LIB}d} ]")
                    if (WX_${LIB} AND WX_${LIB}d)
                        message (STATUS "[ Found ${LIB} and ${LIB}d ]")
                        list (APPEND wxWidgets_LIBRARIES debug ${WX_${LIB}d} optimized ${WX_${LIB}})
                        set (wxWidgets_${LIB}_FOUND TRUE)
                    elseif (NOT wxWidgets_FIND_REQUIRED_${LIB})
                        message (STATUS "[ Ignored optional missing WX_${LIB}=${WX_${LIB}} or WX_${LIB}d=${WX_${LIB}d} ]")
                    else ()
                        message (STATUS "[ Not found due to missing WX_${LIB}=${WX_${LIB}} or WX_${LIB}d=${WX_${LIB}d} ]")
                        set (wxWidgets_FOUND FALSE)
                    endif ()
                endforeach ()
            else ()
                foreach (LIB ${${_LIBS}})
                    message (STATUS "[ Searching for ${LIB}${_DBG} ]")
                    message (STATUS "[ WX_${LIB}${_DBG} : ${WX_${LIB}${_DBG}} ]")
                    if (WX_${LIB}${_DBG})
                        message (STATUS "[ Found ${LIB}${_DBG} ]")
                        list (APPEND wxWidgets_LIBRARIES ${WX_${LIB}${_DBG}})
                        set (wxWidgets_${LIB}_FOUND TRUE)
                    elseif (NOT wxWidgets_FIND_REQUIRED_${LIB})
                        message (STATUS "[ Ignored optional missing WX_${LIB}${_DBG}=${WX_${LIB}${_DBG}} ]")
                    else ()
                        message (STATUS "[ Not found due to missing WX_${LIB}${_DBG}=${WX_${LIB}${_DBG}} ]")
                        set (wxWidgets_FOUND FALSE)
                    endif ()
                endforeach ()
            endif()
            message (STATUS "[ WX_USE_GL : Check usage ]")
            list (FIND ${_LIBS} gl WX_USE_GL)
            if (NOT WX_USE_GL EQUAL -1)
                message (STATUS "[ Added opengl libraries. ]")
                list (APPEND wxWidgets_LIBRARIES opengl32 glu32)
            else ()
                message (STATUS "[ Not required. ]")
            endif ()
            list (APPEND wxWidgets_LIBRARIES winmm comctl32 uuid oleacc uxtheme rpcrt4 shlwapi version wsock32)
        endmacro ()
        if (VCPKG_TOOLCHAIN)
            set (wxWidgets_FOUND TRUE)
            find_path (wxWidgets_ROOT_DIR
                NAMES 
                    include/wx/wx.h
                PATHS
                    ENV wxWidgets_ROOT_DIR
                    DOC "wxWidgets base/installation directory")
            if (CMAKE_BUILD_TYPE STREQUAL "Debug")
                set (wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/debug/lib)
                set (wxWidgets_CONFIGURATION mswud)
            else ()
                set (wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib)
                set (wxWidgets_CONFIGURATION mswu)
            endif ()
            set (wxWidgets_BIN_DIR ${wxWidgets_ROOT_DIR}/bin)
            set (wxWidgets_INCLUDE_DIR ${wxWidgets_ROOT_DIR}/include)
            file (GLOB DLL ${wxWidgets_BIN_DIR}/wxmsw*.dll)
            if (DLL)
                set (wxWidgets_DEFINITIONS WXUSINGDLL)
                message (STATUS "[ Detected SHARED/DLL tree wxWidgets_LIB_DIR=${wxWidgets_LIB_DIR} ]")
            endif ()
            set (WX_ROOT_DIR ${wxWidgets_ROOT_DIR})
            set (WX_LIB_DIR ${wxWidgets_LIB_DIR})
            set (WX_CONFIGURATION_LIST ${wxWidgets_CONFIGURATION})
            if (EXISTS ${wxWidgets_INCLUDE_DIR}/wx/setup.h)
                set (wxWidgets_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIR})
            else ()
                message (STATUS "[ wxWidgets_FOUND FALSE because ${wxWidgets_INCLUDE_DIR}/wx/setup.h does not exists. ]")
                set (wxWidgets_FOUND FALSE)
            endif ()
            if (EXISTS ${wxWidgets_ROOT_DIR}/include/wx/wx.h)
                list (APPEND wxWidgets_INCLUDE_DIRS ${wxWidgets_ROOT_DIR}/include)
            else ()
                message (STATUS "[ wxWidgets_FOUND FALSE because wxWidgets_ROOT_DIR=${wxWidgets_ROOT_DIR} has no ${wxWidgets_ROOT_DIR}/include/wx/wx.h ]")
                set (wxWidgets_FOUND FALSE)
            endif ()
            message (STATUS "[ WX_CONFIGURATION_LIST=${WX_CONFIGURATION_LIST} ]")
            WX_GET_NAME_COMPONENTS (${wxWidgets_CONFIGURATION} PF UNV UCD DBG)
            WX_FIND_LIBS ("${PF}" "${UNV}" "${UCD}" "${DBG}")
            if (WX_USE_REL_AND_DBG)
                WX_FIND_LIBS ("${PF}" "${UNV}" "${UCD}" "d")
            endif ()
            WX_SET_LIBRARIES (wxWidgets_FIND_COMPONENTS "${DBG}")
            if ("${UCD}" STREQUAL "u")
                list (APPEND wxWidgets_DEFINITIONS UNICODE _UNICODE)
            endif ()
            set (wxWidgets_DEFINITIONS_DEBUG _DEBUG __WXDEBUG__)
        else ()
            find_path (wxWidgets_ROOT_DIR
                NAMES
                    include/wx/wx.h
                PATHS
                    ENV wxWidgets_ROOT_DIR
                    ENV WXWIN
                    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\wxWidgets_is1;Inno Setup: App Path]"  # WX 2.6.x
                    C:/
                    D:/
                    ENV ProgramFiles
                PATH_SUFFIXES
                    wxWidgets-3.2.0
                    wxWidgets-3.1.0
                    wxWidgets-3.0.2
                    wxWidgets-3.0.1
                    wxWidgets-3.0.0
                    wxWidgets-2.9.5
                    wxWidgets-2.9.4
                    wxWidgets-2.9.3
                    wxWidgets-2.9.2
                    wxWidgets-2.9.1
                    wxWidgets-2.9.0
                    wxWidgets-2.8.9
                    wxWidgets-2.8.8
                    wxWidgets-2.8.7
                    wxWidgets-2.8.6
                    wxWidgets-2.8.5
                    wxWidgets-2.8.4
                    wxWidgets-2.8.3
                    wxWidgets-2.8.2
                    wxWidgets-2.8.1
                    wxWidgets-2.8.0
                    wxWidgets-2.7.4
                    wxWidgets-2.7.3
                    wxWidgets-2.7.2
                    wxWidgets-2.7.1
                    wxWidgets-2.7.0
                    wxWidgets-2.7.0-1
                    wxWidgets-2.6.4
                    wxWidgets-2.6.3
                    wxWidgets-2.6.2
                    wxWidgets-2.6.1
                    wxWidgets-2.5.4
                    wxWidgets-2.5.3
                    wxWidgets-2.5.2
                    wxWidgets-2.5.1
                    wxWidgets
                    DOC "wxWidgets base/installation directory")
                if (NOT WX_ROOT_DIR STREQUAL wxWidgets_ROOT_DIR)
                    set (WX_ROOT_DIR ${wxWidgets_ROOT_DIR} CACHE INTERNAL "wxWidgets_ROOT_DIR")
                    set (wxWidgets_LIB_DIR "wxWidgets_LIB_DIR-NOTFOUND" CACHE PATH "Cleared." FORCE)
                endif ()
                if (WX_ROOT_DIR)
                    set (_WX_TOOL "")
                    set (_WX_TOOLVER "")
                    set (_WX_ARCH "")
                    if (MINGW)
                        set (_WX_TOOL gcc)
                    elseif (MSVC)
                        set (_WX_TOOL vc)
                        set (_WX_TOOLVER ${MSVC_TOOLSET_VERSION})
                        string (REGEX REPLACE ".$" "x" _WX_TOOLVERx ${_WX_TOOLVER})
                        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
                            set (_WX_ARCH _x64)
                        endif ()
                    endif ()
                if (BUILD_SHARED_LIBS)
                    find_path (wxWidgets_LIB_DIR
                        NAMES
                            qtu/wx/setup.h
                            qtud/wx/setup.h
                            msw/wx/setup.h
                            mswd/wx/setup.h
                            mswu/wx/setup.h
                            mswud/wx/setup.h
                            mswuniv/wx/setup.h
                            mswunivd/wx/setup.h
                            mswunivu/wx/setup.h
                            mswunivud/wx/setup.h
                        PATHS
                            ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVER}_xp${_WX_ARCH}_dll
                            ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVER}${_WX_ARCH}_dll
                            ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVERx}_xp${_WX_ARCH}_dll
                            ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVERx}${_WX_ARCH}_dll
                            ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_ARCH}_dll
                            ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVER}_xp${_WX_ARCH}_lib
                            ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVER}${_WX_ARCH}_lib
                            ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVERx}_xp${_WX_ARCH}_lib
                            ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVERx}${_WX_ARCH}_lib
                            ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_ARCH}_lib
                            DOC "Path to wxWidgets libraries"
                            NO_DEFAULT_PATH)
                    else ()
                        find_path (wxWidgets_LIB_DIR
                            NAMES
                                qtu/wx/setup.h
                                qtud/wx/setup.h
                                msw/wx/setup.h
                                mswd/wx/setup.h
                                mswu/wx/setup.h
                                mswud/wx/setup.h
                                mswuniv/wx/setup.h
                                mswunivd/wx/setup.h
                                mswunivu/wx/setup.h
                                mswunivud/wx/setup.h
                            PATHS
                                ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVER}_xp${_WX_ARCH}_lib
                                ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVER}${_WX_ARCH}_lib
                                ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVERx}_xp${_WX_ARCH}_lib
                                ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVERx}${_WX_ARCH}_lib
                                ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_ARCH}_lib
                                ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVER}_xp${_WX_ARCH}_dll
                                ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVER}${_WX_ARCH}_dll
                                ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVERx}_xp${_WX_ARCH}_dll
                                ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_TOOLVERx}${_WX_ARCH}_dll
                                ${WX_ROOT_DIR}/lib/${_WX_TOOL}${_WX_ARCH}_dll
                                DOC "Path to wxWidgets libraries"
                                NO_DEFAULT_PATH)
                        endif ()
                    unset (_WX_TOOL)
                    unset (_WX_TOOLVER)
                    unset (_WX_ARCH)
                    if (NOT WX_LIB_DIR STREQUAL wxWidgets_LIB_DIR)
                        set (WX_LIB_DIR ${wxWidgets_LIB_DIR} CACHE INTERNAL "wxWidgets_LIB_DIR")
                        WX_CLEAR_ALL_DBG_LIBS()
                        WX_CLEAR_ALL_REL_LIBS()
                    endif ()

                if (WX_LIB_DIR)
                    if (WX_LIB_DIR MATCHES "[dD][lL][lL]")
                        set (wxWidgets_DEFINITIONS WXUSINGDLL)
                        message (STATUS "[ Detected SHARED/DLL tree WX_LIB_DIR=${WX_LIB_DIR} ]")
                    endif ()
                    foreach (CFG mswunivud mswunivd mswud mswd mswunivu mswuniv mswu msw qt qtd qtu qtud)
                        set (WX_${CFG}_FOUND FALSE)
                        if (EXISTS ${WX_LIB_DIR}/${CFG})
                            list (APPEND WX_CONFIGURATION_LIST ${CFG})
                            set (WX_${CFG}_FOUND TRUE)
                            set (WX_CONFIGURATION ${CFG})
                        endif ()
                    endforeach ()
                    message (STATUS "[ WX_CONFIGURATION_LIST=${WX_CONFIGURATION_LIST} ]")
                    if (WX_CONFIGURATION)
                        set (wxWidgets_FOUND TRUE)
                        if (NOT WX_${wxWidgets_CONFIGURATION}_FOUND)
                            set (wxWidgets_CONFIGURATION ${WX_CONFIGURATION} CACHE STRING "Set wxWidgets configuration (${WX_CONFIGURATION_LIST})" FORCE)
                        else ()
                            set (wxWidgets_CONFIGURATION ${wxWidgets_CONFIGURATION} CACHE STRING "Set wxWidgets configuration (${WX_CONFIGURATION_LIST})" FORCE)
                        endif ()
                        if (WX_${wxWidgets_CONFIGURATION}d_FOUND)
                            option (wxWidgets_USE_REL_AND_DBG "Use release and debug configurations?" TRUE)
                            set (WX_USE_REL_AND_DBG ${wxWidgets_USE_REL_AND_DBG})
                        else ()
                            if (wxWidgets_USE_REL_AND_DBG)
                                set(wxWidgets_USE_REL_AND_DBG FALSE CACHE BOOL "No ${wxWidgets_CONFIGURATION}d found." FORCE)
                            endif ()
                            set (WX_USE_REL_AND_DBG FALSE)
                        endif ()
                        WX_GET_NAME_COMPONENTS (${wxWidgets_CONFIGURATION} PF UNV UCD DBG)
                        if (EXISTS ${WX_LIB_DIR}/${wxWidgets_CONFIGURATION}/wx/setup.h)
                            set (wxWidgets_INCLUDE_DIRS ${WX_LIB_DIR}/${wxWidgets_CONFIGURATION})
                        else ()
                            message (STATUS "[ wxWidgets_FOUND FALSE because ${WX_LIB_DIR}/${wxWidgets_CONFIGURATION}/wx/setup.h does not exists. ]")
                            set (wxWidgets_FOUND FALSE)
                        endif ()
                        if (EXISTS ${WX_ROOT_DIR}/include/wx/wx.h)
                            list (APPEND wxWidgets_INCLUDE_DIRS ${WX_ROOT_DIR}/include)
                        else ()
                            message (STATUS "[ wxWidgets_FOUND FALSE because WX_ROOT_DIR=${WX_ROOT_DIR} has no ${WX_ROOT_DIR}/include/wx/wx.h ]")
                            set (wxWidgets_FOUND FALSE)
                        endif ()
                        WX_FIND_LIBS ("${PF}" "${UNV}" "${UCD}" "${DBG}")
                        if (WX_USE_REL_AND_DBG)
                            WX_FIND_LIBS ("${PF}" "${UNV}" "${UCD}" "d")
                        endif ()
                        WX_SET_LIBRARIES (wxWidgets_FIND_COMPONENTS "${DBG}")
                        if ("${UCD}" STREQUAL "u")
                            list (APPEND wxWidgets_DEFINITIONS UNICODE _UNICODE)
                        endif ()
                        set (wxWidgets_DEFINITIONS_DEBUG _DEBUG __WXDEBUG__)
                    endif()
                endif()
            endif()
        endif()

        find_package (wxWidgets)
    else ()
        set (wxWidgets_CONFIG_EXECUTABLE /usr/bin/wx-config)
        find_package (wxWidgets REQUIRED COMPONENTS core base adv aui propgrid richtext OPTIONAL_COMPONENTS net)
    endif ()    
    if (${wxWidgets_FOUND})
        add_definitions (-DUSE_WXWIDGETS)
        if (WIN32)
            set (WIN32_EXECUTABLE ON)
            set (CMAKE_WIN32_EXECUTABLE ${WIN32_EXECUTABLE})
            add_definitions (-DWXUSINGDLL -D_CRT_SECURE_NO_WARNINGS)
        endif ()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${wxWidgets_CXX_FLAGS}")
        message (STATUS "[ CMAKE_WIN32_EXECUTABLE=${CMAKE_WIN32_EXECUTABLE} ]")
        message (STATUS "[ Found wxWidgets libraries (${wxWidgets_DEFINITIONS}) ]") 
        message (STATUS "[ Found boost libraries (version=${Boost_LIB_VERSION}) ]")    
        message (STATUS "[ wxWidgets_ROOT_DIR=${wxWidgets_ROOT_DIR}]")    
        message (STATUS "[ wxWidgets_LIB_DIR=${wxWidgets_LIB_DIR}]")
        message (STATUS "[ wxWidgets_EXCLUDE_COMMON_LIBRARIES=${wxWidgets_EXCLUDE_COMMON_LIBRARIES}]")
        message (STATUS "[ wxWidgets_CONFIG_OPTIONS=${wxWidgets_CONFIG_OPTIONS}]")
        message (STATUS "[ wxWidgets_INCLUDE_DIRS=${wxWidgets_INCLUDE_DIRS}]")
        message (STATUS "[ wxWidgets_LIBRARY_DIRS=${wxWidgets_LIBRARY_DIRS}]")
        message (STATUS "[ wxWidgets_LIBRARIES=${wxWidgets_LIBRARIES}]")
        message (STATUS "[ wxWidgets_DEFINITIONS=${wxWidgets_DEFINITIONS}]")
        message (STATUS "[ wxWidgets_DEFINITIONS_DEBUG=${wxWidgets_DEFINITIONS_DEBUG}]")
        message (STATUS "[ wxWidgets_CXX_FLAGS=${wxWidgets_CXX_FLAGS}]")
        message (STATUS "[ wxWidgets_USE_FILE=${wxWidgets_USE_FILE}]")
        include (${wxWidgets_USE_FILE})
    endif ()
    if (WIN32)
        set (CURSES_INCLUDE_PATH ${CURRENT_PACKAGES_DIR}/include)
        set (CURSES_INCLUDE_DIR ${CURRENT_PACKAGES_DIR}/include)
        set (CURSES_LIBRARY ${CURRENT_PACKAGES_DIR}/lib)
        set (CURSES_BINARY_DIR ${CURRENT_PACKAGES_DIR}/bin)
    endif ()
    message (STATUS "[ Find curses package installation information ]") 
    find_package (Curses REQUIRED)
    if (${CURSES_FOUND})
        message (STATUS "[ Found curses library (${CURSES_HAVE_CURSES_H}) ]")
        message (STATUS "[ CURSES_INCLUDE_DIRS=${CURSES_INCLUDE_DIRS} ]")
        message (STATUS "[ CURSES_LIBRARY=${CURSES_LIBRARY} ]")
        message (STATUS "[ CURSES_BINARY_DIR=${CURSES_BINARY_DIR}]")
        if ("${CURSES_INCLUDE_DIRS}" STREQUAL "${Boost_INCLUDE_DIRS}")
            message (STATUS "[ Already attached. ]")
        else ()
            include_directories (${CURSES_INCLUDE_DIRS})
        endif ()
        if (WIN32)
            set (CURSES_LIBRARIES ${CURSES_LIBRARY}/pdcurses.lib)
            add_definitions (-DPDC_DLL_BUILD -DPDC_WIDE)
            message (STATUS "[ CURSES_LIBRARIES=${CURSES_LIBRARIES} ]")
        endif ()
    endif ()
    if (WIN32)
    else (LINUX)
        message (STATUS "[ Find ixxat packages installation information (${LINUX_LIBRARY_DIR}) ]")    
        if (EXISTS "${LINUX_LIBRARY_DIR}/libeci113DriverLinux-usb-1.0.so")
            message (STATUS "[ Found ixxat usb to can v2 driver ]")
            set (IXXATUSBTOCANV2_FOUND TRUE)
        endif ()
        message (STATUS "[ Set ixxat environment variables ]")
        if (IXXATUSBTOCANV2_FOUND)
            set (IXXATUSBTOCANV2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
            message (STATUS "[ IXXATUSBTOCANV2_INCLUDE_DIR=${IXXATUSBTOCANV2_INCLUDE_DIR} ]")
            include_directories (${IXXATUSBTOCANV2_INCLUDE_DIR})
            set (IXXATUSBTOCANV2_LIBRARY_DIR ${LINUX_LIBRARY_DIR})    
            find_library (IXXATUSBTOCANV2_LIBRARIES libeci113DriverLinux-usb-1.0.so ${IXXATUSBTOCANV2_LIBRARY_DIR})
            message (STATUS "[ IXXATUSBTOCANV2_LIBRARIES=${IXXATUSBTOCANV2_LIBRARIES} ]")
        endif ()
    endif ()                                                                                                                     
    if (WIN32)
    else (LINUX)
        message (STATUS "[ Find advantech packages installation information (${LINUX_LIBRARY_DIR}) ]")    
        if (EXISTS "${LINUX_LIBRARY_DIR}/libbiodaq.so")
            message (STATUS "[ Found advantech usb-4711a ]")
            set (ADVANTECHUSB4711A_FOUND TRUE)
        endif ()
        message (STATUS "[ Set adventech usb-4711a environment variables ]")
        if (ADVANTECHUSB4711A_FOUND)
            set (ADVANTECHUSB4711A_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
            message (STATUS "[ ADVANTECHUSB4711A_INCLUDE_DIR=${ADVANTECHUSB4711A_INCLUDE_DIR} ]")
            include_directories (${ADVANTECHUSB4711A_INCLUDE_DIR})
            set (ADVANTECHUSB4711A_LIBRARY_DIR ${LINUX_LIBRARY_DIR})    
            find_library (ADVANTECHUSB4711A_LIBRARIES libbiodaq.so libbiodaqutil.so libbio4711a.so ${ADVANTECHUSB4711A_LIBRARY_DIR})
            message (STATUS "[ ADVANTECHUSB4711A_LIBRARIES=${ADVANTECHUSB4711A_LIBRARIES} ]")
        endif ()
    endif ()
    if (WIN32)
    else (LINUX)
        message (STATUS "[ Find modbus packages installation information (${LINUX_LIBRARY_DIR}) ]")    
        if (EXISTS "${LINUX_LIBRARY_DIR}/libmodbus.so")
            message (STATUS "[ Found modbus library ]")
            set (LIBMODBUS_FOUND TRUE)
        endif ()
        message (STATUS "[ Set libmodbus environment variables ]")
        if (LIBMODBUS_FOUND)
            set (LIBMODBUS_FOUNDINCLUDE_DIR ${LINUX_INCLUDE_DIR}/modbus)
            message (STATUS "[ LIBMODBUS_FOUNDINCLUDE_DIR=${LIBMODBUS_FOUNDINCLUDE_DIR} ]")
            include_directories (${LIBMODBUS_FOUNDINCLUDE_DIR})
            set (LIBMODBUS_LIBRARY_DIR ${LINUX_LIBRARY_DIR})    
            find_library (LIBMODBUS_LIBRARIES libmodbus.so ${LIBMODBUS_LIBRARY_DIR})
            message (STATUS "[ LIBMODBUS_LIBRARIES=${LIBMODBUS_LIBRARIES} ]")
        endif ()
    endif ()
endif ()

if (("${CMAKE_WIN32_EXECUTABLE}" STREQUAL "ON") AND WIN32)
    message (STATUS "[ Create wxWidgets application on windows. ]")
    if (MSVC)
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
    endif ()
    add_executable (Mediator WIN32
       "Mediator.cpp")
    target_compile_definitions (Mediator PRIVATE
        wxUSE_RC_MANIFEST
        wxUSE_DPI_AWARE_MANIFEST=2)
    set_target_properties (Mediator PROPERTIES WIN32_EXECUTABLE YES)
else ()
    add_executable (Mediator
       "Mediator.cpp")
endif ()

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property (TARGET Mediator PROPERTY CXX_STANDARD 20)
    set_target_properties (Mediator
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

if (${BOOST_FOUND})
    if ("${Boost_LIBRARIES}" STREQUAL "")
        message (STATUS "[ Boost attached only header files. ]") 
    else ()
        message (STATUS "[ Link boost libraries. ]")        
        target_link_libraries (Mediator PUBLIC ${Boost_LIBRARIES})
        if (WIN32)
            file (GLOB DEPENDENCY_FILES "${CURRENT_PACKAGES_DIR}/boost*.dll")
            if (DEFINED DEPENDENCY_FILES)
                foreach (file ${DEPENDENCY_FILES})
                    add_custom_command (TARGET Mediator POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${file} ${CMAKE_BINARY_DIR}/bin
                        COMMENT "Copied specified dependency file: ${file}")
                endforeach (file)
            endif ()
        endif ()
    endif ()
endif ()

if (${LIBMODBUS_FOUND})
    if ("${LIBMODBUS_LIBRARIES}" STREQUAL "")
        message (STATUS "[ Modbus library is not exist. ]") 
    else ()
        message (STATUS "[ Link modbus library. ]")        
        target_link_libraries (Mediator PUBLIC ${LIBMODBUS_LIBRARIES})
        if (WIN32)
            message (STATUS "[ Have to install and check. ]")                    
        endif ()
    endif ()    
endif ()
