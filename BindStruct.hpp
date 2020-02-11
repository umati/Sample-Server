///
/// \file BindStruct.hpp
/// \author Christian von Arnim
/// \date 11.02.2020
///

#pragma once
#include "NodeValue.hpp"
#include <open62541/types.h>

void bindStruct(NodeValue &nodeValue, void *pVariable, const UA_DataType *typeDefinition);
