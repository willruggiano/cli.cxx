#pragma once

#include "cli/options.hxx"
#include "cli/type_enum.hxx"
#include "cli/watch_method.hxx"

#include <cassert>
#include <memory>

namespace cli {
namespace detail {

template<typename T, typename...>
struct make_impl_;

template<typename T>
struct make_impl_<T>
{
  static std::unique_ptr<T> method() { throw std::runtime_error("impossible condition"); }

  template<typename... As>
  static std::unique_ptr<T> method(As&&... as)
  {
    return std::make_unique<T>(std::forward<As>(as)...);
  }
};

template<typename T, typename V, typename... Vs>
struct make_impl_<T, V, Vs...> : make_impl_<T, Vs...>
{
  template<typename A, typename... As>
  static std::unique_ptr<T> method(A&& a, As&&... as)
  {
    using parent_type = make_impl_<T, Vs...>;
    return parent_type::method(std::forward<A>(a), std::forward<As>(as)...);
  }
};

template<typename T, typename... Vs>
std::unique_ptr<T>
make_impl(Options<Vs...>& parsed)
{
  // TODO: Should these be std::forwarded?
  return make_impl_<T, Vs...>::method(parsed.template at<Vs>().value()...);
}

}

template<typename T>
class Command
{
public:
  using type = Command;
  using options_type = typename T::options;

  void operator()()
  {
    detail::watch_method<type> wm{ "operator()()" };
    assert(impl_);
    (*impl_)();
  }

  friend std::vector<std::string_view>& operator>>(std::vector<std::string_view>& args,
                                                   type& t)
  {
    detail::watch_method<type> wm{ "operator>>(...)" };
    t.options_ = std::make_unique<options_type>();
    args >> *(t.options_);
    auto impl = detail::make_impl<T>(*(t.options_));
    t.impl_ = std::move(impl);
    return args;
  }

#ifdef BUILD_TESTING
  T& impl()
  {
    assert(impl_);
    return *impl_;
  }
#endif

private:
  std::unique_ptr<options_type> options_;
  std::unique_ptr<T> impl_;
};

template<typename... Ts>
using Commands = type_enum<Ts...>;

}
