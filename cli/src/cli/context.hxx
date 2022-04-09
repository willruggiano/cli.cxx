#pragma once

#include "cli/options.hxx"
#include "cli/type_enum.hxx"

#include <cassert>
#include <memory>
#include <string_view>
#include <vector>

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

// We can think of argument parsing as producing a "command hierarchy".
// We can model these command hierarchies using types! Because we know all of the possible
// combinations ahead of time via the type-enums. Given a command T, we can create a
// hierarchy for each subcommand S in T.
template<typename...>
class parsed_context;

template<typename T>
class parsed_context<T>
{
public:
  using type = parsed_context;
  using args_type = std::vector<std::string_view>;
  using options_type = typename T::options;

  friend args_type& operator>>(args_type& args, type& t)
  {
    // Parse the arguments specific to T
    args >> t.options_;
    if (args.empty()) {
      t.impl_ = std::make_unique<T>(t.options_);
    }
    return args;
  }

  const auto& options() const { return options_; }
  const auto& impl() const { return *impl_; }

private:
  options_type options_;
  std::unique_ptr<T> impl_;
};

template<typename T, typename S, typename... Ss>
class parsed_context<T, S, Ss...>
  // This is the hierarchy specific to S
  : public parsed_context<S, typename S::commands>
  // This is the continuation of the hierarchy for T
  , public parsed_context<T, type_enum<Ss...>>
{
public:
  using type = parsed_context;
  using this_hierarchy = parsed_context<S, typename S::commands>;
  using that_hierarchy = parsed_context<T, type_enum<Ss...>>;
  using args_type = std::vector<std::string_view>;

  friend args_type& operator>>(args_type& args, type& t)
  {
    assert(args.begin() != args.end());
    if (args.front() == this_hierarchy::name) {
      return args >> static_cast<this_hierarchy&>(t);
    } else {
      return args >> static_cast<that_hierarchy&>(t);
    }
  }
};

template<typename T, typename... Ss>
class parsed_context<T, type_enum<Ss...>> : public parsed_context<T, Ss...>
{
public:
  using type = parsed_context;
  using this_type = parsed_context<T>;
  using parent_type = parsed_context<T, Ss...>;
  using args_type = std::vector<std::string_view>;

  constexpr static auto name = T::name;

  template<typename S>
  parsed_context<S>& at()
  {
    return static_cast<parsed_context<S>&>(*this);
  }

  template<typename S>
  const parsed_context<S>& at() const
  {
    return static_cast<parsed_context<S>&>(*this);
  }

  friend args_type& operator>>(args_type& args, type& t)
  {
    detail::watch_method<type> wm("operator>>(...)");
    assert(args.front() == name);
    args.erase(args.begin());
    if (args >> static_cast<this_type&>(t); !args.empty()) {
      return args >> static_cast<parent_type&>(t);
    }
    return args;
  }
};

} // namespace cli::detail

template<typename T>
using parsed_context = detail::parsed_context<T, typename T::commands>;

} // namespace cli
