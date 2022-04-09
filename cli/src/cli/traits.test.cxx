#include "cli/traits.hxx"

#include <doctest/doctest.h>

namespace cli::config {
static detail::option<int> n{ 42 };
}

TEST_CASE("Traits can be modified")
{
  using namespace cli;

  CHECK(config::n == 42);

  config::n = 64;
  CHECK(config::n == 64);

  config::n = 42;
  CHECK(config::n == 42);
}
