#include <cli/cli.hxx>

#include <iostream>
#include <string>

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
  using options = cli::Options<Name>;

  Hello() = default;
  Hello(std::string name)
    : name_(name)
  {}

  void operator()() { std::cout << "Hello, " << name_ << std::endl; }

private:
  std::string name_;
};

CLI_MAIN(Hello)
