#include "cli/watch_method.hxx"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <sstream>

using namespace cli;

struct S
{
  detail::watch_method<S> f() { return detail::watch_method<S>{ "f()" }; }
};

TEST_CASE("watch_method<T> produces correct method name")
{
  S s;
  auto wm = s.f();
  std::stringstream ss;
  ss << wm;
  CHECK(ss.str() == "S::f()");
}
