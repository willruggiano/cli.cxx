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
  static std::unique_ptr<T> method() { return std::make_unique<T>(); }

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
    // Parse options; NOTE: that this operation MODIFIES the args vector!
    using options_type = typename T::options;
    auto options = std::make_unique<options_type>();
    args >> *options;
    // Here is where we actually construct the command/subcommand.
    // TODO: How do we want to handle subcommands?
    // What we have is a type-enum identifying all of the possible subcommands.
    // Perhaps what we want is more like a "parsed context"?
    // But we don't really need that do we? We just need to create the "impl", which is
    // the "command to execute", which could be a subcommand.
    // 1. Parse the options for this command =: T
    // 2. Check for a subcommand and, if present, recurse into Command<S>
    // Then we need to decide on how to convey parent arguments, if we even need to do so
    // (which we probably do want to do).
    // For this, I think the cleanest approach would be to pass the parent command (e.g.
    // T) to the subcommand S, so the constructor for S would be required to look
    // something like: S(T&, ...) where T& is a reference to S's parent command, T.
    // We have to go parametric here then, meaning impl_ needs to be abstract.
    // We could say that impl_ =: std::unique_ptr<Command<>> but then we have an infinite
    // loop in the case where we just want to execute T.
    // We need another layer of abstraction, call it: App. And then impl becomes a
    // linked-list? Because we need to keep the parents around? Or maybe not an actual
    // linked-list but just something that keeps the parents around; some sort of context.
    // Haha full circle; we're back to the "parsed context".
    auto impl = detail::make_impl<T>(*options);
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
  std::unique_ptr<T> impl_;
};

template<typename... Ts>
using Commands = type_enum<Ts...>;

}
