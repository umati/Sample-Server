#pragma once

#include "ValueDecorator.hpp"
#include <optional>
#include <Open62541Cpp/UA_RelativPathElement.hpp>

template <typename T>
class BindableMember : public ValueDecorator<T>
{
protected:
  bool m_isOptional = true;
  bool m_isBind = false;
  open62541Cpp::UA_RelativPathElement m_relativPathElement;

public:
  bool IsOptional() { return m_isOptional; }
  bool IsBind() { return m_isBind; }

  void SetBind() { m_isBind = true; };
  void SetMandatory() { m_isOptional = false; };

  BindableMember();
  BindableMember(const T &val);
  UA_StatusCode StatusCode = UA_STATUSCODE_GOOD;
  std::optional<UA_DateTime> SourceTimestamp;
  std::optional<UA_DateTime> ServerTimestamp;
};

template <typename T>
BindableMember<T>::BindableMember(const T &val) : BindableMember()
{
  this->value = val;
}

template <typename T>
BindableMember<T>::BindableMember() {}