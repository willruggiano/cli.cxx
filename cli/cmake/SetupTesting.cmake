enable_testing()

find_package(doctest REQUIRED)

file(GLOB TESTS src/cli/*.test.cxx)

foreach(TEST ${TESTS})
  get_filename_component(NAME ${TEST} NAME_WE)
  set(TESTNAME ${NAME}.test)

  add_executable(${TESTNAME} ${TEST})
  target_compile_definitions(${TESTNAME} PRIVATE BUILD_TESTING)
  target_link_libraries(${TESTNAME} PRIVATE cli::cli doctest::doctest)

  add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
endforeach()
