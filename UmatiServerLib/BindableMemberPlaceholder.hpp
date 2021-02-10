#pragma once
#include "UnbindRefl.hpp"
#include "BindableMember.hpp"
#include <list>
#include "NodesMaster.hpp"
#include <Open62541Cpp/UA_QualifiedName.hpp>
#include "Instantiation.hpp"
#include "Util.hpp"
#include "OpcUaEvent.hpp"
#include "../TypeDefinition/ns0/GeneralModelChangeEvent.hpp"
#include <variant>
#include <iomanip>
#include "../arch/gmtime.hpp"
#include <sstream>

///\TODO enable if BINDABLEMEMBER_T is BindableMember(Value)
template <template <typename...> class BINDABLEMEMBER_T, typename T /*, typename = std::enable_if_t<is_base_of_template<BindableMember, BINDABLEMEMBER_T<T>>::value>*/>
class BindableMemberPlaceholder : public BindableMember<std::list<BINDABLEMEMBER_T<T>>>
{
public:
  template <typename ADDED_T = T>
  ADDED_T &Add(UA_Server *pServer, NodesMaster &nodesMaster, open62541Cpp::UA_QualifiedName browseName)
  {
    if (this->ParentNodeId.NodeId == nullptr || this->MemberInTypeNodeId.NodeId == nullptr)
    {
      throw std::runtime_error("Parent not bind.");
    }

    auto &newEl = this->value.emplace_back(BINDABLEMEMBER_T<T>());
    if constexpr (is_base_of_template<std::variant, T>::value)
    {
      // Set variant to correct type
      newEl.value = ADDED_T();
    }
    UA_NodeClass nodeClass = UA_NODECLASS_UNSPECIFIED;
    open62541Cpp::UA_NodeId typeDef;
    if constexpr (hasAttributeIfReflectable<UmatiServerLib::attribute::UaObjectType, ADDED_T>())
    {
      nodeClass = UA_NODECLASS_OBJECTTYPE;
      auto objTypeAttr = refl::descriptor::get_attribute<UmatiServerLib::attribute::UaObjectType>(refl::reflect<ADDED_T>());
      typeDef = objTypeAttr.NodeId.UANodeId(pServer);
    }
    else if constexpr (hasAttributeIfReflectable<UmatiServerLib::attribute::UaVariableType, ADDED_T>())
    {
      nodeClass = UA_NODECLASS_VARIABLETYPE;
    }
    else
    {
      static_assert(always_false<T>::value, "ADDED_T must have ObjectType or VariableType attribute");
    }
    auto referenceType = getReferenceTypeFromMemberNode(pServer, this->MemberInTypeNodeId, this->ParentNodeId);

    UA_StatusCode status = -1;
    switch (nodeClass)
    {
    case UA_NODECLASS_OBJECTTYPE:
    {

      UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;
      UA_String_copy(&browseName.QualifiedName->name, &objAttr.displayName.text);

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

      UA_ObjectAttributes_clear(&objAttr);
    }
    break;
    case UA_NODECLASS_VARIABLETYPE:
    {
      status = InstantiateVariable<ADDED_T>(pServer, this->MemberInTypeNodeId, browseName, referenceType, this->ParentNodeId, newEl.NodeId);
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
      ss << "Could not create placeholder node, Error: " << UA_StatusCode_name(status);
      throw std::runtime_error(ss.str());
    }

    UmatiServerLib::Bind::MemberRefl(newEl.value, pServer, newEl.NodeId, nodesMaster);
    newEl.SetBind();

    sendGeneralModelChangeEvent(pServer, 0x04);
    if constexpr (is_base_of_template<std::variant, T>::value)
    {
      return std::get<ADDED_T>(newEl.value);
    }
    else
    {
      return newEl.value;
    }
  }

  typename std::list<BINDABLEMEMBER_T<T>>::iterator Delete(typename std::list<BINDABLEMEMBER_T<T>>::iterator it, UA_Server *pServer, NodesMaster &nodesMaster)
  {
    BindableMember<T> &el = *it;
    if (!el.IsBind())
    {
      std::cout << "Element not bind" << std::endl;
    }
    else
    {
      UmatiServerLib::Bind::MemberRefl(el, pServer, el.NodeId, nodesMaster);
      UA_Server_deleteNode(pServer, *el.NodeId.NodeId, true);
    }

    sendGeneralModelChangeEvent(pServer, 0x08);
    return this->value.erase(it);
  }

  void sendGeneralModelChangeEvent(UA_Server *pServer, UA_Byte verb)
  {
    auto typeDefinition = readTypeDefinition(pServer, this->ParentNodeId);
    UA_Byte evNotifier = 0;
    auto status = UA_Server_readEventNotifier(pServer, *this->ParentNodeId.NodeId, &evNotifier);
    ///\todo also check if type should emit these events
    if(status == UA_STATUSCODE_GOOD && (evNotifier & UA_EVENTNOTIFIERTYPE_SUBSCRIBETOEVENTS))
    {
      ///\todo else create event on Server?
      ns0::GeneralModelChangeEvent_t evModChange;
      UA_ModelChangeStructureDataType &change = evModChange.Changes->emplace_back();
      change.affected = *this->ParentNodeId.NodeId;
      change.affectedType = *typeDefinition.NodeId;
      change.verb = verb;
      evModChange.Severity = 0;
      OpcUaEvent ev(evModChange, pServer, open62541Cpp::UA_NodeId(this->ParentNodeId));
    }
    updateNodeVersion(pServer);
  }

  void updateNodeVersion(UA_Server *pServer)
  {
    UA_Variant outVar;
    UA_Variant_init(&outVar);
    auto status = UA_Server_readObjectProperty(
        pServer,
        *this->ParentNodeId.NodeId,
        *open62541Cpp::UA_QualifiedName(0, "NodeVersion").QualifiedName,
        &outVar);
    if(status == UA_STATUSCODE_BADNOMATCH)
    {
      // NodeVersion not available
      return;
    }

    if (status != UA_STATUSCODE_GOOD)
    {
      std::cout << "Could not update NodeVerison " << UA_StatusCode_name(status) << std::endl;
      return;
    }

    if (UA_Variant_isEmpty(&outVar) || !UA_Variant_isScalar(&outVar) || outVar.type != &UA_TYPES[UA_TYPES_STRING])
    {
      // Initialize/ Reset to string
      UA_Server_writeObjectProperty_scalar(
          pServer,
          *this->ParentNodeId.NodeId,
          *open62541Cpp::UA_QualifiedName(0, "NodeVersion").QualifiedName,
          &UA_STRING_NULL,
          &UA_TYPES[UA_TYPES_STRING]);
    }
    else
    {
      open62541Cpp::UA_String uaStr(reinterpret_cast<UA_String *>(outVar.data), false);
      auto str = static_cast<std::string>(uaStr);
      bool changed = false;
      /**
      for (int i = str.size() - 1; i >= 0; --i)
      {
        if (str[i] < 'a' || str[i] > 'z')
        {
          str[i] = 'a';
          changed = true;
          break;
        }
        else if (str[i] < 'z')
        {
          ++str[i];
          changed = true;
          break;
        }
        else //is 'z'
        {
          str[i] = 'a';
        }
      }

      if (!changed)
      {
        std::stringstream ss;
        ss << "a" << str;
        str = ss.str();
      }
      **/

      auto t = std::time(nullptr);
      std::tm tm;
      std::memset(&tm, 0, sizeof(tm));
      UMATI_GMTIME(&t, &tm);

      std::ostringstream oss;
      oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
      str = oss.str();

      open62541Cpp::UA_String newUaStr(str);
      UA_Server_writeObjectProperty_scalar(
          pServer,
          *this->ParentNodeId.NodeId,
          *open62541Cpp::UA_QualifiedName(0, "NodeVersion").QualifiedName,
          newUaStr.String,
          &UA_TYPES[UA_TYPES_STRING]);
    }

    UA_Variant_clear(&outVar);
  }
};
