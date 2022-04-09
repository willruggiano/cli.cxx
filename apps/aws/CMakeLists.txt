cmake_minimum_required(VERSION 3.21)

project(aws)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CLI_MONO_BUILD)
  find_package(Cli REQUIRED)
endif()

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cxx)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_include_directories(
  ${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_link_libraries(${PROJECT_NAME} PRIVATE cli::cli)

install(TARGETS ${PROJECT_NAME})
