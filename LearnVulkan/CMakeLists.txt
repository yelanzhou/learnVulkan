cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(LearnVulkan)
cmake_policy(SET CMP0043 NEW)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_PREFIX_PATH "C:/Program Files (x86)/GLFW")


set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_definitions("-std=c++11")

include_directories(ThirdParty/include/)

find_package(Vulkan)
find_package(glfw3 REQUIRED)

if(Vulkan_FOUND)
    message("Vulkan found")
else()
    message(FATAL_ERROR "Cannot find vulkan")
endif()

if(glfw3_FOUND)
    message("glfw3 found")
else()
    message(FATAL_ERROR "can not found glfw3 ")
endif()

add_subdirectory(Run)
add_subdirectory(VulkanEncapsulation)



