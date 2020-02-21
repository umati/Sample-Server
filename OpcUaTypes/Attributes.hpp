///
/// \file Attributes.hpp
/// \author Christian von Arnim
/// \date 19.02.2020
///


#pragma once

#include <refl.hpp>

namespace open62541Cpp::attribute
{
    struct UaBrowseName : refl::attr::usage::field{
        const char* NsURI = nullptr;
        const char* Name = nullptr;
    };

    struct UaVariableType : refl::attr::usage::type {
    };

    struct UaObjectType : refl::attr::usage::type {
    };

    struct UaVariableTypeValue : refl::attr::usage::field {

    };

}
