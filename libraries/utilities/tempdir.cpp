/**
 *  @file
 *  @copyright defined in gya/LICENSE.txt
 */

#include <gyaio/utilities/tempdir.hpp>

#include <cstdlib>

namespace gyaio { namespace utilities {

fc::path temp_directory_path()
{
   const char* gya_tempdir = getenv("GYA_TEMPDIR");
   if( gya_tempdir != nullptr )
      return fc::path( gya_tempdir );
   return fc::temp_directory_path() / "gya-tmp";
}

} } // gyaio::utilities
