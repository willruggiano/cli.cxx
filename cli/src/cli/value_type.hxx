#pragma once

#include <iostream>
#include <vector>

namespace cli {

/**
 * @brief Extracts a list of values from an input stream.
 *
 * @tparam T std::vector<T>::value_type
 * @param is a character input stream
 * @param v the list of values to be extracted
 * @return is
 */
template<typename T>
std::istream&
operator>>(std::istream& is, std::vector<T>& v)
{
  if (is) {
    while (true) {
      // TODO: We need some way to stop this. What would it be? \s or \n? Both?
      if (auto c = is.peek(); c == ',') {
        is.ignore(1, ',');
      } else if (T t; is >> t) {
        v.push_back(t);
      } else {
        return (is.setstate(std::ios_base::failbit), is);
      }
    }
  } else {
    return is;
  }
}

}
