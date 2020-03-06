#pragma once
#include <open62541/server.h>
#include <string>
#include <type_traits>

UA_Int16 nsFromUri(UA_Server *pServer, std::string uri);

template<typename Test, template<typename...> class TemplType>
struct is_same_template : std::false_type {};

template<template<typename...> class TemplType, typename... Args>
struct is_same_template<TemplType<Args...>, TemplType>: std::true_type {};

template <class T>
struct always_false : std::false_type {};
