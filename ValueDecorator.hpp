#pragma once

template<typename T>
class ValueDecorator
{
public:
	ValueDecorator() = default;
	ValueDecorator(const T& val):value(val){}

	template<typename U>
	ValueDecorator<T>& operator=(const U& other){
		value=other;
		return *this;
	}

	T& operator*()
	{
		return value;
	}

	T* operator->()
	{
		return &value;
	}

	T value;
};
