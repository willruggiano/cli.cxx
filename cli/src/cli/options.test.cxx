#include "cli/options.hxx"

#include <doctest/doctest.h>

#include <iostream>
#include <string_view>
#include <vector>

using namespace cli;

struct Name
{
  using type = std::string;
  constexpr static auto name = "--name";
};

TEST_CASE("Parse Option<T> correctly when given single argument/value")
{
  std::vector<std::string_view> args = { "--name", "Gandalf" };

  Option<Name> option;
  args >> option;

  CHECK(option.value() == "Gandalf");
}

TEST_CASE("Parse Option<T> correctly when given multiple arguments/values")
{
  std::vector<std::string_view> args = { "--foo",   "bar",   "--name",
                                         "Gandalf", "--bar", "foo" };

  Option<Name> option;
  args >> option;

  CHECK(option.value() == "Gandalf");
}

TEST_CASE("Parse Options<T> correctly")
{
  std::vector<std::string_view> args = { "--name", "Gandalf" };

  Options<Name> options;
  args >> options;

  auto& option = options.at<Name>();
  CHECK(option.value() == "Gandalf");
}

struct Race
{
  using type = std::string;
  constexpr static auto name = "--race";
};

struct Age
{
  using type = int;
  constexpr static auto name = "--age";
};

TEST_CASE("Parse Options<Ts...> correctly")
{
  std::vector<std::string_view> args = { "--name", "Gandalf", "--race",
                                         "Wizard", "--age",   "42" };
  Options<Name, Race, Age> options;
  args >> options;

  auto& name = options.at<Name>();
  auto& race = options.at<Race>();
  auto& age = options.at<Age>();
  CHECK(name.value() == "Gandalf");
  CHECK(race.value() == "Wizard");
  CHECK(age.value() == 42);
  CHECK(args.size() == 0);
}

TEST_CASE("Parse Options<Ts...> when argument order does not match compiled order")
{
  std::vector<std::string_view> args = { "--name", "Gandalf", "--age",
                                         "42",     "--race",  "Wizard" };
  Options<Name, Race, Age> options;
  args >> options;

  auto& name = options.at<Name>();
  auto& race = options.at<Race>();
  auto& age = options.at<Age>();
  CHECK(name.value() == "Gandalf");
  CHECK(race.value() == "Wizard");
  CHECK(age.value() == 42);
}

struct Hometown
{
  using type = std::string;
  constexpr static auto name = "--hometown";
  constexpr static bool required = true;
};

TEST_CASE("Parse Option<T> throws exception when T is required")
{
  std::vector<std::string_view> args{};
  Options<Hometown> options;
  CHECK_THROWS_WITH(args >> options, "required argument '--hometown' missing");
}

TEST_CASE("Option<T> convertible to value_type")
{
  std::vector<std::string_view> args = { "--name", "Gandalf" };
  Option<Name> option;
  args >> option;

  const std::string& name = option;

  CHECK(name == "Gandalf");
}

TEST_CASE("Options<Ts...> convertible to Option<T> convertible to value_type")
{
  std::vector<std::string_view> args = { "--name", "Gandalf", "--age",
                                         "42",     "--race",  "Wizard" };
  Options<Name, Race, Age> options;
  args >> options;

  std::string name = options.at<Name>();
  std::string race = options.at<Race>();
  int age = options.at<Age>();

  CHECK(name == "Gandalf");
  CHECK(race == "Wizard");
  CHECK(age == 42);

  const auto& options_const = options;
  std::string name_const = options.at<Name>();
  std::string race_const = options.at<Race>();
  int age_const = options.at<Age>();

  CHECK(name_const == "Gandalf");
  CHECK(race_const == "Wizard");
  CHECK(age_const == 42);
}
