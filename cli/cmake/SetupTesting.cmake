enable_testing()

find_package(doctest REQUIRED)

macro(add_test_file FILENAME)
  get_filename_component(NAME ${FILENAME} NAME_WE)
  set(TESTNAME ${NAME}.test)

  add_executable(${TESTNAME} ${FILENAME})
  target_compile_definitions(
    ${TESTNAME} PRIVATE BUILD_TESTING DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)
  target_link_libraries(${TESTNAME} PRIVATE cli::cli doctest::doctest)

  add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
endmacro()
