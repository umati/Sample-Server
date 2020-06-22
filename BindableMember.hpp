#pragma once

#include "ValueDecorator.hpp"
#include <Open62541Cpp/UA_NodeId.hpp>
#include <Open62541Cpp/UA_RelativPathElement.hpp>

template <typename T>
class BindableMember : public ValueDecorator<T>
{
  protected:
  bool m_isOptional = true;
  bool m_isBind = false;

public:
  bool IsOptional() { return m_isOptional; }
  bool IsBind() { return m_isBind; }

  void SetBind() { m_isBind = true; };
  void ResetBind() { m_isBind = false; };
  void SetMandatory() { m_isOptional = false; };

  /// NodeId of the parent element, enable creating an optional member later on
  open62541Cpp::UA_NodeId ParentNodeId;

  /// \TODO replace by utilizing MemberInTypeNodeId
  open62541Cpp::UA_RelativPathElement RelativPathElement;

  /// NodeId of this member in the type definition (taken from MemberInTypeNodeId atribute)
  /// \TODO Replace by pointer to attribute
  open62541Cpp::UA_NodeId MemberInTypeNodeId;

  /// NodeId, when binded. Enable deletion later on
  open62541Cpp::UA_NodeId NodeId;

  BindableMember() = default;
  BindableMember(const T &val);
  virtual ~BindableMember() {}
};

template <typename T>
BindableMember<T>::BindableMember(const T &val) : ValueDecorator<T>(val)
{}
