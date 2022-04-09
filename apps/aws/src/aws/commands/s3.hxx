#pragma once

#include <cli/commands.hxx>
#include <cli/options.hxx>

namespace aws {

class S3
{
public:
  using type = S3;
  using options = cli::Options<>;
  using commands = cli::Commands<>;

  constexpr static auto name = "s3";

  S3() = default;

  void operator()() { std::cout << "Welcome the s3 subcommand!" << std::endl; }
};

}
