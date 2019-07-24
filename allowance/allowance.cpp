#include <eosio/eosio.hpp>

using namespace eosio;


CONTRACT addressbook: public contract {
public:
using contract::contract;
ACTION hi(name user){  
    check(has_auth(user),"xxxxx");
    require_auth( get_self() );
    print("hello,",user);
    }
//allowance에 등록된 게정만 실행 할 수 있다.

ACTION insert(name user) {
    require_auth(user);

    address_index forInsert(get_self(), get_self().value);
    auto itr = forInsert.find(user.value);

        check(itr == forInsert.end(), "already exists");

        forInsert.emplace(user, [&](auto& row) {
        row.user = user;
    });

    print("insert success");
}
ACTION remove(name user) {
    require_auth(user);

    address_index forErase(get_self(), get_self().value);
    auto itr = forErase.require_find(user.value, "no account");
    forErase.erase(itr);

    print("remove success");
}


private:
struct [[eosio::table]] allowance {
name user;

uint64_t primary_key() const { return user.value; }

};

typedef multi_index<"people"_n, allowance> address_index;

};