/**
 *  @file
 *  @copyright defined in gya/LICENSE.txt
 */

#include <gyaio/chain/chain_id_type.hpp>
#include <gyaio/chain/exceptions.hpp>

namespace gyaio { namespace chain {

   void chain_id_type::reflector_verify()const {
      FC_ASSERT( *reinterpret_cast<const fc::sha256*>(this) != fc::sha256(), "chain_id_type cannot be zero" );
   }

} }  // namespace gyaio::chain

namespace fc {

   void to_variant(const gyaio::chain::chain_id_type& cid, fc::variant& v) {
      to_variant( static_cast<const fc::sha256&>(cid), v);
   }

   void from_variant(const fc::variant& v, gyaio::chain::chain_id_type& cid) {
      from_variant( v, static_cast<fc::sha256&>(cid) );
   }

} // fc
