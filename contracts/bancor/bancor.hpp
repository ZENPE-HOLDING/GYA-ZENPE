/**
 *  @file
 *  @copyright defined in gya/LICENSE.txt
 */
#pragma once

#include <gyaiolib/gyaio.hpp>
#include <gyaiolib/token.hpp>
#include <gyaiolib/reflect.hpp>
#include <gyaiolib/generic_currency.hpp>

#include <bancor/converter.hpp>
#include <currency/currency.hpp>

namespace bancor {
   typedef gyaio::generic_currency< gyaio::token<N(other),S(4,OTHER)> >  other_currency;
   typedef gyaio::generic_currency< gyaio::token<N(bancor),S(4,RELAY)> > relay_currency;
   typedef gyaio::generic_currency< gyaio::token<N(currency),S(4,CUR)> > cur_currency;

   typedef converter<relay_currency, other_currency, cur_currency > example_converter;
} /// bancor

