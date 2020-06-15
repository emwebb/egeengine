list(APPEND EGE_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/engine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/engine.hpp
    ${CMAKE_CURRENT_LIST_DIR}/game.cpp
    ${CMAKE_CURRENT_LIST_DIR}/game.hpp)
include(${CMAKE_CURRENT_LIST_DIR}/Vulkan/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/Platform/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/Utils/CMakeLists.txt)