#pragma once

#include <optional>
#include <Open62541Cpp/UA_RelativPathElement.hpp>
#include "BindableMember.hpp"

template <typename T>
class BindableMemberValue : public BindableMember<T>
{
public:
  BindableMemberValue() = default;
  explicit BindableMemberValue(const T &val);
  UA_StatusCode StatusCode = UA_STATUSCODE_GOOD;
  std::optional<UA_DateTime> SourceTimestamp;
  std::optional<UA_DateTime> ServerTimestamp;

  BindableMemberValue<T> &operator=(const T &other)
  {
    this->value = other;
    return *this;
  }
};

template <typename T>
BindableMemberValue<T>::BindableMemberValue(const T &val) : BindableMember<T>(val)
{
}
