#pragma once

#include "cli/value_type.hxx"
#include "cli/watch_method.hxx"

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>

namespace cli {
namespace detail {

template<typename T>
struct is_required : std::true_type
{};

template<typename T>
struct is_required<std::optional<T>> : std::false_type
{};

template<typename T>
constexpr bool is_required_v = is_required<typename T::type>::value;

template<typename T>
concept required = is_required_v<T>;

} // namespace detail

template<typename>
class Option;

/**
 * @brief A list of types.
 */
template<typename... Ts>
class Options : private Option<Ts>...
{
public:
  using type = Options;

  template<typename T>
  Option<T>& at()
  {
    return *this;
  }

  template<typename T>
  const Option<T>& at() const&
  {
    return *this;
  }

  friend std::vector<std::string_view>& operator>>(std::vector<std::string_view>& args,
                                                   type& t)
  {
    detail::watch_method<type> wm{ "operator>>(...)" };
    return ((args >> static_cast<Option<Ts>&>(t)), ..., args);
  }

  friend std::ostream& operator<<(std::ostream& os, const type& t)
  {
    return ((os << static_cast<const Option<Ts>&>(t)), ..., os);
  }
};

template<typename T>
class Option
{
public:
  using type = Option;
  using option_type = T;
  using value_type = typename option_type::type;

  const value_type& value() const { return value_; }

  operator const value_type&() const { return value_; }

  friend std::vector<std::string_view>& operator>>(std::vector<std::string_view>& args,
                                                   type& t)
  {
    detail::watch_method<type> wm{ "operator>>(...)" };

    for (auto it = args.begin(); it != args.end(); ++it) {
      // TODO: We'll need to specialize this for flags.
      if (*it == option_type::name) {
        std::stringstream ss;
        if (it + 1 != args.end()) {
          ss << *(it + 1), ss >> t.value_;
        } else if (detail::is_required_v<T>) {
          throw std::runtime_error(std::string{ "required argument '" } + T::name +
                                   std::string{ "' missing value" });
        }
        args.erase(it, it + 2); // N.B. [first, last)
        return args;
      }
    }

    if (detail::is_required_v<T>) {
      throw std::runtime_error(std::string{ "required argument '" } + T::name +
                               std::string{ "' missing" });
    }

    return args;
  }

  friend std::ostream& operator<<(std::ostream& os, const type& t)
  {
    return os << option_type::name << " " << t.value_;
  }

private:
  value_type value_{};
};

} // namespace cli
