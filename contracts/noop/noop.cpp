/**
 *  @file
 *  @copyright defined in gya/LICENSE.txt
 */

#include <gyaiolib/gyaio.hpp>

namespace gyaio {

   class noop: public contract {
      public:
         noop( account_name self ): contract( self ) { }
         void anyaction( account_name from,
                         const std::string& /*type*/,
                         const std::string& /*data*/ )
         {
            require_auth( from );
         }
   };

   GYAIO_ABI( noop, ( anyaction ) )

} /// gyaio     
