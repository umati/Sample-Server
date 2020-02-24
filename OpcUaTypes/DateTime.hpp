/**
 * @file DateTime.hpp
 * @author Christian von Arnim christian.von-arnim@isw.uni-stuttgart.de
 * @date 2020-02-21
 * @copyright Copyright (c) 2020
 */

#include <chrono>

namespace open62541Cpp{
    typedef std::chrono::time_point<std::chrono::system_clock> DateTime_t;

}
