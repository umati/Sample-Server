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

template <typename T>
void bindMemberRefl(
    T &member,
    UA_Server *pServer,
    UA_NodeId nodeId,
    open62541Cpp::UA_RelativPathBase base,
    NodesMaster &nodesMaster);

template <typename T>
void bindPlaceholder(
    std::list<T> &member,
    UA_Server *pServer,
    UA_NodeId nodeId,
    open62541Cpp::UA_RelativPathBase base,
    NodesMaster &nodesMaster,
    open62541Cpp::UA_NodeId refTypeNodeId);

open62541Cpp::UA_NodeId resolveBrowsePath(
    UA_Server *pServer,
    const open62541Cpp::UA_BrowsePath &brPath);

/**
 * @brief Binding the members by it's reflection description
 *
 * @tparam T Any type with defined reflection
 * @param instance Instance to be bind
 * @param pServer Pointer to OPC UA Server
 * @param nodeId Start reference for binding
 * @param base Base, added to nodeId for all nodes
 * @param nodesMaster NodesMaster instace for resolving the bindings.
 */
template <typename T>
void bindMembersRefl(T &instance, UA_Server *pServer, UA_NodeId nodeId, open62541Cpp::UA_RelativPathBase base, NodesMaster &nodesMaster)
{
  if constexpr (refl::descriptor::has_attribute<Bases>(refl::reflect<T>()))
  {
    constexpr auto bases = refl::descriptor::get_attribute<Bases>(refl::reflect<T>());
    if constexpr (bases.descriptors.size)
    {
      refl::util::for_each(bases.descriptors, [&](auto t) {
        bindMembersRefl(static_cast<typename decltype(t)::type &>(instance), pServer, nodeId, base, nodesMaster);
      });
    }
  }

  for_each(refl::reflect(instance).members, [&](auto member) {
    auto childRelativPathElements = base();

    if constexpr (
        is_same_template<typename decltype(member)::value_type, std::list>::value)
    {
      std::cout << member.name << " is a placeholder." << std::endl;
      std::cout << "Placeholders are not implemented." << std::endl;
      ///\todo Read placeholder instances from Server.

      if constexpr (
          !refl::descriptor::has_attribute<open62541Cpp::attribute::UaReference>(member))
      {
        std::cout << "Placeholder " << member.name << " has no UaReference." << std::endl;
        throw std::runtime_error("Required attribute UaReference not found.");
      }
      const auto &reference = refl::descriptor::get_attribute<open62541Cpp::attribute::UaReference>(member);
      auto refTypeNodeId = reference.NodeId.UANodeId(pServer);

      bindPlaceholder(member(instance), pServer, nodeId, base, nodesMaster, refTypeNodeId);

      return;
    }

    // Check if this is not the value of a variable type
    // If this is the value of a variable type bind it to the base without appending a browse name (skip adding a browse name)
    if constexpr (
        !refl::descriptor::has_attribute<open62541Cpp::attribute::UaVariableTypeValue>(member))
    {
      std::uint16_t nsIndex = 2;
      std::string name(member.name);
      if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::UaBrowseName>(member))
      {
        const auto &attrBrowseName = refl::descriptor::get_attribute<open62541Cpp::attribute::UaBrowseName>(member);
        if (attrBrowseName.NsURI != nullptr)
        {
          nsIndex = nsFromUri(pServer, attrBrowseName.NsURI);
        }

        if (attrBrowseName.Name != nullptr)
        {
          name = attrBrowseName.Name;
        }
      }
      else if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<T>()))
      {
        // Take index from type as default BrowseName Index
        auto objTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<T>());
        if (objTypeAttr.NodeId.NsUri != nullptr)
        {
          nsIndex = nsFromUri(pServer, objTypeAttr.NodeId.NsUri);
        }
        else
        {
          std::cout << "No NodeId at object type provided!" << std::endl;
        }
      }
      else if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::UaVariableType>(refl::reflect<T>()))
      {
        // Take index from type as default BrowseName Index
        auto varTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaVariableType>(refl::reflect<T>());
        if (varTypeAttr.NodeId.NsUri != nullptr)
        {
          nsIndex = nsFromUri(pServer, varTypeAttr.NodeId.NsUri);
        }
        else
        {
          std::cout << "No NodeId at variable type provided!" << std::endl;
        }
      }

      childRelativPathElements.push_back(open62541Cpp::UA_RelativPathElement(nsIndex, name));
    }

    bindMemberRefl(member(instance), pServer, nodeId, childRelativPathElements, nodesMaster);
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
void bindMemberRefl(T &member, UA_Server *pServer, UA_NodeId nodeId, open62541Cpp::UA_RelativPathBase base, NodesMaster &nodesMaster)
{
  constexpr bool isReflectable = refl::trait::is_reflectable<T>::value;
  if constexpr (
      hasAttributeIfReflectable<open62541Cpp::attribute::UaObjectType>(member) ||
      hasAttributeIfReflectable<open62541Cpp::attribute::UaVariableType>(member))
  {
    bindMembersRefl(member, pServer, nodeId, base, nodesMaster);
  }
  else
  {
    auto nodeIdMember = resolveBrowsePath(
        pServer,
        open62541Cpp::UA_BrowsePath(
            nodeId,
            base()));
    if constexpr (is_same_template<T, BindableMember>::value)
    {
      bindVariableByPath(
          pServer,
          nodeIdMember,
          nodesMaster,
          member);
    }
    else
    {
      bindValueByPath(
          pServer,
          nodeIdMember,
          nodesMaster,
          member);
    }
  }
}

template <typename T>
void bindPlaceholder(std::list<T> &member, UA_Server *pServer, UA_NodeId baseNodeId, open62541Cpp::UA_RelativPathBase base, NodesMaster &nodesMaster, open62541Cpp::UA_NodeId refTypeNodeId)
{
  auto nodeId = resolveBrowsePath(
      pServer, open62541Cpp::UA_BrowsePath(
                   baseNodeId,
                   base()));
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
      bindMembersRefl(instance, pServer, browseResult.references[i].nodeId.nodeId, {}, nodesMaster);
    }
  }

  UA_BrowseResult_deleteMembers(&browseResult);
  UA_BrowseDescription_deleteMembers(&brDesc);
}
