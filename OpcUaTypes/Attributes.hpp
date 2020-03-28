///
/// \file Attributes.hpp
/// \author Christian von Arnim
/// \date 19.02.2020
///


#pragma once

#include <refl.hpp>
#include "ConstNodeId.hpp"

namespace open62541Cpp::attribute
{
    struct UaBrowseName : refl::attr::usage::field{
        const char* NsURI = nullptr;
        const char* Name = nullptr;
    };

    struct UaVariableType : refl::attr::usage::type {
        UaVariableType() = default;
        open62541Cpp::constexp::NodeId NodeId;
    };

    struct UaObjectType : refl::attr::usage::type {
        UaObjectType() = default;
        open62541Cpp::constexp::NodeId NodeId;
    };

    /// \TOOD use constructor as NodeId is required
    struct UaReference : refl::attr::usage::field {
        UaReference() = default;
        open62541Cpp::constexp::NodeId NodeId;
    };

    struct UaVariableTypeValue : refl::attr::usage::field {

    };

}
