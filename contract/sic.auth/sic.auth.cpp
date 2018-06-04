/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 *  @auther: hanfei
 *  @createtime: 2018-05-04
 */
#include <sic.auth.hpp>

namespace sic {

void sic_auth::reqauth(const uint64_t id, 
                    const uint8_t  type,
                    const account_name user, 
                    const account_name requester,
                    const public_key req_pubkey, 
                    const uint32_t howlong,
                    const std::string auth_value) {
    require_auth(requester);
    
    authlist_index auths(_self, user);
    auto getauth = auths.find(id);
    eosio_assert(getauth == auths.end(), "the request auth have exist!");
      
    //TODO: add the dedup
    auths.emplace(requester, [&]( auto& g ) {
        g.authID = id;
        g.type = type;
        g.requester = requester;
        g.req_pubkey = req_pubkey;
        g.status = Status::s_unauth;
        g.req_time = ::now();
        g.howlong = howlong; 
        g.auth_value = auth_value;
    });

    authstatus_index authst(_self, requester);
    authst.emplace(requester, [&]( auto& g ) {
        g.authID = id;
        g.user = user;
        g.status = Status::s_unauth;
    });

}

void sic_auth::agreeauth(const uint64_t id, account_name user, std::string value) {
    require_auth(user);

    authlist_index auths(_self, user);
    auto getauth = auths.find(id);
    eosio_assert(getauth != auths.end(), "the request auth don't exist!!");
    eosio_assert(getauth->status == Status::s_unauth || getauth->status == Status::s_reject,
                 "You have agree this auth!");

    auths.modify( getauth, user, [&]( auto& s ) {
        s.agree_time = ::now();
        s.limit_time = ::now() + getauth->howlong;
        s.status = Status::s_haveauth;
        s.auth_value = value;
    });

    authstatus_index authst(_self, getauth->requester);
    auto getauthst = authst.find(id);
    
    authst.modify( getauthst, user, [&]( auto& g ) {
        g.status = Status::s_haveauth;
    });
    
}

void sic_auth::rejectauth(const uint64_t id, account_name user) {
    require_auth(user);

    authlist_index auths(_self, user);
    auto getauth = auths.find(id);
    eosio_assert(getauth != auths.end(), "the request auth don't exist!!");
    eosio_assert(getauth->status == Status::s_unauth || getauth->status == Status::s_haveauth,
                 "You have reject this auth!!");

    auths.modify( getauth, user, [&]( auto& s ) {
        s.status = Status::s_reject;
    }); 

    authstatus_index authst(_self, getauth->requester);
    auto getauthst = authst.find(id);
    
    authst.modify( getauthst, user, [&]( auto& g ) {
        g.status = Status::s_reject;
    });
}

/// @abi action
void sic_auth::burnauth(const uint64_t id, account_name user)
{
    require_auth(_self);

    authlist_index auths(_self, user);
    auto getauth = auths.find(id);
    eosio_assert(getauth != auths.end(), "the request auth don't exist!!");

    eosio_assert(getauth->limit_time > ::now(), "didn't the time to del!!");

    auths.modify( getauth, _self, [&]( auto& s ) {
        s.status = Status::s_burned;
        s.auth_value = std::string("");
    });

    authstatus_index authst(_self, getauth->requester);
    auto getauthst = authst.find(id);
    
    authst.modify( getauthst, user, [&]( auto& g ) {
        g.status = Status::s_burned;
    });
}


} // namespace sic

