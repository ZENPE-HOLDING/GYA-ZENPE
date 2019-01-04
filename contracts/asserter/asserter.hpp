/**
 *  @file
 *  @copyright defined in gya/LICENSE.txt
 */

#include <gyaiolib/gyaio.hpp>

namespace asserter {
   struct assertdef {
      int8_t      condition;
      std::string message;

      GYALIB_SERIALIZE( assertdef, (condition)(message) )
   };
}
