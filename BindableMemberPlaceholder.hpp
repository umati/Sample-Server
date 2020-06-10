#pragma once

#include "BindableMember.hpp"
#include <list>
#include "NodesMaster.hpp"
#include <Open62541Cpp/UA_QualifiedName.hpp>
#include "Instantiation.hpp"

///\TODO enable if BINDABLEMEMBER_T is BindableMember(Value)
template <template <typename...> class BINDABLEMEMBER_T, typename T /*, typename = std::enable_if_t<is_base_of_template<BindableMember, BINDABLEMEMBER_T<T>>::value>*/>
class BindableMemberPlaceholder : public BindableMember<std::list<BINDABLEMEMBER_T<T>>>
{
public:
  template <typename ADDED_T = T>
  ADDED_T &Add(UA_Server *pServer, NodesMaster &nodesMaster, open62541Cpp::UA_QualifiedName browseName)
  {
    if (this->ParentNodeId.NodeId == nullptr || this->MemerInTypeNodeId.NodeId == nullptr)
    {
      throw std::runtime_error("Parent not bind.");
    }
    auto &newEl = this->value.emplace_back(BINDABLEMEMBER_T<ADDED_T>());

    UA_NodeClass nodeClass = UA_NODECLASS_UNSPECIFIED;
    open62541Cpp::UA_NodeId typeDef;
    if constexpr (hasAttributeIfReflectable<open62541Cpp::attribute::UaObjectType, T>())
    {
      nodeClass = UA_NODECLASS_OBJECTTYPE;
      auto objTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<T>());
      typeDef = objTypeAttr.NodeId.UANodeId(pServer);
    }
    else if constexpr (hasAttributeIfReflectable<open62541Cpp::attribute::UaObjectType, T>())
    {
      nodeClass = UA_NODECLASS_VARIABLETYPE;
      auto varTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaVariableType>(refl::reflect<T>());
      typeDef = varTypeAttr.NodeId.UANodeId(pServer);
    }
    else
    {
      static_assert(always_false<T>::value, "ADDED_T must be an ObjectType or VariableType");
    }
    auto referenceType = getReferenceTypeFromMemberNode(pServer, this->MemerInTypeNodeId, this->ParentNodeId);
    
    UA_StatusCode status = -1;
    switch (nodeClass)
    {
    case UA_NODECLASS_OBJECTTYPE:
    {

      UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;
      UA_String_copy(&browseName.QualifiedName->name, &objAttr.displayName.text);
      {
        status = UA_Server_addObjectNode(
            pServer,
            UA_NODEID_NUMERIC(browseName.QualifiedName->namespaceIndex, 0),
            *this->ParentNodeId.NodeId,
            *referenceType.NodeId,
            *browseName.QualifiedName,
            *typeDef.NodeId,
            objAttr,
            nullptr,
            newEl.NodeId.NodeId);
      }
      UA_ObjectAttributes_clear(&objAttr);
    }
    break;
    case UA_NODECLASS_VARIABLETYPE:
    {
      UA_VariableAttributes varAttr = UA_VariableAttributes_default;
      UA_String_copy(&browseName.QualifiedName->name, &varAttr.displayName.text);
      {
        status = UA_Server_addVariableNode(
            pServer,
            UA_NODEID_NUMERIC(browseName.QualifiedName->namespaceIndex, 0),
            *this->ParentNodeId.NodeId,
            *referenceType.NodeId,
            *browseName.QualifiedName,
            *typeDef.NodeId,
            varAttr,
            nullptr,
            newEl.NodeId.NodeId);
      }
      UA_VariableAttributes_clear(&varAttr);
    }
    break;
    default:
    {
      throw std::runtime_error("Unexpected NodeClass.");
    }
    break;
    }

    if (status != UA_STATUSCODE_GOOD)
    {
      std::stringstream ss;
      ss << "Could create node, Error: " << UA_StatusCode_name(status);
      throw std::runtime_error(ss.str());
    }

    bindMemberRefl(newEl.value, pServer, newEl.NodeId, nodesMaster);
    newEl.SetBind();
    ///\todo trigger ModelChangeEvent

    return newEl.value;
  }

  typename std::list<BINDABLEMEMBER_T<T>>::iterator Delete(typename std::list<BINDABLEMEMBER_T<T>>::iterator it, UA_Server* pServer, NodesMaster &nodesMaster)
  {
    BindableMember<T>& el = *it;
    if(!el.IsBind())
    {
      std::cout << "Element not bind" << std::endl;
    }
    else
    {
      unbindMemberRefl(el, pServer, el.NodeId, nodesMaster);
      UA_Server_deleteNode(pServer, *el.NodeId.NodeId, true);
    }

    ///\todo trigger ModelChangeEvent
    return this->value.erase(it);
  }
};
