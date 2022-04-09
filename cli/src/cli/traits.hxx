#pragma once

#ifdef DEBUG
#define CLI_DEBUG_FLAG true
#else
#define CLI_DEBUG_FLAG false
#endif

namespace cli::config {

namespace detail {

template<typename T>
class option
{
public:
  using type = option;
  using value_type = T;

  option() = delete;
  option(value_type value)
    : value_(value)
  {}

  operator value_type() const { return value_; }

private:
  value_type value_;
};

} // namespace cli::config::detail

/// Enable or disable watch_methods.
/// By default, watch_methods are only enabled for debug builds.
static detail::option<bool> enable_watch_methods{ CLI_DEBUG_FLAG };

} // namespace cli::config
