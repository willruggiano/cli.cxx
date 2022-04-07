#include "cli/commands.hxx"
#include "cli/options.hxx"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace cli;

struct Name
{
  using type = std::string;
  constexpr static auto name = "--name";
  constexpr static bool required = true;
};

class Hello
{
public:
  using type = Hello;
  using options = Options<Name>;

  Hello(std::string name)
    : name(name)
  {}

public: // For testing.
  const std::string name;
};

TEST_CASE("Parse Command<T> correctly")
{
  std::vector<std::string_view> args = { "--name", "Gandalf" };
  Command<Hello> cmd;
  args >> cmd;

  CHECK(cmd.impl().name == "Gandalf");
}
