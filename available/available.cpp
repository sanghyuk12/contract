#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT available2: public contract {
public:
using contract::contract;

ACTION dummy() {}

ACTION eraseall(){
    require_auth(get_self());
    SscopeList fc(get_self(), get_self().value );
    auto itrSendScope = fc.begin();

    while(itrSendScope != fc.end()) {
        Send forErase(get_self(), get_self().value);
        auto itr = forErase.begin();
        while(itr != forErase.end()){
            itr=forErase.erase(itr);
        }
        itrSendScope = fc.erase(itrSendScope);
        print("SenderScope삭제");
    }

    RscopeList fctwo(get_self(), get_self().value );
    auto itrReceiveScope = fctwo.begin();
    while(itrReceiveScope != fctwo.end()) {
        Receive forErasetwo(get_self(), get_self().value);
        auto itrtwo = forErasetwo.begin();
        while(itrtwo != forErasetwo.end()){
            itrtwo=forErasetwo.erase(itrtwo);
        }
        itrReceiveScope = fctwo.erase(itrReceiveScope);
        print("ReceiveScope삭제");
    }
}

// ACTION selecterase(){
//     SscopeList fc(get_self(), get_self().value);
//     auto itrSendScope = fc.begin();
//     for(itrSendeScope )

// }

[[eosio::on_notify("eosio.token::transfer")]]
void ontransfer(name from, name to, asset quantity, std::string memo) {
    
    if(from == get_self()) {
        Receive forReceiver(get_self(), to.value );
        forReceiver.emplace(get_self(), [&](auto& row) {
        row.mykey = forReceiver.available_primary_key(); //이것 때문에 키가 1씩증가
        row.user = to;
        row.balance = quantity;
        });

        SscopeList fC(get_self(), get_self().value );
        fC.emplace(get_self(),[&](auto&row){
            row.scope = to.value;
        });
    }
    else {
        Send forSender(get_self(), from.value );
        forSender.emplace(get_self(), [&](auto& row) {
        row.mykey = forSender.available_primary_key();
        row.user = from;
        row.balance = quantity;
        });

        RscopeList fC(get_self(), get_self().value );
        fC.emplace(get_self(),[&](auto&row){
            row.scope = from.value;
        });
    }
}
    private:
    TABLE tradingbook_struct {
        uint64_t mykey;
        name user;
        asset balance;
        uint64_t primary_key() const { return mykey; }
    };
    TABLE scopebook_struct {
        uint64_t scope;
        uint64_t primary_key() const { return scope; }
    };

    typedef multi_index<"senderbook"_n, tradingbook_struct> Send;
    typedef multi_index<"receiverbook"_n, tradingbook_struct> Receive;
    typedef multi_index<"sscopebook"_n, scopebook_struct> SscopeList;
    typedef multi_index<"rscopebook"_n, scopebook_struct> RscopeList;
};