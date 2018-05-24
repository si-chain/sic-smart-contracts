/**
 *  @file license_list.cpp
 *  @copyright defined in eos/LICENSE.txt
 *  @auther: hanfei
 *  @createtime: 2018-04-17
 */

#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/contract.hpp>

using eosio::indexed_by;
using eosio::const_mem_fun;
using std::string;

class license_list : public eosio::contract {
    public:
        using contract::contract;

        license_list(account_name self)
            :eosio::contract(self),
            infos(_self, _self)
            {}

        /// @abi action
        void agree(const account_name user, const uint64_t versionid,const std::string version) {
            require_auth(user);
            
            license_index licenses(_self, user);

            auto itrinfo = infos.find(versionid);
            eosio_assert(itrinfo != infos.end(), "There is no such license!\n");

            auto itr = licenses.find(versionid);
            eosio_assert(itr == licenses.end(), "you have agree This version!\n");

            licenses.emplace(user, [&]( auto& g ) {
                g.version_ID = versionid;
                g.version = version;
                g.agree_time = publication_time();
            });
        }

        /// @abi action 
        void setlicense(const uint64_t versionid, const std::string version, const std::string license_info) {
            require_auth(_self);

            auto itr = infos.find(versionid);
            eosio_assert(itr == infos.end(), "The version have upload!\n");

            infos.emplace(_self, [&]( auto& g ) {
                g.version_ID = versionid;
                g.version = version;
                g.info = license_info;
            });
        }

    private:

        /// @abi table license i64
        struct license {
            uint64_t version_ID;
            std::string version;
            time agree_time;

            uint64_t primary_key()const { return version_ID; }

            EOSLIB_SERIALIZE( license, (version_ID)(version)(agree_time) )
        };

        typedef eosio::multi_index< N(license), license> license_index;

        /// @abi table licenseinfo i64
        struct licenseinfo {
            uint64_t version_ID;
            std::string version;
            std::string info;

            uint64_t primary_key()const { return version_ID; }

            EOSLIB_SERIALIZE( licenseinfo, (version_ID)(version)(info) )
        };

        typedef eosio::multi_index< N(licenseinfo), licenseinfo> info_index;

        info_index infos;
};

EOSIO_ABI( license_list, (agree)(setlicense) )