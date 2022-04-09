#include <cli/cli.hxx>

#include "aws/commands/s3.hxx"

using namespace aws;

class Aws
{
public:
  using type = Aws;
  using options = cli::Options<>;
  using commands = cli::Commands<S3>;

  Aws() = default;

  void operator()() { std::cout << "Welcome to the awscli!" << std::endl; }
};

CLI_MAIN(Aws)
