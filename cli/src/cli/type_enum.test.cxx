#include "cli/type_enum.hxx"

#include <doctest/doctest.h>

#include <string>

using namespace cli;

using e0 = type_enum<>;
using e4 = type_enum<bool, int, float, std::string>;

TEST_CASE("size")
{
  CHECK(e4::size == 4);
}

TEST_CASE("size (empty)")
{
  CHECK(e0::size == 0);
}

TEST_CASE("index")
{
  CHECK(index_of<int, e4> == 1);
}
