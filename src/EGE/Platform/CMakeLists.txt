list(APPEND EGE_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/platform.cpp
    ${CMAKE_CURRENT_LIST_DIR}/platform.hpp)
if(WIN32)
    include(${CMAKE_CURRENT_LIST_DIR}/windows/CMakeLists.txt)
endif()
if(LINUX)
    include(${CMAKE_CURRENT_LIST_DIR}/linux/CMakeLists.txt)
endif()