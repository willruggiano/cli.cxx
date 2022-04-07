#include <cli/commands.hxx>
#include <cli/options.hxx>
#include <cli/parser.hxx>

#include <cstdlib>
#include <iostream>
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

  Hello() = default;
  Hello(std::string name)
    : name_(name)
  {}

  void operator()() { std::cout << "Hello, " << name_ << std::endl; }

private:
  std::string name_;
};

int
main(int argc, char* argv[])
{
  try {
    auto cmd = parse<Hello>(argc, argv);
    (*cmd)();
    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    std::cerr << "e.what(): " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
