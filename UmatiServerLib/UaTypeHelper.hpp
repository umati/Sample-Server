/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2019-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include <open62541/types.h>
#include <open62541/types_generated.h>

struct primitivTypeVisitor_getPointer {
  inline const void* operator()(std::int8_t* p) { return p; }

  inline const void* operator()(std::int16_t* p) { return p; }

  inline const void* operator()(std::int32_t* p) { return p; }

  inline const void* operator()(std::int64_t* p) { return p; }

  inline const void* operator()(std::uint8_t* p) { return p; }

  inline const void* operator()(std::uint16_t* p) { return p; }

  inline const void* operator()(std::uint32_t* p) { return p; }

  inline const void* operator()(std::uint64_t* p) { return p; }

  inline const void* operator()(float* p) { return p; }

  inline const void* operator()(double* p) { return p; }
};

struct primitivTypeVisitor_getTypePointer {
  inline const UA_DataType* operator()(std::int8_t*) { return &UA_TYPES[UA_TYPES_SBYTE]; }

  inline const UA_DataType* operator()(std::int16_t*) { return &UA_TYPES[UA_TYPES_INT16]; }

  inline const UA_DataType* operator()(std::int32_t*) { return &UA_TYPES[UA_TYPES_INT32]; }

  inline const UA_DataType* operator()(std::int64_t*) { return &UA_TYPES[UA_TYPES_INT64]; }

  inline const UA_DataType* operator()(std::uint8_t*) { return &UA_TYPES[UA_TYPES_BYTE]; }

  inline const UA_DataType* operator()(std::uint16_t*) { return &UA_TYPES[UA_TYPES_UINT16]; }

  inline const UA_DataType* operator()(std::uint32_t*) { return &UA_TYPES[UA_TYPES_UINT32]; }

  inline const UA_DataType* operator()(std::uint64_t*) { return &UA_TYPES[UA_TYPES_UINT64]; }

  inline const UA_DataType* operator()(float*) { return &UA_TYPES[UA_TYPES_FLOAT]; }

  inline const UA_DataType* operator()(double*) { return &UA_TYPES[UA_TYPES_DOUBLE]; }
};
