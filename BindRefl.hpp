#pragma once

#include <iostream>
#include <open62541/server.h>
#include "NodesMaster.hpp"
#include <refl.hpp>
#include <list>
#include <Open62541Cpp/UA_RelativPathBase.hpp>
#include <Open62541Cpp/UA_BrowsePath.hpp>
#include "BindValueHelper.hpp"
#include "Util.hpp"
#include "OpcUaTypes/Attributes.hpp"
#include "Exceptions/NodeNotFound.hpp"

template <typename T>
void bindMemberRefl(
    T &member,
    UA_Server *pServer,
    open62541Cpp::UA_NodeId nodeId,
    NodesMaster &nodesMaster);

template <typename T>
void bindPlaceholder(
    std::list<T> &member,
    UA_Server *pServer,
    open62541Cpp::UA_NodeId nodeId,
    NodesMaster &nodesMaster,
    open62541Cpp::UA_NodeId refTypeNodeId);

open62541Cpp::UA_NodeId resolveBrowsePath(
    UA_Server *pServer,
    const open62541Cpp::UA_BrowsePath &brPath);

template <typename B, typename... T>
open62541Cpp::UA_RelativPathElement getBrowseName(const B &instance, const refl::descriptor::field_descriptor<T...> &member, UA_Server *pServer);

template <typename T>
void setBindOrMandatory(BindableMember<T> &instance, bool bind = true, bool mandatory = true);

template <typename T, typename = std::enable_if_t<!is_base_of_template<BindableMember, T>::value>>
void setBindOrMandatory(T &instance, bool bind = true, bool mandatory = true){};

template <typename T>
void setAddrSpaceLocation(BindableMember<T> &instance, const open62541Cpp::UA_NodeId &nodeId, const open62541Cpp::UA_RelativPathElement pathEl);

template <typename T, typename = std::enable_if_t<!is_base_of_template<BindableMember, T>::value>>
void setAddrSpaceLocation(T &instance, const open62541Cpp::UA_NodeId &nodeId, const open62541Cpp::UA_RelativPathElement pathEl){};

/**
 * @brief Binding the members by it's reflection description
 *
 * @tparam T Any type with defined reflection
 * @param instance Instance to be bind
 * @param pServer Pointer to OPC UA Server
 * @param nodeId Start reference for binding
 * @param nodesMaster NodesMaster instace for resolving the bindings.
 */
template <typename T>
void bindMembersRefl(T &instance, UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, NodesMaster &nodesMaster)
{
  open62541Cpp::UA_RelativPathBase basePath;
  // Handle base classes first
  if constexpr (refl::descriptor::has_attribute<Bases>(refl::reflect<T>()))
  {
    constexpr auto bases = refl::descriptor::get_attribute<Bases>(refl::reflect<T>());
    if constexpr (bases.descriptors.size)
    {
      refl::util::for_each(bases.descriptors, [&](auto t) {
        bindMembersRefl(static_cast<typename decltype(t)::type &>(instance), pServer, nodeId, nodesMaster);
      });
    }
  }

  for_each(refl::reflect(instance).members, [&](auto member) {
    auto childRelativPathElements = basePath();

    if constexpr (
        is_same_template<typename decltype(member)::value_type, std::list>::value)
    {
      std::cout << member.name << " is a placeholder." << std::endl;

      if constexpr (
          !refl::descriptor::has_attribute<open62541Cpp::attribute::UaReference>(member))
      {
        std::cout << "Placeholder " << member.name << " has no UaReference." << std::endl;
        throw std::runtime_error("Required attribute UaReference not found.");
      }
      const auto &reference = refl::descriptor::get_attribute<open62541Cpp::attribute::UaReference>(member);
      auto refTypeNodeId = reference.NodeId.UANodeId(pServer);

      bindPlaceholder(member(instance), pServer, nodeId, nodesMaster, refTypeNodeId);

      return;
    }

    open62541Cpp::UA_RelativPathElement pathEl;
    // Check if this is not the value of a variable type
    // If this is the value of a variable type bind it to the base without appending a browse name (skip adding a browse name)
    if constexpr (
        !refl::descriptor::has_attribute<open62541Cpp::attribute::UaVariableTypeValue>(member))
    {
      pathEl = getBrowseName(instance, member, pServer);
      childRelativPathElements.push_back(pathEl);
    }
    constexpr bool isOptional = refl::descriptor::has_attribute<open62541Cpp::attribute::PlaceholderOptional>(member);
    try
    {
      auto nodeIdChild = resolveBrowsePath(
          pServer,
          open62541Cpp::UA_BrowsePath(
              *nodeId.NodeId,
              childRelativPathElements));
      bindMemberRefl(member(instance), pServer, nodeIdChild, nodesMaster);
      setBindOrMandatory(member(instance), true, !isOptional);
      setAddrSpaceLocation(member(instance), nodeId, pathEl);
    }
    catch (const open62541Cpp::Exceptions::NodeNotFound &ex)
    {
      if constexpr (!isOptional)
      {
        std::stringstream ss;
        ss << "Mandatory node not found for binding. " << ex.what();
        throw std::runtime_error(ss.str());
      }
    }
  });
}

template <typename AttributeType, typename T>
constexpr bool hasAttributeIfReflectable(const T &member) noexcept
{
  if constexpr (!refl::trait::is_reflectable<T>::value)
  {
    return false;
  }
  else
  {
    return refl::descriptor::has_attribute<AttributeType>(
        refl::reflect(member));
  }
}

template <typename T>
auto &getValueFromBindableMember(T &instance, bool bind = true, bool mandatory = true)
{
  if constexpr (is_base_of_template<BindableMember, T>::value)
  {
    return instance.value;
  }
  else
  {
    return instance;
  }
}

/**
 * @brief Bind a member/instance using reflection
 *
 * When the member is a Object or Variable all childs are iterated. Otherwise the value is bind to the correspoding OPC UA Value
 *
 * @tparam T
 * @param member
 * @param pServer
 * @param nodeId

 * @param nodesMaster
 */
template <typename T>
void bindMemberRefl(
    T &member, UA_Server *pServer,
    open62541Cpp::UA_NodeId nodeId,
    NodesMaster &nodesMaster)
{
  if constexpr (
      hasAttributeIfReflectable<open62541Cpp::attribute::UaObjectType>(member) ||
      hasAttributeIfReflectable<open62541Cpp::attribute::UaVariableType>(member))
  {
    bindMembersRefl(member, pServer, nodeId, nodesMaster);
  }
  else if constexpr (
      is_same_template<T, BindableMember>::value)
  {
    if constexpr (
        hasAttributeIfReflectable<open62541Cpp::attribute::UaObjectType>(member.value) ||
        hasAttributeIfReflectable<open62541Cpp::attribute::UaVariableType>(member.value))
    {
      bindMembersRefl(member.value, pServer, nodeId, nodesMaster);
    }
    else
    {
      static_assert(always_false<T>::value, "BindableMember must be an Object or VariableType");
    }
  }
  else
  {
    if constexpr (is_same_template<T, BindableMemberValue>::value)
    {
      bindVariableByPath(
          pServer,
          nodeId,
          nodesMaster,
          member);
    }
    else
    {
      bindValueByPath(
          pServer,
          nodeId,
          nodesMaster,
          member);
    }
  }
}

template <typename T>
void bindPlaceholder(
    std::list<T> &member,
    UA_Server *pServer,
    open62541Cpp::UA_NodeId nodeId,
    NodesMaster &nodesMaster,
    open62541Cpp::UA_NodeId refTypeNodeId)
{
  UA_BrowseDescription brDesc;
  UA_BrowseDescription_init(&brDesc);
  UA_NodeId_copy(nodeId.NodeId, &brDesc.nodeId);
  UA_NodeId_copy(refTypeNodeId.NodeId, &brDesc.referenceTypeId);

  open62541Cpp::UA_NodeId typeNodeId;

  if constexpr (!refl::trait::is_reflectable<T>::value)
  {
    static_assert(always_false<T>::value, "T not reflectable");
  }

  if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<T>()))
  {
    auto objTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<T>());
    typeNodeId = objTypeAttr.NodeId.UANodeId(pServer);
    brDesc.nodeClassMask = UA_NODECLASS_OBJECT;
  }
  else
  {
    // TODO handle variable types
    static_assert(always_false<T>::value, "T has attribute UaObjectType or (UaVariableType -- not implemented).");
  }

  brDesc.browseDirection = UA_BrowseDirection::UA_BROWSEDIRECTION_FORWARD;
  brDesc.includeSubtypes = UA_TRUE;
  brDesc.resultMask = UA_BROWSERESULTMASK_ALL;

  auto browseResult = UA_Server_browse(pServer, UA_UINT32_MAX, &brDesc);
  if (browseResult.statusCode != UA_STATUSCODE_GOOD)
  {
    std::cout << "Resutl not good: " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
    std::cout << "Could not find placeholders." << std::endl;
  }

  for (std::size_t i = 0; i < browseResult.referencesSize; ++i)
  {
    open62541Cpp::UA_String txt(&browseResult.references[i].displayName.text);

    std::cout << "Placeholder possible value: " << txt << std::endl;
    if (UA_NodeId_equal(&browseResult.references[i].typeDefinition.nodeId, typeNodeId.NodeId))
    {
      std::cout << "Placeholder type match" << std::endl;
      member.push_back(T());
      T &instance = *(member.rbegin());
      bindMembersRefl(instance, pServer, open62541Cpp::UA_NodeId(browseResult.references[i].nodeId.nodeId), nodesMaster);
    }
  }

  UA_BrowseResult_deleteMembers(&browseResult);
  UA_BrowseDescription_deleteMembers(&brDesc);
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
open62541Cpp::UA_RelativPathElement getBrowseName(const B &instance, const refl::descriptor::field_descriptor<T, N> &member, UA_Server *pServer)
{
  std::uint16_t nsIndex = ~static_cast<std::uint16_t>(0);
  std::string name(member.name);
  const char *nsUri = nullptr;
  if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::UaBrowseName>(member))
  {
    const auto &attrBrowseName = refl::descriptor::get_attribute<open62541Cpp::attribute::UaBrowseName>(member);
    nsUri = attrBrowseName.NsURI;

    if (attrBrowseName.Name != nullptr)
    {
      name = attrBrowseName.Name;
    }
  }
  else if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<B>()))
  {
    // Take index from type as default BrowseName Index
    auto objTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<B>());
    nsUri = objTypeAttr.NodeId.NsUri;
  }
  else if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::UaVariableType>(refl::reflect<B>()))
  {
    // Take index from type as default BrowseName Index
    auto varTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaVariableType>(refl::reflect<B>());
    nsUri = varTypeAttr.NodeId.NsUri;
  }
  else
  {
    static_assert(always_false<B>::value, "Unknown source of browse name");
  }

  if (nsUri != nullptr)
  {
    nsIndex = nsFromUri(pServer, nsUri);
  }
  else
  {
    std::cout << "Namespace URI null" << std::endl;
    throw std::runtime_error("Namespace URI is null, no index for browse name could be identified.");
  }

  return open62541Cpp::UA_RelativPathElement(nsIndex, name);
}

template <typename T>
void setBindOrMandatory(BindableMember<T> &instance, bool bind, bool mandatory)
{
  if (bind)
  {
    instance.SetBind();
  }

  if (mandatory)
  {
    instance.SetMandatory();
  }
}

template <typename T>
void setAddrSpaceLocation(BindableMember<T> &instance, const open62541Cpp::UA_NodeId &nodeId, const open62541Cpp::UA_RelativPathElement pathEl)
{
  instance.ParentNodeId = nodeId;
  instance.RelativPathElement = pathEl;
}
