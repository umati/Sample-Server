#pragma once

#include "ValueDecorator.hpp"
#include <optional>

template<typename T>
class Variable : public ValueDecorator<T>
{
    public:
    Variable() = default;
    Variable(const T& val);
    UA_StatusCode StatusCode = UA_STATUSCODE_GOOD;
    std::optional<UA_DateTime> SourceTimestamp;
    std::optional<UA_DateTime> ServerTimestamp;
};

template<typename T>
Variable<T>::Variable(const T& val) : ValueDecorator<T>(val){}
