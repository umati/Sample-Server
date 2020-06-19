#pragma once
#include "BindHelper.hpp"

#include <iostream>
#include <open62541/server.h>
#include "NodesMaster.hpp"
#include <refl.hpp>
#include <list>
#include <Open62541Cpp/UA_RelativPathBase.hpp>
#include <Open62541Cpp/UA_BrowsePath.hpp>
#include "BindVariable.hpp"
#include "Util.hpp"
#include "OpcUaTypes/Attributes.hpp"
#include "Exceptions/NodeNotFound.hpp"
#include <variant>

namespace UmatiServerLib
{
  class Bind
  {
  public:
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
    static void MemberRefl(
        T &member,
        UA_Server *pServer,
        open62541Cpp::UA_NodeId nodeId,
        NodesMaster &nodesMaster);

    template <typename... T>
    static void MemberRefl(
        std::variant<T...> &member, UA_Server *pServer,
        open62541Cpp::UA_NodeId nodeId,
        NodesMaster &nodesMaster);

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
    static void MembersRefl(
        T &instance,
        UA_Server *pServer,
        open62541Cpp::UA_NodeId nodeId,
        NodesMaster &nodesMaster);

    template <typename T>
    static void Placeholder(
        std::list<T> &member,
        UA_Server *pServer,
        open62541Cpp::UA_NodeId nodeId,
        NodesMaster &nodesMaster,
        open62541Cpp::UA_NodeId refTypeNodeId);
  };

  template <typename T>
  void Bind::MembersRefl(T &instance, UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, NodesMaster &nodesMaster)
  {
    open62541Cpp::UA_RelativPathBase basePath;
    // Handle base classes first
    if constexpr (refl::descriptor::has_attribute<Bases>(refl::reflect<T>()))
    {
      constexpr auto bases = refl::descriptor::get_attribute<Bases>(refl::reflect<T>());
      if constexpr (bases.descriptors.size)
      {
        refl::util::for_each(bases.descriptors, [&](auto t) {
          MembersRefl(static_cast<typename decltype(t)::type &>(instance), pServer, nodeId, nodesMaster);
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

      if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::MemberInTypeNodeId>(member))
      {
        auto &attr = refl::descriptor::get_attribute<open62541Cpp::attribute::MemberInTypeNodeId>(member);
        setMemberInTypeNodeId(member(instance), attr.NodeId, pServer);
      }

      open62541Cpp::UA_RelativPathElement pathEl(getRelativPathElement(member(instance)));
      // Check if path was set by base class? => Skip use the previous RelativPath (with correct namespace)
      if (pathEl.RelativePathElement == nullptr)
      {
        // Check if this is not the value of a variable type
        // If this is the value of a variable type bind it to the base without appending a browse name (skip adding a browse name)
        if constexpr (
            !refl::descriptor::has_attribute<open62541Cpp::attribute::UaVariableTypeValue>(member))
        {
          pathEl = getBrowseName(instance, member, pServer);
          childRelativPathElements.push_back(pathEl);
        }
      }
      else
      {
        childRelativPathElements.push_back(pathEl);
      }

      constexpr bool isOptional = refl::descriptor::has_attribute<open62541Cpp::attribute::PlaceholderOptional>(member);
      try
      {
        setAddrSpaceLocation(member(instance), nodeId, pathEl);
        auto nodeIdChild = resolveBrowsePath(
            pServer,
            open62541Cpp::UA_BrowsePath(
                *nodeId.NodeId,
                childRelativPathElements));
        MemberRefl(member(instance), pServer, nodeIdChild, nodesMaster);
        setBindOrMandatory(member(instance), true, !isOptional);
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

  template <typename T>
  void Bind::MemberRefl(
      T &member, UA_Server *pServer,
      open62541Cpp::UA_NodeId nodeId,
      NodesMaster &nodesMaster)
  {
    if constexpr (
        is_base_of_template<BindableMember, T>::value)
    {
      member.NodeId = nodeId;
    }

    if constexpr (
        hasAttributeIfReflectable<open62541Cpp::attribute::UaObjectType, T>() ||
        hasAttributeIfReflectable<open62541Cpp::attribute::UaVariableType, T>())
    {
      MembersRefl(member, pServer, nodeId, nodesMaster);
    }
    else if constexpr (
        is_same_template<T, BindableMember>::value)
    {
      if constexpr (
          hasAttributeIfReflectable<open62541Cpp::attribute::UaObjectType>(member.value) ||
          hasAttributeIfReflectable<open62541Cpp::attribute::UaVariableType>(member.value))
      {
        MembersRefl(member.value, pServer, nodeId, nodesMaster);
      }
      else
      {
        static_assert(always_false<T>::value, "BindableMember must be an Object or VariableType");
      }
    }
    else
    {
      BindVariable::ToNode(
          pServer,
          nodeId,
          nodesMaster,
          member);
    }
  }

  template <typename... T>
  void Bind::MemberRefl(
      std::variant<T...> &member, UA_Server *pServer,
      open62541Cpp::UA_NodeId nodeId,
      NodesMaster &nodesMaster)
  {
    /// todo catch variant in BindableMember
    std::visit(
        [&](auto &&arg) {
          MemberRefl(arg, pServer, nodeId, nodesMaster);
        },
        member);
  }

  template <typename T>
  void Bind::Placeholder(
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
        MembersRefl(instance, pServer, open62541Cpp::UA_NodeId(browseResult.references[i].nodeId.nodeId), nodesMaster);
      }
    }

    UA_BrowseResult_deleteMembers(&browseResult);
    UA_BrowseDescription_deleteMembers(&brDesc);
  }

} // namespace UmatiServerLib
