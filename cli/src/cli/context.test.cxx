#include "cli/context.hxx"

#include <doctest/doctest.h>

#include <string>
#include <string_view>
#include <vector>

using namespace cli;

struct BucketName
{
  using type = std::string;
  constexpr static auto name = "--bucket-name";
  constexpr static bool required = true;
};

class ls
{
public:
  using type = ls;
  using options = Options<BucketName>;
  using commands = type_enum<>;

  constexpr static auto name = "ls";

  ls(const options& parsed)
    : bucket_name_(parsed.at<BucketName>())
  {}

public:
  const std::string bucket_name_;
};

class s3
{
public:
  using type = s3;
  using options = Options<>;
  using commands = type_enum<ls>;

  constexpr static auto name = "s3";

  s3(const options&) {}
};

TEST_CASE("parsed_context<T> parses correctly")
{
  std::vector<std::string_view> args = { "s3", "ls", "--bucket-name", "s3-bucket" };
  parsed_context<s3> context;
  args >> context;

  CHECK(args.size() == 0);
  std::string bucket_name = context.at<ls>().options().at<BucketName>();
  CHECK(bucket_name == "s3-bucket");
}

TEST_CASE("parsed_context<T> yields the correct impl")
{
  std::vector<std::string_view> args = { "s3", "ls", "--bucket-name", "s3-bucket" };
  parsed_context<s3> context;
  args >> context;

  // FIXME: This won't work. We need to yield the impl dynamically!
  // We're going to need an abstraction in place of the raw T.
  // This guy will be the "operation". So we'll do context.command() -> op&
  // and then op.exec() to execute the command. An interesting question here will be:
  // How will we deal with parent options? e.g. if a sub-command needs to know that
  // --verbose (a parent flag) was passed
  // Perhaps the solution lies in a single interface which returns the parsed options for
  // the entire command hierarchy? e.g. context.options -> Options<...>&
  // Which is not *every possible* option, but just those that are "in tree".
  auto& impl = context.at<ls>().impl();
  CHECK(impl.bucket_name_ == "s3-bucket");
}
