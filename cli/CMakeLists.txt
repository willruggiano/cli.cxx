cmake_minimum_required(VERSION 3.21)

project(
  cli
  VERSION 0.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(GNUInstallDirs)

add_library(${PROJECT_NAME} STATIC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/cli/parser.cxx)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

add_library(cli::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} EXPORT CliTargets)
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/cli
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "*.hxx")
install(
  EXPORT CliTargets
  NAMESPACE cli::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

include(CMakePackageConfigHelpers)

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CliConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/CliConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  PATH_VARS INCLUDE_INSTALL_DIR
  INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/CliConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CliConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/CliConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

include(SetupTesting)
add_test_file(${CMAKE_CURRENT_SOURCE_DIR}/src/cli/commands.test.cxx)
add_test_file(${CMAKE_CURRENT_SOURCE_DIR}/src/cli/context.test.cxx)
add_test_file(${CMAKE_CURRENT_SOURCE_DIR}/src/cli/options.test.cxx)
add_test_file(${CMAKE_CURRENT_SOURCE_DIR}/src/cli/traits.test.cxx)
add_test_file(${CMAKE_CURRENT_SOURCE_DIR}/src/cli/type_enum.test.cxx)
add_test_file(${CMAKE_CURRENT_SOURCE_DIR}/src/cli/type_name.test.cxx)
add_test_file(${CMAKE_CURRENT_SOURCE_DIR}/src/cli/watch_method.test.cxx)
