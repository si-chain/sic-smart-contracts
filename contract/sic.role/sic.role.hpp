/**
 *  @file auditor.cpp
 *  @copyright defined in eos/LICENSE.txt
 *  @auther: hanfei
 *  @createtime: 2018-04-23
 */

#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/contract.hpp>

using std::string;

namespace sic {

class sic_role : public eosio::contract {
    public:
        using contract::contract;

        sic_role(account_name self)
            :eosio::contract(self),
            admins(_self, _self),
            roles(_self, _self)
            {}

        /// @abi action
        void setrole(account_name user, uint8_t type, account_name admin);

        /// @abi action 
        void delrole(account_name user, account_name admin);


        /// @abi action 
        void setadmin(account_name admin);

        /// @abi action 
        void deladmin(account_name admin);


        /// @abi table rolelist i64
        struct rolelist {
            account_name    role;
            uint8_t         role_type;
            time            set_time;
            
            uint64_t primary_key()const { return role; }
            EOSLIB_SERIALIZE( rolelist, (role)(role_type)(set_time) )
        };

        typedef eosio::multi_index< N(rolelist), rolelist> rolelist_index;
        rolelist_index roles;

    private:

        /// @abi table adminlist i64
        struct adminlist {
            account_name    admin;

            uint64_t primary_key()const { return admin; }
            EOSLIB_SERIALIZE( adminlist, (admin) )
        };
        typedef eosio::multi_index< N(adminlist), adminlist> adminlist_index;

        adminlist_index admins;        

};


} //namespace sic