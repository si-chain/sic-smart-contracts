/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 *  @auther: hanfei
 *  @createtime: 2018-04-16
 */
#include <sic.policy.hpp>
#include <../sic.role/sic.role.hpp>

namespace sic {

void sic_policy::upload(const uint64_t id, const uint8_t type, const account_name producer, const std::string ossAddr, const uint8_t upload_num) {
    require_auth(producer);
    
    policy_index upload_policys(_self, producer);
    auto get_time = publication_time();
    //TODO: add the dedup
    upload_policys.emplace(producer, [&]( auto& g ) {
        g.ID = id;
        g.type = type;
        g.status = Status::s_uploaded;
        g.ossAddr = ossAddr;
        g.producer = producer;
        g.upload_num = upload_num;
        g.upload_time = get_time;
    });
    idlists.emplace(_self, [&]( auto& g ) {
        g.ID = id;
        g.producer = producer;
    });
    unapproveds.emplace(_self, [&]( auto& g ) {
        g.ID = id;
        g.type = type;
        g.producer = producer;
        g.upload_num = upload_num;
        g.upload_time = get_time;
    });
    policynum_op(producer, '+');
}


void sic_policy::reviewing(const uint64_t ID, const account_name reviewer) {
    sic_role::rolelist_index roles(N(sic.role), N(sic.role));
    auto role = roles.find(reviewer);
    eosio_assert(role != roles.end(), "You are not reviewer!");
    eosio_assert(role->role_type == 1, "You are not reviewer!");

    require_auth(reviewer);
    
    auto itrid = idlists.find(ID);
    eosio_assert(itrid != idlists.end(), "this Policy doesn't exists!\n");
    policy_index review_policys(_self, itrid->producer);
    auto itr = review_policys.find( ID );
    eosio_assert(itr != review_policys.end(), "this Policy doesn't exists!\n");
    eosio_assert(itr->producer != reviewer, "you can't review youself!\n");
    eosio_assert(itr->status == Status::s_uploaded, "this Policy is reviewing or reviewed!\n");
    
    review_policys.modify(itr, itr->reviewer, [&](auto& g){
        g.status = Status::s_reviewing;
        g.reviewer = reviewer;
        g.reveiwing_time = publication_time();
    });
    auto unappr = unapproveds.find(ID);
    if( unappr != unapproveds.end() ) {
        unapproveds.erase(unappr);
    } 
    reviewlist_index reviews(_self, reviewer);
    reviews.emplace(reviewer, [&]( auto& g ) {
        g.ID = ID;
        g.ossAddr = itr->ossAddr;
        g.producer = itr->producer;
    });
    require_recipient(itrid->producer);
}


void sic_policy::approved(const uint64_t ID, const std::string policyID, const std::string value,
                account_name account,
                std::string type, std::string param1, std::string param2, std::string param3,
                    std::string param4, std::string param5) 
{
    auto itrid = idlists.find(ID);
    eosio_assert(itrid != idlists.end(), "this Policy doesn't exists!\n");
    policy_index approve_policys(_self, itrid->producer);
    auto itr = approve_policys.find( ID );
    eosio_assert(itr != approve_policys.end(), "this Policy doesn't exists!\n");
    eosio_assert(itr->status == Status::s_reviewing, "this Policy is reviewed!\n");
    require_auth(itr->reviewer);
    
    approve_policys.modify(itr, itr->reviewer, [&](auto& g){
        g.status = Status::s_approved;
        g.approved_time = publication_time();
        g.policyID = policyID;
        g.value = value;
    });
    reviewlist_index reviews(_self, itr->reviewer);
    auto reviewed = reviews.find(ID);
    if( reviewed != reviews.end() ) {
        reviews.erase(reviewed);
    } 
    require_recipient(itrid->producer);
}


void sic_policy::disapprove(const uint64_t ID, std::string reason) {
    auto itrid = idlists.find(ID);
    eosio_assert(itrid != idlists.end(), "this Policy doesn't exists!\n");
    policy_index disappr_policys(_self, itrid->producer);
    auto itr = disappr_policys.find( ID );
    eosio_assert(itr != disappr_policys.end(), "this Policy doesn't exists!\n");
    eosio_assert(itr->status == Status::s_reviewing, "this Policy is reviewed!!\n");
    require_auth(itr->reviewer);
    
    disappr_policys.modify(itr, itr->reviewer, [&](auto& g){
        g.status = Status::s_disapprove;
        g.value = reason;
    });
    reviewlist_index reviews(_self, itr->reviewer);
    auto reviewed = reviews.find(ID);
    if( reviewed != reviews.end() ) {
        reviews.erase(reviewed);
    } 
    require_recipient(itrid->producer);
}


void sic_policy::remove(const uint64_t ID) {
    auto itrid = idlists.find(ID);
    require_auth(itrid->producer);
    eosio_assert(itrid != idlists.end(), "this Policy doesn't exists!\n");
    policy_index remove_policys(_self, itrid->producer);
    auto itr = remove_policys.find( ID );
    eosio_assert(itr != remove_policys.end(), "this Policy doesn't exists!\n");
    eosio_assert(itr->status == Status::s_uploaded || itr->status == Status::s_disapprove, "this Policy is reviewing or reviewed!!\n");
    
    remove_policys.erase(itr);
}


} // namespace sic

