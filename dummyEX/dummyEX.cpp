#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT dummyEX: public contract {
public:
using contract::contract;

ACTION dummy(name user) {
    require_auth(user);

    print("dummy");

    action(
        // permission
        permission_level(user, "active"_n),     //actor:계정이름 //active: 권한
        // contract name
        get_self(),
        //action name,
        "dummytwo"_n,
        //parameters
        std::make_tuple(user)
    ).send();
}

ACTION dummytwo(name user) {
    require_auth(user);
    print("dummy two, ", user);
}
private:
};