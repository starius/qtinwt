# qtinwt, QtWebKit wrapped in Wt application
# Copyright (C) 2014-2015 Boris Nagaev
# See the LICENSE file for terms of use

find_package(Threads REQUIRED)

include(CheckCXXSourceCompiles)

include(FindBoost)
IF (MINGW)
    SET(B_THREAD "thread_win32")
    add_definitions(-DBOOST_THREAD_USE_LIB)
ELSE (MINGW)
    SET(B_THREAD "thread")
ENDIF (MINGW)
FIND_PACKAGE(Boost 1.42 COMPONENTS ${B_THREAD} signals REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR})
FIND_PACKAGE(Wt REQUIRED)
INCLUDE_DIRECTORIES(${Wt_INCLUDE_DIR})

FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtWebKit QtNetwork)
INCLUDE(${QT_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB headers "*.hpp")
file(GLOB sources "*.cpp")

ADD_DEFINITIONS(-DWT_NO_SLOT_MACROS)

QT4_WRAP_CPP(MOCSrcs Pages.hpp Page.hpp
    OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)

ADD_EXECUTABLE(qtinwt ${sources} ${MOCSrcs})
TARGET_LINK_LIBRARIES(qtinwt
    ${QT_LIBRARIES}
    ${Wt_HTTP_LIBRARY}
    ${Wt_LIBRARY}
    ${Boost_LIBRARIES}
)
IF (MINGW)
    target_link_libraries(qtinwt lzma)
    target_link_libraries(qtinwt jscore)
    target_link_libraries(qtinwt version)
    target_link_libraries(qtinwt shlwapi)
    target_link_libraries(qtinwt sqlite3)
    target_link_libraries(qtinwt crypto)
    target_link_libraries(qtinwt ssl)
    target_link_libraries(qtinwt ${QT_PLUGINS_DIR}/codecs/libqcncodecs.a)
    target_link_libraries(qtinwt ${QT_PLUGINS_DIR}/codecs/libqjpcodecs.a)
    target_link_libraries(qtinwt ${QT_PLUGINS_DIR}/codecs/libqkrcodecs.a)
    target_link_libraries(qtinwt ${QT_PLUGINS_DIR}/codecs/libqtwcodecs.a)
    target_link_libraries(qtinwt wsock32)
    target_link_libraries(qtinwt ws2_32)
    target_link_libraries(qtinwt GraphicsMagick)
    target_link_libraries(qtinwt winmm)
    target_link_libraries(qtinwt pthread)
    target_link_libraries(qtinwt xml2)
    target_link_libraries(qtinwt tiff)
    target_link_libraries(qtinwt freetype)
    target_link_libraries(qtinwt bz2)
    target_link_libraries(qtinwt iconv)
    target_link_libraries(qtinwt jasper)
    target_link_libraries(qtinwt jpeg)
    check_cxx_source_compiles("#include <lcms2.h>\n
    int main() { }" LCMS2_FOUND)
    IF (LCMS2_FOUND)
        target_link_libraries(qtinwt lcms2)
    ELSE (LCMS2_FOUND)
        target_link_libraries(qtinwt lcms)
    ENDIF (LCMS2_FOUND)
    SET(CMAKE_EXE_LINKER_FLAGS -mwindows)
ENDIF (MINGW)
