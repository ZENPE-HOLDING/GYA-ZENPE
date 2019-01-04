/**
 *  @file
 *  @copyright defined in gya/LICENSE.txt
 */
#pragma once

#include <gyaio/chain/types.hpp>
#include <gyaio/chain/contract_types.hpp>

namespace gyaio { namespace chain {

   class apply_context;

   /**
    * @defgroup native_action_handlers Native Action Handlers
    */
   ///@{
   void apply_gyaio_newaccount(apply_context&);
   void apply_gyaio_updateauth(apply_context&);
   void apply_gyaio_deleteauth(apply_context&);
   void apply_gyaio_linkauth(apply_context&);
   void apply_gyaio_unlinkauth(apply_context&);

   /*
   void apply_gyaio_postrecovery(apply_context&);
   void apply_gyaio_passrecovery(apply_context&);
   void apply_gyaio_vetorecovery(apply_context&);
   */

   void apply_gyaio_setcode(apply_context&);
   void apply_gyaio_setabi(apply_context&);

   void apply_gyaio_canceldelay(apply_context&);
   ///@}  end action handlers

} } /// namespace gyaio::chain
