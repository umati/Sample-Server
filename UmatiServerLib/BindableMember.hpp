/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once

#include <vector>
#include <functional>
#include <Open62541Cpp/UA_NodeId.hpp>
#include <Open62541Cpp/UA_RelativPathElement.hpp>

#include "ValueDecorator.hpp"

template <typename T>
class BindableMember : public ValueDecorator<T> {
 protected:
  bool m_isOptional = true;
  bool m_isBind = false;

 public:
  bool IsOptional() { return m_isOptional; }
  bool IsBind() { return m_isBind; }

  void SetBind() { m_isBind = true; };
  void ResetBind() { m_isBind = false; };
  void SetMandatory() { m_isOptional = false; };

  /// NodeId of the parent element, enable creating an optional member later on
  open62541Cpp::UA_NodeId ParentNodeId;

  /// \TODO replace by utilizing MemberInTypeNodeId
  open62541Cpp::UA_RelativPathElement RelativPathElement;

  /// NodeId of this member in the type definition (taken from MemberInTypeNodeId atribute)
  /// \TODO Replace by pointer to attribute
  open62541Cpp::UA_NodeId MemberInTypeNodeId;

  /// NodeId, when binded. Enable deletion later on
  open62541Cpp::UA_NodeId NodeId;

  /// NodeId, when part of publishedDataSets
  std::vector<open62541Cpp::UA_NodeId> PublishedDataSetNodeIds;
  std::vector<open62541Cpp::UA_NodeId> DataSetFieldNodeIds;

  template <typename B>
  void CallOnDataSetFieldId(std::function<void(B, UA_NodeId)> func, B as, UA_Boolean minor, UA_Boolean major) {
    std::for_each(DataSetFieldNodeIds.begin(), DataSetFieldNodeIds.end(), 
      [func, as, minor, major](auto pdsId) { func(as, *pdsId.NodeId); });
  };

  BindableMember() = default;
  BindableMember(const T &val);
  virtual ~BindableMember() {}
};

template <typename T>
BindableMember<T>::BindableMember(const T &val) : ValueDecorator<T>(val) {}
