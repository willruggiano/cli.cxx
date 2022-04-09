#include "cli/type_name.hxx"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

using namespace cli;

struct S0;

template<typename T>
struct S1;

TEST_CASE("type_name<T> produces correct typename")
{
  CHECK(detail::type_name<S0>() == "S0");
  CHECK(detail::type_name<S1<int>>() == "S1<int>");
  CHECK(detail::type_name<S1<S0>>() == "S1<S0>");
}
