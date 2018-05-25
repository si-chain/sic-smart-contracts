/**
 *  @file auditor.cpp
 *  @copyright defined in eos/LICENSE.txt
 *  @auther: hanfei
 *  @createtime: 2018-04-23
 */

#include <sic.role.hpp>

using std::string;

namespace sic {

/// @abi action
void sic_role::setrole(account_name user, uint8_t type, account_name admin) {
    eosio_assert(user > 0 , "The role name can not be null!"); 
    eosio_assert(type > 0 , "you can't set the admin!"); 
    auto adminitr = admins.find(admin);
    eosio_assert(adminitr != admins.end(), "the permission is not push by admin!");
    require_auth(admin);

    auto itr = roles.find(user);
    if(itr == roles.end()) {
        roles.emplace(admin, [&]( auto& g ) {
            g.role = user;
            g.role_type = type;
            g.set_time = ::now();
        });
    } else {
        roles.modify( itr, admin, [&]( auto& s ) {
            s.role_type = type;
            s.set_time = ::now();
        });
    }
    
}

/// @abi action 
void sic_role::delrole(account_name user, account_name admin) {
    eosio_assert(user > 0 , "The role name can not be null!"); 
    auto adminitr = admins.find(admin);
    eosio_assert(adminitr != admins.end(), "the permission is not push by admin!");
    require_auth(admin);
    auto itr = roles.find(user);
    eosio_assert(itr != roles.end(), "This auditor do not exist!");
    roles.erase(itr);
}

/// @abi action 
void sic_role::setadmin(account_name admin) {
    eosio_assert(admin > 0 , "The admin name can not be null!"); 
    require_auth(_self);

    auto itr = admins.find(admin);  
    eosio_assert(itr == admins.end(), "This admin have existed!");  
    admins.emplace(_self, [&]( auto& g ) {
        g.admin = admin;
    });

    auto itrr = roles.find(admin);
    if(itrr == roles.end()) {
        roles.emplace(_self, [&]( auto& g ) {
            g.role = admin;
            g.role_type = 0;
            g.set_time = ::now();
        });
    } else {
        roles.modify( itrr, _self, [&]( auto& s ) {
            s.role_type = 0;
            s.set_time = ::now();
        });
    }
}

/// @abi action 
void sic_role::deladmin(account_name admin) {
    eosio_assert(admin > 0 , "The admin name can not be null!"); 
    require_auth(_self);

    auto itr = admins.find(admin); 
    eosio_assert(itr != admins.end(), "This admin do not exist!"); 
    admins.erase(itr);

    auto itrr = roles.find(admin);
    eosio_assert(itrr != roles.end(), "This auditor do not exist!");
    roles.erase(itrr);
}


EOSIO_ABI( sic_role, (setrole)(delrole)(setadmin)(deladmin) )

} // namespace sic