#project(VulkanEncapsulation)

#include_directories(
#    Private
#)

#aux_source_directory(Private SubPrivatePrivate)

#add_executable(
#    ${PROJECT_NAME}
#     main.cpp
#     ${SubPrivatePrivate}
#)

#target_link_libraries(
#    ${PROJECT_NAME}
#    ${Vulkan_LIBRARY}
#    glfw
#)

project(VulkanEncapsulation)

file(GLOB SRC_FILES
    "Private/*.cpp"
    "Pass/Pipeline/*.cpp"
    "Pass/*.cpp"
 )



add_executable(
    ${PROJECT_NAME}
    main.cpp
    ${SRC_FILES}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${Vulkan_LIBRARY}
    glfw
)