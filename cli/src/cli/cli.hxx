#pragma once

#include "cli/commands.hxx"
#include "cli/options.hxx"
#include "cli/parser.hxx"

#include <stdlib.h>

namespace cli {

#define CLI_MAIN(T)                                                                      \
  int main(int argc, char* argv[])                                                       \
  {                                                                                      \
    try {                                                                                \
      auto cmd = cli::parse<T>(argc, argv);                                              \
      (*cmd)();                                                                          \
      return EXIT_SUCCESS;                                                               \
    } catch (const std::exception& e) {                                                  \
      std::cerr << "e.what(): " << e.what() << std::endl;                                \
      return EXIT_FAILURE;                                                               \
    }                                                                                    \
  }

}
