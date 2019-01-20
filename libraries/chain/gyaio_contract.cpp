/**
 *  @file
 *  @copyright defined in gya/LICENSE.txt
 */
#include <gyaio/chain/gyaio_contract.hpp>
#include <gyaio/chain/contract_table_objects.hpp>

#include <gyaio/chain/controller.hpp>
#include <gyaio/chain/transaction_context.hpp>
#include <gyaio/chain/apply_context.hpp>
#include <gyaio/chain/transaction.hpp>
#include <gyaio/chain/exceptions.hpp>

#include <gyaio/chain/account_object.hpp>
#include <gyaio/chain/permission_object.hpp>
#include <gyaio/chain/permission_link_object.hpp>
#include <gyaio/chain/global_property_object.hpp>
#include <gyaio/chain/contract_types.hpp>
#include <gyaio/chain/producer_object.hpp>

#include <gyaio/chain/wasm_interface.hpp>
#include <gyaio/chain/abi_serializer.hpp>

#include <gyaio/chain/authorization_manager.hpp>
#include <gyaio/chain/resource_limits.hpp>

namespace gyaio { namespace chain {



uint128_t transaction_id_to_sender_id( const transaction_id_type& tid ) {
   fc::uint128_t _id(tid._hash[3], tid._hash[2]);
   return (unsigned __int128)_id;
}

void validate_authority_precondition( const apply_context& context, const authority& auth ) {
   for(const auto& a : auth.accounts) {
      auto* acct = context.db.find<account_object, by_name>(a.permission.actor);
      GYA_ASSERT( acct != nullptr, action_validate_exception,
                  "account '${account}' does not exist",
                  ("account", a.permission.actor)
                );

      if( a.permission.permission == config::owner_name || a.permission.permission == config::active_name )
         continue; // account was already checked to exist, so its owner and active permissions should exist

      if( a.permission.permission == config::gyaio_code_name ) // virtual gyaio.code permission does not really exist but is allowed
         continue;

      try {
         context.control.get_authorization_manager().get_permission({a.permission.actor, a.permission.permission});
      } catch( const permission_query_exception& ) {
         GYA_THROW( action_validate_exception,
                    "permission '${perm}' does not exist",
                    ("perm", a.permission)
                  );
      }
   }

   if( context.control.is_producing_block() ) {
      for( const auto& p : auth.keys ) {
         context.control.check_key_list( p.key );
      }
   }
}

/**
 *  This method is called assuming precondition_system_newaccount succeeds a
 */
void apply_gyaio_newaccount(apply_context& context) {
   auto create = context.act.data_as<newaccount>();
   try {
   context.require_authorization(create.creator);
//   context.require_write_lock( config::gyaio_auth_scope );
   auto& authorization = context.control.get_mutable_authorization_manager();

   GYA_ASSERT( validate(create.owner), action_validate_exception, "Invalid owner authority");
   GYA_ASSERT( validate(create.active), action_validate_exception, "Invalid active authority");

   auto& db = context.db;

   auto name_str = name(create.name).to_string();

   GYA_ASSERT( !create.name.empty(), action_validate_exception, "account name cannot be empty" );
   GYA_ASSERT( name_str.size() <= 12, action_validate_exception, "account names can only be 12 chars long" );

   // Check if the creator is privileged
   const auto &creator = db.get<account_object, by_name>(create.creator);
   if( !creator.privileged ) {
      GYA_ASSERT( name_str.find( "gyaio." ) != 0, action_validate_exception,
                  "only privileged accounts can have names that start with 'gyaio.'" );
   }

   auto existing_account = db.find<account_object, by_name>(create.name);
   GYA_ASSERT(existing_account == nullptr, account_name_exists_exception,
              "Cannot create account named ${name}, as that name is already taken",
              ("name", create.name));

   const auto& new_account = db.create<account_object>([&](auto& a) {
      a.name = create.name;
      a.creation_date = context.control.pending_block_time();
   });

   db.create<account_sequence_object>([&](auto& a) {
      a.name = create.name;
   });

   for( const auto& auth : { create.owner, create.active } ){
      validate_authority_precondition( context, auth );
   }

   const auto& owner_permission  = authorization.create_permission( create.name, config::owner_name, 0,
                                                                    std::move(create.owner) );
   const auto& active_permission = authorization.create_permission( create.name, config::active_name, owner_permission.id,
                                                                    std::move(create.active) );

   context.control.get_mutable_resource_limits_manager().initialize_account(create.name);

   int64_t ram_delta = config::overhead_per_account_ram_bytes;
   ram_delta += 2*config::billable_size_v<permission_object>;
   ram_delta += owner_permission.auth.get_billable_size();
   ram_delta += active_permission.auth.get_billable_size();

   context.add_ram_usage(create.name, ram_delta);

} FC_CAPTURE_AND_RETHROW( (create) ) }

void apply_gyaio_setcode(apply_context& context) {
   const auto& cfg = context.control.get_global_properties().configuration;

   auto& db = context.db;
   auto  act = context.act.data_as<setcode>();
   context.require_authorization(act.account);

   GYA_ASSERT( act.vmtype == 0, invalid_contract_vm_type, "code should be 0" );
   GYA_ASSERT( act.vmversion == 0, invalid_contract_vm_version, "version should be 0" );

   fc::sha256 code_id; /// default ID == 0

   if( act.code.size() > 0 ) {
     code_id = fc::sha256::hash( act.code.data(), (uint32_t)act.code.size() );
     wasm_interface::validate(context.control, act.code);
   }

   const auto& account = db.get<account_object,by_name>(act.account);

   int64_t code_size = (int64_t)act.code.size();
   int64_t old_size  = (int64_t)account.code.size() * config::setcode_ram_bytes_multiplier;
   int64_t new_size  = code_size * config::setcode_ram_bytes_multiplier;

   GYA_ASSERT( account.code_version != code_id, set_exact_code, "contract is already running this version of code" );

   db.modify( account, [&]( auto& a ) {
      /** TODO: consider whether a microsecond level local timestamp is sufficient to detect code version changes*/
      // TODO: update setcode message to include the hash, then validate it in validate
      a.last_code_update = context.control.pending_block_time();
      a.code_version = code_id;
      a.code.resize( code_size );
      if( code_size > 0 )
         memcpy( a.code.data(), act.code.data(), code_size );

   });

   const auto& account_sequence = db.get<account_sequence_object, by_name>(act.account);
   db.modify( account_sequence, [&]( auto& aso ) {
      aso.code_sequence += 1;
   });

   if (new_size != old_size) {
      context.add_ram_usage( act.account, new_size - old_size );
   }
}

void apply_gyaio_setabi(apply_context& context) {
   auto& db  = context.db;
   auto  act = context.act.data_as<setabi>();

   context.require_authorization(act.account);

   const auto& account = db.get<account_object,by_name>(act.account);

   int64_t abi_size = act.abi.size();

   int64_t old_size = (int64_t)account.abi.size();
   int64_t new_size = abi_size;

   db.modify( account, [&]( auto& a ) {
      a.abi.resize( abi_size );
      if( abi_size > 0 )
         memcpy( a.abi.data(), act.abi.data(), abi_size );
   });

   const auto& account_sequence = db.get<account_sequence_object, by_name>(act.account);
   db.modify( account_sequence, [&]( auto& aso ) {
      aso.abi_sequence += 1;
   });

   if (new_size != old_size) {
      context.add_ram_usage( act.account, new_size - old_size );
   }
}

void apply_gyaio_updateauth(apply_context& context) {

   auto update = context.act.data_as<updateauth>();
   context.require_authorization(update.account); // only here to mark the single authority on this action as used

   auto& authorization = context.control.get_mutable_authorization_manager();
   auto& db = context.db;

   GYA_ASSERT(!update.permission.empty(), action_validate_exception, "Cannot create authority with empty name");
   GYA_ASSERT( update.permission.to_string().find( "gyaio." ) != 0, action_validate_exception,
               "Permission names that start with 'gyaio.' are reserved" );
   GYA_ASSERT(update.permission != update.parent, action_validate_exception, "Cannot set an authority as its own parent");
   db.get<account_object, by_name>(update.account);
   GYA_ASSERT(validate(update.auth), action_validate_exception,
              "Invalid authority: ${auth}", ("auth", update.auth));
   if( update.permission == config::active_name )
      GYA_ASSERT(update.parent == config::owner_name, action_validate_exception, "Cannot change active authority's parent from owner", ("update.parent", update.parent) );
   if (update.permission == config::owner_name)
      GYA_ASSERT(update.parent.empty(), action_validate_exception, "Cannot change owner authority's parent");
   else
      GYA_ASSERT(!update.parent.empty(), action_validate_exception, "Only owner permission can have empty parent" );

   if( update.auth.waits.size() > 0 ) {
      auto max_delay = context.control.get_global_properties().configuration.max_transaction_delay;
      GYA_ASSERT( update.auth.waits.back().wait_sec <= max_delay, action_validate_exception,
                  "Cannot set delay longer than max_transacton_delay, which is ${max_delay} seconds",
                  ("max_delay", max_delay) );
   }

   validate_authority_precondition(context, update.auth);



   auto permission = authorization.find_permission({update.account, update.permission});

   // If a parent_id of 0 is going to be used to indicate the absence of a parent, then we need to make sure that the chain
   // initializes permission_index with a dummy object that reserves the id of 0.
   authorization_manager::permission_id_type parent_id = 0;
   if( update.permission != config::owner_name ) {
      auto& parent = authorization.get_permission({update.account, update.parent});
      parent_id = parent.id;
   }

   if( permission ) {
      GYA_ASSERT(parent_id == permission->parent, action_validate_exception,
                 "Changing parent authority is not currently supported");


      int64_t old_size = (int64_t)(config::billable_size_v<permission_object> + permission->auth.get_billable_size());

      authorization.modify_permission( *permission, update.auth );

      int64_t new_size = (int64_t)(config::billable_size_v<permission_object> + permission->auth.get_billable_size());

      context.add_ram_usage( permission->owner, new_size - old_size );
   } else {
      const auto& p = authorization.create_permission( update.account, update.permission, parent_id, update.auth );

      int64_t new_size = (int64_t)(config::billable_size_v<permission_object> + p.auth.get_billable_size());

      context.add_ram_usage( update.account, new_size );
   }
}

void apply_gyaio_deleteauth(apply_context& context) {
//   context.require_write_lock( config::gyaio_auth_scope );

   auto remove = context.act.data_as<deleteauth>();
   context.require_authorization(remove.account); // only here to mark the single authority on this action as used

   GYA_ASSERT(remove.permission != config::active_name, action_validate_exception, "Cannot delete active authority");
   GYA_ASSERT(remove.permission != config::owner_name, action_validate_exception, "Cannot delete owner authority");

   auto& authorization = context.control.get_mutable_authorization_manager();
   auto& db = context.db;



   { // Check for links to this permission
      const auto& index = db.get_index<permission_link_index, by_permission_name>();
      auto range = index.equal_range(boost::make_tuple(remove.account, remove.permission));
      GYA_ASSERT(range.first == range.second, action_validate_exception,
                 "Cannot delete a linked authority. Unlink the authority first. This authority is linked to ${code}::${type}.", 
                 ("code", string(range.first->code))("type", string(range.first->message_type)));
   }

   const auto& permission = authorization.get_permission({remove.account, remove.permission});
   int64_t old_size = config::billable_size_v<permission_object> + permission.auth.get_billable_size();

   authorization.remove_permission( permission );

   context.add_ram_usage( remove.account, -old_size );

}

void apply_gyaio_linkauth(apply_context& context) {
//   context.require_write_lock( config::gyaio_auth_scope );

   auto requirement = context.act.data_as<linkauth>();
   try {
      GYA_ASSERT(!requirement.requirement.empty(), action_validate_exception, "Required permission cannot be empty");

      context.require_authorization(requirement.account); // only here to mark the single authority on this action as used

      auto& db = context.db;
      const auto *account = db.find<account_object, by_name>(requirement.account);
      GYA_ASSERT(account != nullptr, account_query_exception,
                 "Failed to retrieve account: ${account}", ("account", requirement.account)); // Redundant?
      const auto *code = db.find<account_object, by_name>(requirement.code);
      GYA_ASSERT(code != nullptr, account_query_exception,
                 "Failed to retrieve code for account: ${account}", ("account", requirement.code));
      if( requirement.requirement != config::gyaio_any_name ) {
         const auto *permission = db.find<permission_object, by_name>(requirement.requirement);
         GYA_ASSERT(permission != nullptr, permission_query_exception,
                    "Failed to retrieve permission: ${permission}", ("permission", requirement.requirement));
      }

      auto link_key = boost::make_tuple(requirement.account, requirement.code, requirement.type);
      auto link = db.find<permission_link_object, by_action_name>(link_key);

      if( link ) {
         GYA_ASSERT(link->required_permission != requirement.requirement, action_validate_exception,
                    "Attempting to update required authority, but new requirement is same as old");
         db.modify(*link, [requirement = requirement.requirement](permission_link_object& link) {
             link.required_permission = requirement;
         });
      } else {
         const auto& l =  db.create<permission_link_object>([&requirement](permission_link_object& link) {
            link.account = requirement.account;
            link.code = requirement.code;
            link.message_type = requirement.type;
            link.required_permission = requirement.requirement;
         });

         context.add_ram_usage(
            l.account,
            (int64_t)(config::billable_size_v<permission_link_object>)
         );
      }

  } FC_CAPTURE_AND_RETHROW((requirement))
}

void apply_gyaio_unlinkauth(apply_context& context) {
//   context.require_write_lock( config::gyaio_auth_scope );

   auto& db = context.db;
   auto unlink = context.act.data_as<unlinkauth>();

   context.require_authorization(unlink.account); // only here to mark the single authority on this action as used

   auto link_key = boost::make_tuple(unlink.account, unlink.code, unlink.type);
   auto link = db.find<permission_link_object, by_action_name>(link_key);
   GYA_ASSERT(link != nullptr, action_validate_exception, "Attempting to unlink authority, but no link found");
   context.add_ram_usage(
      link->account,
      -(int64_t)(config::billable_size_v<permission_link_object>)
   );

   db.remove(*link);
}

void apply_gyaio_canceldelay(apply_context& context) {
   auto cancel = context.act.data_as<canceldelay>();
   context.require_authorization(cancel.canceling_auth.actor); // only here to mark the single authority on this action as used

   const auto& trx_id = cancel.trx_id;

   context.cancel_deferred_transaction(transaction_id_to_sender_id(trx_id), account_name());
}

} } // namespace gyaio::chain