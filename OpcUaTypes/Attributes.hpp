///
/// \file Attributes.hpp
/// \author Christian von Arnim
/// \date 19.02.2020
///

#pragma once

#include <refl.hpp>
#include "ConstNodeId.hpp"

namespace UmatiServerLib::attribute
{
  struct UaBrowseName : refl::attr::usage::field
  {
    const char *NsURI = nullptr;
    const char *Name = nullptr;
  };

  struct UaVariableType : refl::attr::usage::type
  {
    UaVariableType() = default;
    UmatiServerLib::constexp::NodeId NodeId;
  };

  struct UaObjectType : refl::attr::usage::type
  {
    UaObjectType() = default;
    UmatiServerLib::constexp::NodeId NodeId;
  };

  /// \TOOD use constructor as NodeId is required
  /// \TODO Replace by MemberInTypeNodeId, ReferenceType can be get from
  ///    browse for reverse hierarchical reference
  struct UaReference : refl::attr::usage::field
  {
    UaReference() = default;
    UmatiServerLib::constexp::NodeId NodeId;
  };

  /// Contains the nodeId of the member in the type hierachy. Can be used to
  ///  read parent type nodes, reference types, attriutes like dataType,
  ///  TypeDefinition ...
  struct MemberInTypeNodeId : refl::attr::usage::field
  {
    MemberInTypeNodeId() = default;
    UmatiServerLib::constexp::NodeId NodeId;
  };

  // This identifier the Value memer in a variable type.
  struct UaVariableTypeValue : refl::attr::usage::field
  {
  };

  struct PlaceholderOptional : refl::attr::usage::field
  {
  };

} // namespace UmatiServerLib::attribute
