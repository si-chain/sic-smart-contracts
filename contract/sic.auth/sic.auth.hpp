/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 *  @auther: hanfei
 *  @createtime: 2018-05-04
 */

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/public_key.hpp>

using eosio::indexed_by;
using eosio::const_mem_fun;
using std::string;

namespace sic {

class sic_auth : public eosio::contract {
    public:
        using contract::contract;
        sic_auth(account_name self)
            :eosio::contract(self)
            {}

        /// @abi action
        void reqauth(const uint64_t id,
                    const uint8_t  type,
                    const account_name user, 
                    const account_name requester,
                    const public_key req_pubkey, 
                    const uint32_t howlong,
                    std::string auth_value);

        /// @abi action
        void agreeauth(const uint64_t id, account_name user, std::string value);

        /// @abi action
        void rejectauth(const uint64_t id, account_name user);

        /// @abi action
        void burnauth(const uint64_t id, account_name user);

    private:
        enum Status {
            s_unauth=0,
            s_haveauth=10,
            s_reject=20,
            s_burned=30
        };

        enum DateType {
            Policy=0,
            Claim=1
        };

        //@abi table authlist i64
        struct authlist {
            uint64_t        authID;
            uint8_t         type;
            account_name    requester;
            public_key      req_pubkey;
            uint8_t         status = 0;
            time            req_time;
            time            agree_time;
            uint32_t        howlong;
            time            limit_time;
            std::string     auth_value;

            uint64_t primary_key()const { return authID; }

            EOSLIB_SERIALIZE( authlist, (authID)(type)(requester)(req_pubkey)(status)(req_time)(agree_time)(howlong)(limit_time)(auth_value) )
        };

        typedef eosio::multi_index< N(authlist), authlist> authlist_index;

        //@abi table authstatus i64
        struct authstatus {
            uint64_t        authID;
            account_name    user;
            uint8_t         status = 0;

            uint64_t primary_key()const { return authID; }

            EOSLIB_SERIALIZE( authstatus, (authID)(user)(status) )
        };
        typedef eosio::multi_index< N(authstatus), authstatus> authstatus_index;
    
};

EOSIO_ABI( sic_auth, (reqauth)(agreeauth)(rejectauth)(burnauth) )
} //namespace sic

