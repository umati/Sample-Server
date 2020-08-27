#pragma once
#include <open62541/server.h>
#include <string>
#include <type_traits>
#include <refl.hpp>
#include <Open62541Cpp/UA_NodeId.hpp>

UA_Int16 nsFromUri(UA_Server *pServer, std::string uri);

bool isSubtypeOf(UA_Server *pServer, const open62541Cpp::UA_NodeId &typeNodeId, const open62541Cpp::UA_NodeId &baseNodeId);

template<typename Test, template<typename...> class TemplType>
struct is_same_template : std::false_type {};

template<template<typename...> class TemplType, typename... Args>
struct is_same_template<TemplType<Args...>, TemplType>: std::true_type {};

// See https://stackoverflow.com/a/34672753
template < template <typename...> class base,typename derived>
struct is_base_of_template_impl
{
    template<typename... Ts>
    static constexpr std::true_type  test(const base<Ts...> *);
    static constexpr std::false_type test(...);
    using type = decltype(test(std::declval<derived*>()));
};

template < template <typename...> class base,typename derived>
using is_base_of_template = typename is_base_of_template_impl<base,derived>::type;


template <class T>
struct always_false : std::false_type {};

template <typename... Ts>
struct Bases : public refl::attr::usage::type {
    static constexpr refl::type_list<refl::type_descriptor<Ts>...> descriptors = {};
};

open62541Cpp::UA_NodeId browseForParent(open62541Cpp::UA_NodeId node, open62541Cpp::UA_NodeId referenceType, UA_Server *pServer);
