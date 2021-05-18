#pragma once

#include <open62541/server.h>

#include <Open62541Cpp/UA_BrowsePath.hpp>
#include <Open62541Cpp/UA_NodeId.hpp>
#include <iostream>
#include <refl.hpp>

#include "../Exceptions/NodeNotFound.hpp"
#include "../OpcUaTypes/Attributes.hpp"
#include "../OpcUaTypes/ConstNodeId.hpp"
#include "BindableMember.hpp"
#include "BindableMemberValue.hpp"
#include "Util.hpp"

open62541Cpp::UA_NodeId resolveBrowsePath(UA_Server *pServer, const open62541Cpp::UA_BrowsePath &brPath);

template <typename B, typename... T>
open62541Cpp::UA_RelativPathElement getBrowseName(const B &instance, const refl::descriptor::field_descriptor<T...> &member, UA_Server *pServer);

template <typename T>
void setBindOrMandatory(BindableMember<T> &instance, bool bind = true, bool mandatory = true);

template <typename T, typename = std::enable_if_t<!is_base_of_template<BindableMember, T>::value>>
void setBindOrMandatory(T &instance, bool bind = true, bool mandatory = true){};

template <typename T>
void setMemberInTypeNodeId(BindableMember<T> &instance, const UmatiServerLib::constexp::NodeId &nodeId, UA_Server *pServer);

template <typename T, typename = std::enable_if_t<!is_base_of_template<BindableMember, T>::value>>
void setMemberInTypeNodeId(T &instance, const UmatiServerLib::constexp::NodeId &nodeId, UA_Server *pServer) {
  static_assert(always_false<T>::value, "Try to set MemberInType for non Bindable Member");
};

template <typename T>
void setAddrSpaceLocation(BindableMember<T> &instance, const open62541Cpp::UA_NodeId &parentNodeId, const open62541Cpp::UA_RelativPathElement pathEl);

template <typename T, typename = std::enable_if_t<!is_base_of_template<BindableMember, T>::value>>
void setAddrSpaceLocation(T &instance, const open62541Cpp::UA_NodeId &parentNodeId, const open62541Cpp::UA_RelativPathElement pathEl){};

template <typename T>
open62541Cpp::UA_RelativPathElement getRelativPathElement(BindableMember<T> &instance) {
  return instance.RelativPathElement;
}

template <typename T, typename = std::enable_if_t<!is_base_of_template<BindableMember, T>::value>>
open62541Cpp::UA_RelativPathElement getRelativPathElement(T &instance) {
  return open62541Cpp::UA_RelativPathElement();
};

template <typename AttributeType, typename T>
constexpr bool hasAttributeIfReflectable() noexcept {
  if constexpr (!refl::trait::is_reflectable<T>::value) {
    return false;
  } else {
    return refl::descriptor::has_attribute<AttributeType>(refl::reflect<T>());
  }
}

template <typename AttributeType, typename T>
constexpr bool hasAttributeIfReflectable(const T &member) noexcept {
  return hasAttributeIfReflectable<AttributeType, T>();
}

template <typename T>
auto &getValueFromBindableMember(T &instance, bool bind = true, bool mandatory = true) {
  if constexpr (is_base_of_template<BindableMember, T>::value) {
    return instance.value;
  } else {
    return instance;
  }
}

/**
 * @brief Get the BrowseName from a member
 * Checks specific BrowseName definition, a
 * @tparam T
 * @param member
 * @param pServer
 * @return open62541Cpp::UA_RelativPathElement
 */
template <typename B, typename T, size_t N>
open62541Cpp::UA_RelativPathElement getBrowseName(const B &instance, const refl::descriptor::field_descriptor<T, N> &member, UA_Server *pServer) {
  std::uint16_t nsIndex = ~static_cast<std::uint16_t>(0);
  std::string name(member.name);
  const char *nsUri = nullptr;
  if constexpr (refl::descriptor::has_attribute<UmatiServerLib::attribute::UaBrowseName>(refl::descriptor::field_descriptor<T, N>())) {
    const auto &attrBrowseName = refl::descriptor::get_attribute<UmatiServerLib::attribute::UaBrowseName>(member);
    nsUri = attrBrowseName.NsURI;

    if (attrBrowseName.Name != nullptr) {
      name = attrBrowseName.Name;
    }
  } else if constexpr (refl::descriptor::has_attribute<UmatiServerLib::attribute::UaObjectType>(refl::reflect<B>())) {
    // Take index from type as default BrowseName Index
    auto objTypeAttr = refl::descriptor::get_attribute<UmatiServerLib::attribute::UaObjectType>(refl::reflect<B>());
    nsUri = objTypeAttr.NodeId.NsUri;
  } else if constexpr (refl::descriptor::has_attribute<UmatiServerLib::attribute::UaVariableType>(refl::reflect<B>())) {
    // Take index from type as default BrowseName Index
    auto varTypeAttr = refl::descriptor::get_attribute<UmatiServerLib::attribute::UaVariableType>(refl::reflect<B>());
    nsUri = varTypeAttr.NodeId.NsUri;
  } else {
    static_assert(always_false<B>::value, "Unknown source of browse name");
  }

  if (nsUri != nullptr) {
    nsIndex = nsFromUri(pServer, nsUri);
  } else {
    std::cout << "Namespace URI null" << std::endl;
    throw std::runtime_error("Namespace URI is null, no index for browse name could be identified.");
  }

  return open62541Cpp::UA_RelativPathElement(nsIndex, name);
}

template <typename T>
void setBindOrMandatory(BindableMember<T> &instance, bool bind, bool mandatory) {
  if (bind) {
    instance.SetBind();
  }

  if (mandatory) {
    instance.SetMandatory();
  }
}

template <typename T>
void setAddrSpaceLocation(BindableMember<T> &instance, const open62541Cpp::UA_NodeId &parentNodeId, const open62541Cpp::UA_RelativPathElement pathEl) {
  instance.ParentNodeId = parentNodeId;
  instance.RelativPathElement = pathEl;
}

template <typename T>
void setMemberInTypeNodeId(BindableMember<T> &instance, const UmatiServerLib::constexp::NodeId &memberInTypeNodeId, UA_Server *pServer) {
  instance.MemberInTypeNodeId = memberInTypeNodeId.UANodeId(pServer);
}
