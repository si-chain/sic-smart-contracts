/**
 *  @file sic.token.cpp
 *  @copyright defined in eos/LICENSE.txt
 *  @auther: hanfei
 *  @createtime: 2018-04-20
 */
//#include <fc/time.hpp>
#include <sic.token.hpp>

using std::string;

namespace eosio {

/// @abi action
void sic_token::issue( account_name to, asset quantity, string memo ) 
{
    require_auth(_self);
    auto sym = quantity.symbol.name();
    eosio_assert(sym == default_sym.name(), "You can only reward The SIC");
    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must issue positive quantity" );

    const auto& st = stats.find( sym );
    if(st == stats.end()) {
        stats.emplace( _self, [&]( auto& s ) {
            s.supply = quantity;
        });
    } else {
        eosio_assert( st->supply.amount+quantity.amount <= 1600000000000, "You've issued the SIC that exceeds the limit(160000000.0000 SIC)." );
        stats.modify( st, _self, [&]( auto& s ) {
            s.supply += quantity;
        });
    }
    require_recipient( to );

    add_balance( to, quantity, _self );
}

/// @abi action
void sic_token::transfer( account_name from, account_name to, asset quantity, string memo ) 
{
    eosio_assert( from != N(sic.hole), "the account Black.hole can not transfer!");
    require_auth( from );
    auto sym = quantity.symbol.name();
    
    require_recipient( from );
    require_recipient( to );
    
    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
    
    sub_balance( from, quantity );
    add_balance( to, quantity, from );
}

/// @abi action
void sic_token::withdraw( account_name from, asset quantity, string memo )
{
    print("withdraw");   
    
    auto st = stats.get(default_sym.name());
    eosio_assert( st.can_withdraw, "The SIC can't withdraw now!");
    
    require_auth(from);
    auto sym = quantity.symbol.name();
    
    require_recipient( from );
    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0 && quantity.amount <= 1000*10000, "must withdraw positive quantity" );
    
    sub_balance( from, quantity, true );
    add_balance( N(sic.hole), quantity, from );
}

///@abi action 
void sic_token::setfreeze(account_name user, uint8_t status) 
{
    require_auth(_self);
    require_recipient( user );
    account_index to_freeze( _self, user );
    auto to = to_freeze.find( default_sym.name() );
    if( to == to_freeze.end() ) {
        to_freeze.emplace( _self, [&]( auto& a ){
            a.balance = asset(0, S(4,SIC));
            a.frozen = status;
        });
    } else {
        to_freeze.modify( to, 0, [&]( auto& a ) {
            a.frozen = status;
        });
    }
}

///@abi action 
void sic_token::setallfreeze(uint8_t status) 
{
    require_auth(_self);

    auto st = stats.find(default_sym.name());
    if(st == stats.end()) {
        stats.emplace( _self, [&]( auto& s ) {
            s.supply = asset(0, S(4,SIC));
            s.is_frozen = status;
        });
    } else {
        stats.modify( st, _self, [&]( auto& s ) {
            s.is_frozen = status;
        });
    }
}

///@abi action
void sic_token::setwithdraw(uint8_t status)
{
    require_auth(_self);

    auto st = stats.find(default_sym.name());
    if(st == stats.end()) {
        stats.emplace( _self, [&]( auto& s ) {
            s.supply = asset(0, S(4,SIC));
            s.can_withdraw = status;
        });
    } else {
        stats.modify( st, _self, [&]( auto& s ) {
            s.can_withdraw = status;
        });
    }
}


}

