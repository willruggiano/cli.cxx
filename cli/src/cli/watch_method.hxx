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
#ifdef DEBUG
    std::cerr << type_name<T>() << "::" << method_name_ << ": ..." << std::endl;
#endif
  }

  ~watch_method()
  {
#ifdef DEBUG
    std::cerr << type_name<T>() << "::" << method_name_ << ": done." << std::endl;
#endif
  }

  friend std::ostream& operator<<(std::ostream& os, const type& t)
  {
    return os << type_name<T>() << "::" << t.method_name_;
  }

private:
  std::string method_name_;
};

}
