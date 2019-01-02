#pragma once 
#include <gyaiolib/varint.hpp>
#include <gyaiolib/serialize.hpp>

namespace gyaio {
   struct public_key {
      unsigned_int        type;
      std::array<char,33> data;

      friend bool operator == ( const public_key& a, const public_key& b ) {
        return std::tie(a.type,a.data) == std::tie(b.type,b.data);
      }
      friend bool operator != ( const public_key& a, const public_key& b ) {
        return std::tie(a.type,a.data) != std::tie(b.type,b.data);
      }
      GYALIB_SERIALIZE( public_key, (type)(data) )
   };
}