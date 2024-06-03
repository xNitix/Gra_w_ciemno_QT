#include "trade.h"
#include <random>
#include "letter.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>

using namespace std;

std::vector<Letter *> Trade::getHost_Letters() const
{
    return host_Letters;
}

int Trade::getPlayer_money() const
{
    return player_money;
}

int Trade::propability()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(10000, 8000);

    double randomNumber = distribution(gen);

    int boundedRandomNumber = std::max(0, std::min(100000, static_cast<int>(randomNumber)));

    return boundedRandomNumber;
}

std::vector<Letter *> Trade::getPlayer_Letters() const
{
    return player_Letters;
}

bool Trade::is_offert_accepted(std::vector<int> letter_to_sell, int price_for_letters, int price)
{
    /*
    cout << "KOPERTY NA SPRZEDAZ: ";
    for(int nr: letter_to_sell){
        cout << nr << " ";
    }cout << " za: "<< price_for_letters << endl;

    cout << " KOPERTY HISTORIA: " << endl; */
    for(vector<int> offert : offerts_history){
        /*
        for(int nr : offert){
            cout << nr <<" ";
        }cout << endl; */

        int isAccepted = offert[0];
        int priceHistory = offert[offert.size()-1];
        int letters_amount = offert.size()-2;
        int same_letters_amount = 0;
        for(int i=1; i < offert.size() - 1; i++){ //iterowanie po nr kopert w historii
            for(int nr : letter_to_sell){
                if(nr == offert[i]){
                    same_letters_amount++;
                }
            }
        }
        if(letters_amount == letter_to_sell.size() and
            same_letters_amount == letters_amount and
            priceHistory == price_for_letters)
        {
            cout << "OFERTA JUZ BYLA, KOPERTY: ";
            /*
            for(int i=1; i< offert.size()-1; i++){
                cout<<offert[i]<<" ";
            }
            cout<<endl<<"ZA: "<< priceHistory<<endl; */
            return false;
        }
    }



    //std::cout << "TARGET: " << target_value << "  ILE MAMY W TEORII PO ofercie: " << price << std::endl;
    double difference = std::abs(price - target_value);

    bool offert_res;
    if (difference < 2500) {
        // Szansa 95%
        offert_res = (rand() % 100) < 95;
    } else if (difference >= 2500 && difference < 5000) {
        // Szansa 70%
        offert_res = (rand() % 100) < 70;
    } else if (difference >= 5000 && difference < 10000) {
        // Szansa 40%
        offert_res = (rand() % 100) < 40;
    } else if (difference >= 10000 && difference < 15000) {
        // Szansa 10%
        offert_res = (rand() % 100) < 10;
    } else if (difference >= 15000 && difference < 25000) {
        // Szansa 3%
        offert_res = (rand() % 100) < 3;
    } else {
        // Dla większych różnic szansa 1%
        offert_res = (rand() % 100) < 1;
    }
    letter_to_sell.insert(letter_to_sell.begin(), (int) offert_res);
    letter_to_sell.push_back(price_for_letters);
    offerts_history.push_back(letter_to_sell);
    return offert_res;
}
Trade::Trade()
{
    target_value = propability();
    cout << "WYLOSOWALEM: "<< target_value << endl;
}

bool Trade::player_offer(std::vector<int> letter_to_sell, int price_for_letters)
{
    int sum = 0;
    int minus100 = 1;
    float minus50 = 1;
    for(Letter* letter : player_Letters){
        int player_letter_nr = letter->getNr();
        bool som = false;
        for(int nr : letter_to_sell){
            if(nr == player_letter_nr){
                som = true;
            }
        }
        if(! som){
            if(letter->get_value() ==  -100){
                minus100 = 0;
            } else if(letter->get_value() ==  -50){
                minus50 = 0.5;
            } else {
                sum += letter->get_value();
            }
        }
    }
    //cout << "SUMA PO SUMIE: "  << sum << endl;
    return is_offert_accepted(letter_to_sell, price_for_letters,(sum + price_for_letters + this->player_money)*(minus100)*(minus50));
}

void Trade::start_trade()
{
    wasTradeAccepted = false;
    std::vector<int> letter_to_sell;

    std::cout << "letter1: " + std::to_string(chosenLetter[1]) +
    " letter2: " + std::to_string(chosenLetter[2]) +
    " letter3: " + std::to_string(chosenLetter[3]) +
    " letter4: " + std::to_string(chosenLetter[4]) +
    " letter5: " + std::to_string(chosenLetter[5]) << std::endl;
    for(Letter* letter : player_Letters){
        cout << letter->get_value() << "  |  ";
    }
    cout << endl;
    //std::cout << "trade price: " + std::to_string(trade_price) << std::endl;

    for(int i=1; i <= 5; i++){
        if(chosenLetter[i]){
            letter_to_sell.push_back(index_to_letter_number_map[i]);
        }
    }
    if(letter_to_sell.size() == 0){
        return;
    }
    //cout << "TEST OFERTY" << endl;
    bool result = player_offer(letter_to_sell, trade_price);
    if(result){
        cout << "Transakcja powiodla sie!" << endl;
        wasTradeAccepted = true;
        move_letters_to_host(letter_to_sell, trade_price);
    }
}

void Trade::move_letters_to_host(std::vector<int> letter_to_sell, int trade_price){
    std::vector<Letter*> new_player_Letters;
    player_money += trade_price;
    for(Letter* letter : player_Letters){
        bool was_added = false;
        for(int nr : letter_to_sell){
            if(letter->getNr() == nr){
                host_Letters.push_back(letter);
                was_added = true;
            }
        }
        if(!was_added){
            new_player_Letters.push_back(letter);
        }
    }
    player_Letters = new_player_Letters;
}

void Trade::set_player_letters(std::vector<Letter *> player_Letters)
{
    this->player_Letters = player_Letters;
    for(Letter* letter : player_Letters){
        if(letter->get_value()< -100 or letter->get_value() > 100000){
            cout<< letter->get_value() << "  " << letter->getQuestion().getNumber();
            cout << "   " << letter->getWin() << "  " << "JDJDJDJDJD" << endl;


            letter->repair_value();
        }
    }
}

void Trade::init_letters_map(){
    int index = 1;
    for(Letter* letter : player_Letters){
        index_to_letter_number_map[index] = letter->getNr();
        index++;
    }

}

void Trade::set_trade_price(int price)
{
    this->trade_price = price;
}

void Trade::generujKombinacje() {
    int combinations_count = 0;
    if((player_Letters.size() + host_Letters.size()) > 2){
        combinations_count = 5;
    } else if((player_Letters.size() + host_Letters.size())== 2){
        combinations_count = 3;
    } else {
        combinations_count = 1;
    }

    float player_cash_in_letter;
    for(Letter* letter : player_Letters){
        player_cash_in_letter += letter->get_value();
    }

    int k = 0;

    //while(k < combinations_count){
    int player_let_num = rand() % player_Letters.size();
    int host_let_num = rand() % host_Letters.size();
    std::random_shuffle(player_Letters.begin(), player_Letters.end());
    std::random_shuffle(host_Letters.begin(), host_Letters.end());

    int player_letter_cash = 0;
    int host_letter_cash = 0;
    vector<int> letters_num;
    vector<int> letters_host_num;
    for(int j = 0; j < player_let_num; j++){
        player_letter_cash += getPlayer_Letters()[j]->get_value();
        letters_num.push_back(getPlayer_Letters()[j]->getNr());
    }

    for(int j = 0; j < host_let_num; j++){
        host_letter_cash += getHost_Letters()[j]->get_value();
        letters_host_num.push_back(getHost_Letters()[j]->getNr());
    }

    float price = propability();
    if(player_offer(letters_num, host_letter_cash + price)){
        k += 1;
        letter_give = letters_num;
        letter_take = letters_host_num;
        money_for_trade.push_back(price);
    }
    //}
}

std::vector<vector<int>> Trade::take_offer()
{
    generujKombinacje();
    vector<vector<int>> res;
    res.push_back(letter_give);
    res.push_back(letter_take);
    res.push_back(money_for_trade);
    return res;
}


// std::vector<std::vector<int> > Trade::take_new_offer()
// {

// }


