#pragma once

#include "cli/type_name.hxx"

#include <iostream>
#include <string>

namespace cli::detail {

template<typename T>
class watch_method
{
public:
  using type = watch_method;

  watch_method(std::string method_name)
    : method_name_(method_name)
  {
    std::cerr << type_name<T>() << "::" << method_name_ << ": ..." << std::endl;
  }

  ~watch_method()
  {
    std::cerr << type_name<T>() << "::" << method_name_ << ": done." << std::endl;
  }

  friend std::ostream& operator<<(std::ostream& os, const type& t)
  {
    return os << type_name<T>() << "::" << t.method_name_;
  }

private:
  std::string_view method_name_;
};

}
