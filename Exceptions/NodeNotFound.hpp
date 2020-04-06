#pragma once
#include <stdexcept>

namespace open62541Cpp::Exceptions {

class NodeNotFound :
    public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

}
