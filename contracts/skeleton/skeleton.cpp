#include <gyaiolib/gyaio.hpp>

using namespace gyaio;

class hello : public gyaio::contract {
  public:
      using contract::contract;

      /// @abi action 
      void hi( account_name user ) {
         print( "Hello, ", name{user} );
      }
};

GYAIO_ABI( hello, (hi) )
