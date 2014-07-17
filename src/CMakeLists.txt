find_package(Threads REQUIRED)

include(FindBoost)
FIND_PACKAGE(Boost 1.42
    COMPONENTS thread program_options filesystem system
        date_time signals
    REQUIRED)
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
    ${Boost_LIBRARIES}
    ${QT_LIBRARIES}
    ${Wt_LIBRARY} ${Wt_HTTP_LIBRARY})

