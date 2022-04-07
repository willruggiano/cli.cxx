#include "cli/parser.hxx"

#include <vector>

namespace cli {

std::vector<std::string_view>
make_arguments_from_argv(int argc, char* argv[])
{
  return std::vector<std::string_view>(argv + 1, argv + argc);
}

}
