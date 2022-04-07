#pragma once

#include "cli/type_enum.hxx"

#include <stdexcept>
#include <type_traits>
#include <utility>

namespace cli {

template<typename...>
class type_array;

namespace detail {

template<int I, typename...>
class type_array;

template<int I>
class type_array<I, type_enum<>>
{
public:
  using type = type_array;

  // int& at(size_t i) { throw std::runtime_error("illegal argument"); }
};

template<int I, typename T, typename... Ts, typename U, typename... Us>
class type_array<I, type_enum<T, Ts...>, U, Us...>
  : public type_array<I + 1, type_enum<Ts...>, Us...>
{
public:
  using type = type_array;
  using parent_type = type_array<I + 1, type_enum<Ts...>, Us...>;

  // auto& at(size_t i)
  // {
  //   if (i == I) {
  //     return value_;
  //   } else {
  //     return parent_type::at(i);
  //   }
  // }

private:
  U value_;
};

}

template<typename... Ts, typename... Us>
class type_array<type_enum<Ts...>, Us...>
  : private detail::type_array<0, type_enum<Ts...>, Us...>
{
public:
  using type = type_array;
  using parent_type = detail::type_array<0, type_enum<Ts...>, Us...>;

  type_array(Us&&... us);

  // auto& at(size_t i) { return parent_type::at(i); }
};

}
