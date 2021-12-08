/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once

#include <open62541/server.h>

#include <Open62541Cpp/UA_BrowsePath.hpp>
#include <Open62541Cpp/UA_NodeId.hpp>
#include <Open62541Cpp/UA_RelativPathBase.hpp>
#include <iostream>
#include <list>

#include "../Exceptions/NodeNotFound.hpp"
#include "BindHelper.hpp"
#include "NodesMaster.hpp"

namespace UmatiServerLib {
class Unbind {
 public:
  template <typename T>
  static void MemberRefl(T &member, UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, NodesMaster &nodesMaster);

  template <typename T>
  static void MembersRefl(T &instance, UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, NodesMaster &nodesMaster);
};

template <typename T>
void Unbind::MembersRefl(T &instance, UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, NodesMaster &nodesMaster) {
  open62541Cpp::UA_RelativPathBase basePath;
  // Handle base classes first
  if constexpr (refl::descriptor::has_attribute<Bases>(refl::reflect<T>())) {
    constexpr auto bases = refl::descriptor::get_attribute<Bases>(refl::reflect<T>());
    if constexpr (bases.descriptors.size) {
      refl::util::for_each(bases.descriptors, [&](auto t) { MembersRefl(static_cast<typename decltype(t)::type &>(instance), pServer, nodeId, nodesMaster); });
    }
  }

  for_each(refl::reflect(instance).members, [&](auto member) {
    auto childRelativPathElements = basePath();

    if constexpr (is_same_template<typename decltype(member)::value_type, std::list>::value) {
      std::cout << member.name << " is a placeholder." << std::endl;
      std::cout << "Unbind placeholder not implemented." << std::endl;
      return;
    }

    open62541Cpp::UA_RelativPathElement pathEl(getRelativPathElement(member(instance)));
    // Check if path was set by base class? => Skip use the previous RelativPath (with correct namespace)
    if (pathEl.RelativePathElement == nullptr) {
      // Check if this is not the value of a variable type
      // If this is the value of a variable type bind it to the base without appending a browse name (skip adding a browse name)
      if constexpr (!refl::descriptor::has_attribute<UmatiServerLib::attribute::UaVariableTypeValue>(member)) {
        pathEl = getBrowseName(instance, member, pServer);
        childRelativPathElements.push_back(pathEl);
      }
    } else {
      childRelativPathElements.push_back(pathEl);
    }

    constexpr bool isOptional = refl::descriptor::has_attribute<UmatiServerLib::attribute::PlaceholderOptional>(member);
    try {
      auto nodeIdChild = resolveBrowsePath(pServer, open62541Cpp::UA_BrowsePath(*nodeId.NodeId, childRelativPathElements));
      MemberRefl(member(instance), pServer, nodeIdChild, nodesMaster);
    } catch (const UmatiServerLib::Exceptions::NodeNotFound &ex) {
      if constexpr (!isOptional) {
        std::stringstream ss;
        ss << "Mandatory node not found for binding. " << ex.what();
        throw std::runtime_error(ss.str());
      }
    }
  });
}

/**
 * @brief Remove all bindings of this member
 *
 * @tparam T
 * @param member
 * @param pServer
 * @param nodeId
 * @param nodesMaster
 */
template <typename T>
void Unbind::MemberRefl(T &member, UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, NodesMaster &nodesMaster) {
  if constexpr (
    hasAttributeIfReflectable<UmatiServerLib::attribute::UaObjectType, T>() || hasAttributeIfReflectable<UmatiServerLib::attribute::UaVariableType, T>()) {
    MembersRefl(member, pServer, nodeId, nodesMaster);
  } else if constexpr (is_same_template<T, BindableMember>::value) {
    if constexpr (
      hasAttributeIfReflectable<UmatiServerLib::attribute::UaObjectType>(member.value) ||
      hasAttributeIfReflectable<UmatiServerLib::attribute::UaVariableType>(member.value)) {
      MembersRefl(member.value, pServer, nodeId, nodesMaster);
    } else {
      static_assert(always_false<T>::value, "BindableMember must be an Object or VariableType");
    }
  }

  nodesMaster.Remove(nodeId);
  if constexpr (is_base_of_template<BindableMember, T>::value) {
    member.ResetBind();
  }
}

}  // namespace UmatiServerLib
