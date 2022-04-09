#pragma once

#include "cli/commands.hxx"

#include <algorithm>
#include <memory>
#include <vector>

namespace cli {

template<typename T>
std::unique_ptr<Command<T>>
parse(int argc, char* argv[])
{
#ifdef DEBUG
  std::copy(argv, argv + argc, std::ostream_iterator<char*>(std::cerr, "\n"));
#endif
  std::vector<std::string_view> args(argv + 1, argv + argc);
  auto cmd = std::make_unique<Command<T>>();
  return (args >> *cmd, std::move(cmd));
}

}
