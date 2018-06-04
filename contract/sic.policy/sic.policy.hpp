/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 *  @auther: hanfei
 *  @createtime: 2018-04-16
 */

#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/contract.hpp>

using eosio::indexed_by;
using eosio::const_mem_fun;
using std::string;

namespace sic {

class sic_policy : public eosio::contract {
    public:
        using contract::contract;
        sic_policy(account_name self)
            :eosio::contract(self),
            accounts(_self, _self),
            idlists(_self, _self),
            unapproveds(_self, _self)
            {}

        /// @abi action
        void upload(const uint64_t id, 
                    const uint8_t type,
                    const account_name producer, 
                    const std::string ossAddr, 
                    const uint8_t upload_num);

        /// @abi action
        void reviewing(const uint64_t ID, const account_name reviewer);

        /// @abi action
        void approved(const uint64_t ID, const std::string policyID, const std::string value,
                    account_name account,
                    std::string type, std::string param1, std::string param2, std::string param3,
                    std::string param4, std::string param5);

        /// @abi action
        void disapprove(const uint64_t ID, std::string reason);

        /// @abi action
        void remove(const uint64_t ID);


    private:
        enum Status {
            s_uploaded=0,
            s_reviewing=10,
            s_approved=20,
            s_disapprove=30
        };

        enum DateType {
            Policy=0,
            Claim=1
        };

        //@abi table account i64
        struct account {
            account( account_name o = account_name() ):owner(o){}

            account_name owner;
            uint32_t     policynum = 0;

            bool is_empty()const { return !policynum; }

            uint64_t primary_key()const { return owner; }

            EOSLIB_SERIALIZE( account, (owner)(policynum) )
        };

        typedef eosio::multi_index< N(account), account> account_index;

        struct idlist {
            uint64_t ID;
            account_name producer;

            uint64_t primary_key()const { return ID; }

            EOSLIB_SERIALIZE( idlist, (ID)(producer) )
        };
        typedef eosio::multi_index< N(idlist), idlist> idlist_index;

        //@abi table unapproved i64
        struct unapproved {
            uint64_t ID;
            uint8_t type;
            account_name producer;
            uint8_t upload_num;
            time upload_time;

            uint64_t primary_key()const { return ID; }

            EOSLIB_SERIALIZE( unapproved, (ID)(type)(producer)(upload_num)(upload_time) )
        };
        typedef eosio::multi_index< N(unapproved), unapproved> unapproved_index;

        //@abi table reviewlist i64
        struct reviewlist {
            uint64_t ID;
            std::string ossAddr;
            account_name producer;

            uint64_t primary_key()const { return ID; }

            EOSLIB_SERIALIZE( reviewlist, (ID)(ossAddr)(producer) )
        };
        typedef eosio::multi_index< N(reviewlist), reviewlist> reviewlist_index;

        //@abi table policy i64
        struct policy {
            uint64_t ID;
            uint8_t type;
            uint8_t status;
            std::string ossAddr;
            account_name producer;
            account_name reviewer;
            uint8_t upload_num;
            time upload_time;
            time reveiwing_time;
            time approved_time;
            std::string policyID; 
            std::string value;

            auto primary_key() const { return ID; }

            /*
            key256 by_policyID() const { return get_policyID(policyID); }

            static key256 get_policyID(const checksum256& policyID) {
                const uint64_t *p64 = reinterpret_cast<const uint64_t *>(&policyID);
                return key256::make_from_word_sequence<uint64_t>(p64[0], p64[1], p64[2], p64[3]);
            }*/

            EOSLIB_SERIALIZE( policy, (ID)(type)(status)(ossAddr)(producer)(reviewer)(upload_num)(upload_time)(reveiwing_time)(approved_time)(policyID)(value) )
        }; 

        /*
        typedef eosio::multi_index< N(policy), policy,
            indexed_by< N(policyID), const_mem_fun<policy, key256, &policy::by_policyID > >
        > policy_index;*/
         typedef eosio::multi_index< N(policy), policy> policy_index;

        account_index accounts;
        idlist_index idlists;
        unapproved_index unapproveds;

        // get the code's policynum
        uint32_t get_policynum() {
            auto itr = accounts.find( _self );
            if ( itr == accounts.end() ) {
                return 0;
            } else {
                return itr->policynum;
            }
        }

        /**
         * to operate the account's policynum
         * op : '+','-'
         */
        void policynum_op(account_name name, char op) {
            auto itr = accounts.find( name );
            if ( itr == accounts.end() ) {
                accounts.emplace(name, [&]( auto& g ) {
                    g.owner = name;
                    g.policynum = 1;
                });
            } else {
                if( op == '+' ) {
                    accounts.modify(itr, itr->owner, []( auto& g ) {g.policynum += 1;});
                } else if( op == '-' ) {
                    accounts.modify(itr, itr->owner, []( auto& g ) {g.policynum -= 1;});
                }
            }

            itr = accounts.find( _self );
            if ( itr == accounts.end() ) {
                accounts.emplace(_self, [&]( auto& g ) {
                    g.owner = _self;
                    g.policynum = 1;
                });
            } else {
                if( op == '+' ) {
                    accounts.modify(itr, itr->owner, []( auto& g ) {g.policynum += 1;});
                }
            }
        }
    
};
EOSIO_ABI( sic_policy, (upload)(reviewing)(approved)(disapprove)(remove) )
} //namespace sic

