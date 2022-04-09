# Configuring cli.cxx

Everything is configurable within cli.cxx. See [traits.hxx](../src/cli/traits.hxx) for
default values.

You can directly modify traits;

```cpp
#include "cli/traits.hxx"

...

cli::config::enable_watch_methods = true;
```
