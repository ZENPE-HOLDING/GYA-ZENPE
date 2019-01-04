#pragma once

#include <gyaiolib/gyaio.hpp>

namespace gyaio {

   class sudo : public contract {
      public:
         sudo( account_name self ):contract(self){}

         void exec();

   };

} /// namespace gyaio
