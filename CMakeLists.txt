cmake_minimum_required(VERSION 3.21)

project(cli-mono)

set(CLI_MONO_BUILD
    ON
    CACHE BOOL "" FORCE)

add_subdirectory(cli)
add_subdirectory(apps/hello)
