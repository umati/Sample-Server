
#include "ValueDecorator.hpp"

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
  void SetMandatory() { m_isOptional = false; };

  // Store location, so the node can be created at a later time
  open62541Cpp::UA_RelativPathElement RelativPathElement;
  open62541Cpp::UA_NodeId ParentNodeId;

  BindableMember() = default;
  BindableMember(const T &val);
};

template <typename T>
BindableMember<T>::BindableMember(const T &val) : ValueDecorator<T>(val)
{}
