#pragma once

namespace cli {

namespace detail {

template<int I, typename T>
class type_index_element
{
public:
  using type = type_index_element;
};

template<int I, typename T, typename... Ts>
class type_index : private type_index_element<I, T>
{
public:
  using type = type_index;
  using parent_type = type_index<I + 1, Ts...>;
};

}

template<typename... Ts>
class type_index : public detail::type_index<0, Ts...>
{
public:
  using type = type_index;
};

}
