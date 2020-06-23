#pragma once
#include <stdexcept>

namespace UmatiServerLib::Exceptions
{

  class NodeNotFound : public std::runtime_error
  {
  public:
    using std::runtime_error::runtime_error;
  };

} // namespace UmatiServerLib::Exceptions
