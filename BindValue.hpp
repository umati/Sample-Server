///
/// \file BindValue.hpp
/// \author Christian von Arnim
/// \date 17.12.2019
///

#pragma once

#include <cstdint>
#include <string>

#include "NodeValue.hpp"

void bindValue(NodeValue &nodeValue, std::uint32_t &variable);
void bindValue(NodeValue &nodeValue, std::string &variable);

