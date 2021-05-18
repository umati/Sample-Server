#pragma once
#include <open62541/server.h>

#include <Open62541Cpp/UA_NodeId.hpp>
#include <list>
#include <refl.hpp>
#include <string>
#include <type_traits>

UA_Int16 nsFromUri(UA_Server *pServer, std::string uri);

// Check if the type is a subtype of another type
bool isSubtypeOf(UA_Server *pServer, const open62541Cpp::UA_NodeId &typeNodeId, const open62541Cpp::UA_NodeId &baseNodeId);

// Check if a type is a specific template type
template <typename Test, template <typename...> class TemplType>
struct is_same_template : std::false_type {};

template <template <typename...> class TemplType, typename... Args>
struct is_same_template<TemplType<Args...>, TemplType> : std::true_type {};

// Check if a type is a specific template type or subtype
// See https://stackoverflow.com/a/34672753
template <template <typename...> class base, typename derived>
struct is_base_of_template_impl {
  template <typename... Ts>
  static constexpr std::true_type test(const base<Ts...> *);
  static constexpr std::false_type test(...);
  using type = decltype(test(std::declval<derived *>()));
};

template <template <typename...> class base, typename derived>
using is_base_of_template = typename is_base_of_template_impl<base, derived>::type;

template <class T>
struct always_false : std::false_type {};

// Custom Bases oimplementation for refl-cpp, can be replaced by the new refl::bases from the library,
// but will also affect some other code parts as memers work also as inherited
template <typename... Ts>
struct Bases : public refl::attr::usage::type {
  static constexpr refl::type_list<refl::type_descriptor<Ts>...> descriptors = {};
};

// Get the parent node, ba reverse browsing hierarchical reference
open62541Cpp::UA_NodeId browseForParent(open62541Cpp::UA_NodeId node, open62541Cpp::UA_NodeId referenceType, UA_Server *pServer);

// Only exact match for the object type are handled, \todo allow to include subtypes
std::list<open62541Cpp::UA_NodeId> browseForChilds(
  UA_Server *pServer, open62541Cpp::UA_NodeId node, open62541Cpp::UA_NodeId referenceType, open62541Cpp::UA_NodeId typeDef);

// Check if a type is a specific type or one of it's subtypes
bool isTypeOrSubtype(UA_Server *pServer, const open62541Cpp::UA_NodeId &baseType, const open62541Cpp::UA_NodeId &checkType);
