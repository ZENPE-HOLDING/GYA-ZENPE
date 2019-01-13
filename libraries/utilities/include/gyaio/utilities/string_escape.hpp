/**
 *  @file
 *  @copyright defined in gya/LICENSE.txt
 */
#pragma once

#include <string>

namespace gyaio { namespace utilities {

  std::string escape_string_for_c_source_code(const std::string& input);

} } // end namespace gyaio::utilities
