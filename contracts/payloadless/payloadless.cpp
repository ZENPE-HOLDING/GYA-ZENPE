#include <gyaiolib/gyaio.hpp>
#include <gyaiolib/print.hpp>
using namespace gyaio;

class payloadless : public gyaio::contract {
  public:
      using contract::contract;

      void doit() {
         print( "Im a payloadless action" );
      }
};

GYAIO_ABI( payloadless, (doit) )
