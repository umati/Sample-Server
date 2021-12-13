/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once

#include <refl.hpp>

#include "ConstNodeId.hpp"

namespace UmatiServerLib::attribute {
struct UaBrowseName : refl::attr::usage::field {
  UaBrowseName() = default;
  constexpr UaBrowseName(const char *nsURI, const char *name = nullptr) : NsURI(nsURI), Name(name) {}
  const char *NsURI = nullptr;
  const char *Name = nullptr;
};

struct UaVariableType : refl::attr::usage::type {
  UaVariableType() = default;
  constexpr UaVariableType(UmatiServerLib::constexp::NodeId nodeId) : NodeId(nodeId) {}
  UmatiServerLib::constexp::NodeId NodeId;
};

struct UaObjectType : refl::attr::usage::type {
  UaObjectType() = default;
  constexpr UaObjectType(UmatiServerLib::constexp::NodeId nodeId) : NodeId(nodeId) {}
  UmatiServerLib::constexp::NodeId NodeId;
};

/// \TOOD use constructor as NodeId is required
/// \TODO Replace by MemberInTypeNodeId, ReferenceType can be get from
///    browse for reverse hierarchical reference
struct UaReference : refl::attr::usage::field {
  UaReference() = default;
  UmatiServerLib::constexp::NodeId NodeId;
};

/// Contains the nodeId of the member in the type hierachy. Can be used to
///  read parent type nodes, reference types, attriutes like dataType,
///  TypeDefinition ...
struct MemberInTypeNodeId : refl::attr::usage::field {
  MemberInTypeNodeId() = default;
  constexpr MemberInTypeNodeId(UmatiServerLib::constexp::NodeId nodeId) : NodeId(nodeId) {}
  UmatiServerLib::constexp::NodeId NodeId;
};

// This identifier the Value memer in a variable type.
struct UaVariableTypeValue : refl::attr::usage::field {};

struct PlaceholderOptional : refl::attr::usage::field {};

}  // namespace UmatiServerLib::attribute
