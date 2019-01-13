/**
 *  @file
 *  @copyright defined in gya/LICENSE.txt
 */
#pragma once
#include <fc/exception/exception.hpp>

namespace gyaio { namespace client { namespace help {
   bool print_recognized_errors(const fc::exception& e, const bool verbose_errors);
   bool print_help_text(const fc::exception& e);
}}}