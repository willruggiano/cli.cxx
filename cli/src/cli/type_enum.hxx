#pragma once

#include <cstddef>

namespace cli {

template<typename... Ts>
struct type_enum;

namespace detail {

template<int I, typename... Ts>
struct type_enum_traits;

template<int I>
struct type_enum_traits<I>
{
  static constexpr size_t size = I;
};

template<int I, typename T, typename... Ts>
struct type_enum_traits<I, T, Ts...> : type_enum_traits<I + 1, Ts...>
{};

template<int I, typename...>
struct index_of;

template<int I, typename T, typename... Us>
struct index_of<I, T, T, Us...>
{
  static constexpr size_t index = I;
};

template<int I, typename T, typename U, typename... Ts>
struct index_of<I, T, U, Ts...> : index_of<I + 1, T, Ts...>
{};

template<int I, typename T, typename... Us>
struct index_of<I, T, type_enum<Us...>> : index_of<I, T, Us...>
{};

}

template<typename... Ts>
struct type_enum_traits : detail::type_enum_traits<0, Ts...>
{};

template<typename... Ts>
struct type_enum
{
  static constexpr size_t size = type_enum_traits<Ts...>::size;
};

template<typename T, typename T_Enum>
constexpr size_t index_of = detail::index_of<0, T, T_Enum>::index;

}
