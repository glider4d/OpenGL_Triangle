cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(PROJECT_NAME OPENGL_TRIANGLE)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
add_executable(${PROJECT_NAME} src/main.cpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BIULD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

#add glfw library
add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

#add glad library
add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

#add stb_image
target_include_directories(${PROJECT_NAME} PUBLIC external/stb_image/) #public to access the master project


#for output dirrectory and copy res

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)