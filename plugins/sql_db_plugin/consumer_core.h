/**
 *  @file
 *  @copyright defined in gya/LICENSE.txt
 */

#pragma once

#include <vector>

namespace gyaio {

template<typename T>
class consumer_core {
public:
    virtual ~consumer_core() {}
    virtual void consume(const std::vector<T>& elements) = 0;
};

} // namespace


