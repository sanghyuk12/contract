#include <eosio/eosio.hpp> 
#include <eosio/asset.hpp>
//각 테이블에 name type변수를 선언  => EOS를 보낸사람 || EOS를 받은사람 그리고 얼마나 받았는지 기록
using namespace eosio;
using namespace std;

CONTRACT onnotify: public contract {
    public:
    using contract::contract;
    
    ACTION dummy() {}
    
    [[eosio::on_notify("eosio.token::transfer")]]
    
    void ontransfer(name from, name to, asset quantity, std::string memo) {
        
        if(from == get_self()) {
            outs myTable(get_self(), get_self().value);
            auto itr = myTable.find(to.value);


            if(itr == myTable.end()) {
                myTable.emplace(from, [&](auto& row) {
                    row.user = to;
                    row.balance = quantity;
                });
            }
            else {
                auto itr = myTable.begin();
                myTable.modify(itr, from, [&](auto& row) {
                    row.balance += quantity;
                });
            }
        }

        else if(to == get_self()){
            ins myTable(get_self(),get_self().value);
            auto itr = myTable.find(from.value);

            if(itr == myTable.end()){
                myTable.emplace(to,[&](auto& row){
                    row.user = from;
                    row.balance = quantity;
                });
            }
            else {
                auto itr = myTable.begin();
                myTable.modify(itr,to,[&](auto& row) {
                    row.balance +=quantity;
                });
            }
        }
    }


    private:
    TABLE outstruct {
        asset balance;
        name user;
        uint64_t primary_key() const { return user.value; }
        }; 
    

typedef multi_index<"out3"_n, outstruct> outs;
typedef multi_index<"in3"_n, outstruct> ins;
};