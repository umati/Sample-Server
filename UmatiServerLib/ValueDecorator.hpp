#pragma once

template <typename T>
class ValueDecorator {
 public:
  ValueDecorator() = default;
  ValueDecorator(const T& val) : value(val) {}

  T& operator*() { return value; }

  T* operator->() { return &value; }

  T value;
};
