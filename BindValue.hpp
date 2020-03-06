///
/// \file BindValue.hpp
/// \author Christian von Arnim
/// \date 17.12.2019
///

#pragma once

#include <cstdint>
#include <string>

#include "NodeValue.hpp"
#include "OpcUaTypes/DateTime.hpp"
#include <variant>

using primitivTypes_t = std::variant<
    std::int8_t*,
    std::int16_t*,
    std::int32_t*,
    std::int64_t*,
    std::uint8_t*,
    std::uint16_t*,
    std::uint32_t*,
    std::uint64_t*,
    float*,
    double*
    >;

void bindValue(NodeValue &nodeValue, primitivTypes_t pVariable);
void bindValue(NodeValue &nodeValue, std::string *variable);
void bindValue(NodeValue &nodeValue, open62541Cpp::DateTime_t *variable);
void bindValue(NodeValue &nodeValue, bool *variable);

