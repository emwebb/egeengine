cmake_minimum_required(VERSION 3.16)

project(EthansGameEngine VERSION 1.0)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)  
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(glfw3 REQUIRED FATAL_ERROR)
include(src/CMakeLists.txt)

add_library (egeengine STATIC ${EGE_SOURCES})
target_include_directories (egeengine 
    PUBLIC ${VULKAN_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIR}
)
if(WIN32)
    target_compile_definitions(egeengine PRIVATE VK_USE_PLATFORM_WIN32_KHR)
endif()
if(LINUX)
    target_compile_definitions(egeengine PRIVATE VK_USE_PLATFORM_XLIB_KHR)
endif()
add_executable(example ${Example_SOURCES})
target_include_directories (example 
    PUBLIC ${VULKAN_INCLUDE_DIRS}
)
target_link_libraries(example egeengine)
target_link_libraries(example Vulkan::Vulkan)
target_link_libraries(example glfw)
add_definitions(-DLOG_LEVEL=5)