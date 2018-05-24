/**
 *  @file sic_token.cpp
 *  @copyright defined in eos/LICENSE.txt
 *  @auther: hanfei
 *  @createtime: 2018-04-20
 */
//#include <fc/time.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/contract.hpp>

using std::string;

namespace eosio {

    class sic_token : public eosio::contract {
        public:
            using contract::contract;

            sic_token(account_name self)
                :eosio::contract(self),
                stats(_self,_self),
                default_sym{S(4,SIC)}
            {
                
            }

            /// @abi action
            void issue( account_name to, asset quantity, string memo );
            

            /// @abi action
            void transfer( account_name from, account_name to, asset quantity, string memo );
            

            /// @abi action
            void withdraw( account_name from, asset quantity, string memo );
            

            ///@abi action 
            void setfreeze(account_name user, uint8_t status);
            

            ///@abi action 
            void setallfreeze(uint8_t status);

            ///@abi action
            void setwithdraw(uint8_t status);
            

        private:
            /// @abi table account i64
            struct account {
                account( asset sic = asset(0, S(4,SIC)) ):balance(sic){}

                asset       balance;
                time        last_withdraw;
                uint8_t     frozen    = false;
                uint8_t     whitelist = true;

                uint64_t primary_key()const { return balance.symbol.name(); }

                EOSLIB_SERIALIZE( account, (balance)(last_withdraw)(frozen)(whitelist) )
            };

            typedef eosio::multi_index< N(account), account> account_index;


            /// @abi table stat i64
            struct sic_stat {
                sic_stat( asset sic = asset(0, S(4,SIC)) ):supply(sic){}

                asset             supply;
                uint8_t           can_freeze         = true;
                uint8_t           can_whitelist      = true;
                uint8_t           is_frozen          = false;
                uint8_t           can_withdraw       = false;
                uint8_t           enforce_whitelist  = false;

                uint64_t primary_key()const { return supply.symbol.name(); }

                EOSLIB_SERIALIZE( sic_stat, (supply)(can_freeze)(can_whitelist)(is_frozen)(can_withdraw)(enforce_whitelist) )
            };

            typedef eosio::multi_index< N(stat), sic_stat> stat_index;

            stat_index stats;
            eosio::symbol_type default_sym;

            #define S(P,X) ::eosio::string_to_symbol(P,#X)

            void sub_balance( account_name owner, asset value, bool is_withdraw=false) {
                auto st = stats.get(default_sym.name());

                account_index from_acnts( _self, owner );

                const auto& from = from_acnts.get( value.symbol.name() );
                eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );

                if( has_auth( owner ) ) {
                    eosio_assert( !st.can_freeze || !from.frozen, "account is frozen by issuer" );
                    eosio_assert( !st.can_freeze || !st.is_frozen, "all transfers are frozen by issuer" );
                    eosio_assert( !st.enforce_whitelist || from.whitelist, "account is not white listed" );
                } else {
                    eosio_assert( false, "insufficient authority" );
                }

                if( is_withdraw ) {
                    eosio_assert( from.last_withdraw - ::now() < 7200, "you can't withdraw in 2 hours!" );
                }

                from_acnts.modify( from, owner, [&]( auto& a ) {
                    a.balance -= value;
                    a.last_withdraw = ::now();
                });
            }

            void add_balance( account_name owner, asset value, account_name from)
            {
                auto st = stats.get(default_sym.name());

                account_index to_acnts( _self, owner );
                auto to = to_acnts.find( value.symbol.name() );
                if( to == to_acnts.end() ) {
                    eosio_assert( !st.enforce_whitelist, "can only transfer to white listed accounts" );
                    to_acnts.emplace( from, [&]( auto& a ){
                        a.balance = value;
                    });
                } else {
                    eosio_assert( !st.enforce_whitelist || to->whitelist, "receiver requires whitelist by issuer" );
                    to_acnts.modify( to, 0, [&]( auto& a ) {
                        a.balance += value;
                    });
                }
            }  
            
    };
    
EOSIO_ABI( sic_token, (issue)(transfer)(withdraw)(setfreeze)(setallfreeze)(setwithdraw) )
}//namespace eosio

